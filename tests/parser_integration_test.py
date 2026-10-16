"""Run against a freshly built Linux mini-shell; uses only Python's stdlib."""
import pathlib
import subprocess
import sys
import tempfile
import time
import unittest


if len(sys.argv) != 2:
    raise SystemExit("Usage: python3 tests/parser_integration_test.py /path/to/minishell")
EXECUTABLE = str(pathlib.Path(sys.argv.pop()).resolve())


class ParserIntegrationTests(unittest.TestCase):
    def run_shell(self, commands, cwd):
        result = subprocess.run(
            [EXECUTABLE], input=commands, text=True, capture_output=True,
            cwd=cwd, timeout=5,
        )
        self.assertEqual(result.returncode, 0, result.stderr)
        output = result.stdout.replace("mini-shell> ", "")
        return output, result.stderr

    def test_whitespace_and_exit(self):
        with tempfile.TemporaryDirectory() as directory:
            output, errors = self.run_shell("\n \t\n  pwd \t\n  exit  \npwd\n", directory)
            self.assertEqual(output.splitlines(), [str(pathlib.Path(directory).resolve())])
            self.assertEqual(errors, "")

    def test_quoted_paths(self):
        with tempfile.TemporaryDirectory() as directory:
            target = pathlib.Path(directory) / "folder with spaces"
            target.mkdir()
            output, errors = self.run_shell(
                'cd "folder with spaces"\npwd\ncd ..\n'
                "cd 'folder with spaces'\npwd\nexit\n", directory,
            )
            self.assertEqual(output.splitlines(), [str(target.resolve())] * 2)
            self.assertEqual(errors, "")

    def test_syntax_error_recovery(self):
        with tempfile.TemporaryDirectory() as directory:
            for command in ['cd "unfinished', 'pwd >', '| pwd']:
                with self.subTest(command=command):
                    output, errors = self.run_shell(command + "\npwd\nexit\n", directory)
                    self.assertEqual(output.splitlines(), [str(pathlib.Path(directory).resolve())])
                    self.assertTrue(errors.strip())

    def test_unsupported_forms_have_no_side_effects(self):
        with tempfile.TemporaryDirectory() as directory:
            for command in ['cd / &', 'cd / | pwd', 'cd / > output.txt',
                            'pwd < missing.txt', 'exit &', 'exit | pwd',
                            'exit > output.txt', 'echo hi | cat > output.txt']:
                with self.subTest(command=command):
                    output, errors = self.run_shell(command + "\npwd\nexit\n", directory)
                    self.assertEqual(output.splitlines(), [str(pathlib.Path(directory).resolve())])
                    if command in ['cd / > output.txt', 'pwd < missing.txt', 'exit > output.txt']:
                        self.assertIn("redirection for built-ins is not supported", errors)
                    elif command in ['cd / | pwd', 'exit | pwd']:
                        self.assertIn("built-ins in pipelines are not supported", errors)
                    else:
                        self.assertIn("executor integration", errors)
                    self.assertFalse((pathlib.Path(directory) / "output.txt").exists())

    def test_external_execution_and_recovery(self):
        with tempfile.TemporaryDirectory() as directory:
            output, errors = self.run_shell(
                'echo "hello world"\nminishell_nonexistent_command_72519\npwd\nexit\n', directory,
            )
            self.assertEqual(output.splitlines(), ["hello world", str(pathlib.Path(directory).resolve())])
            self.assertIn("minishell_nonexistent_command_72519", errors)

    def test_foreground_wait(self):
        with tempfile.TemporaryDirectory() as directory:
            started = time.monotonic()
            output, errors = self.run_shell("sleep 1\npwd\nexit\n", directory)
            self.assertGreaterEqual(time.monotonic() - started, 0.9)
            self.assertEqual(output.splitlines(), [str(pathlib.Path(directory).resolve())])
            self.assertEqual(errors, "")

    def test_external_inherits_changed_directory(self):
        with tempfile.TemporaryDirectory() as directory:
            target = pathlib.Path(directory) / "child"
            target.mkdir()
            (target / "marker.txt").write_text("marker")
            output, errors = self.run_shell("cd child\nls\nexit\n", directory)
            self.assertEqual(output.splitlines(), ["marker.txt"])
            self.assertEqual(errors, "")

    def test_external_pipeline(self):
        with tempfile.TemporaryDirectory() as directory:
            for command in ["echo hello | wc -c", "echo hello | cat | wc -c"]:
                with self.subTest(command=command):
                    output, errors = self.run_shell(command + "\necho done\nexit\n", directory)
                    self.assertEqual([line.strip() for line in output.splitlines()], ["6", "done"])
                    self.assertEqual(errors, "")

    def test_pipeline_large_output(self):
        with tempfile.TemporaryDirectory() as directory:
            output, errors = self.run_shell("seq 1 100000 | cat | wc -l\nexit\n", directory)
            self.assertEqual(output.strip(), "100000")
            self.assertEqual(errors, "")

    def test_pipeline_missing_program_recovery(self):
        with tempfile.TemporaryDirectory() as directory:
            missing = "minishell_nonexistent_command_72519"
            for command in [missing + " | cat", "echo hi | " + missing]:
                with self.subTest(command=command):
                    output, errors = self.run_shell(command + "\necho recovered\nexit\n", directory)
                    self.assertEqual(output.splitlines(), ["recovered"])
                    self.assertIn(missing, errors)

    def test_pipeline_validates_all_stages_before_execution(self):
        with tempfile.TemporaryDirectory() as directory:
            for command in ['touch marker | pwd', 'touch marker | ""',
                            'touch marker | cat > output.txt']:
                with self.subTest(command=command):
                    output, errors = self.run_shell(command + "\necho recovered\nexit\n", directory)
                    self.assertEqual(output.splitlines(), ["recovered"])
                    self.assertTrue(errors.strip())
                    self.assertFalse((pathlib.Path(directory) / "marker").exists())
                    self.assertFalse((pathlib.Path(directory) / "output.txt").exists())

    def test_external_redirection(self):
        with tempfile.TemporaryDirectory() as directory:
            output, errors = self.run_shell(
                "echo first > output.txt\necho second >> output.txt\ncat < output.txt\nexit\n", directory,
            )
            self.assertEqual(output.splitlines(), ["first", "second"])
            self.assertEqual((pathlib.Path(directory) / "output.txt").read_text(), "first\nsecond\n")
            self.assertEqual(errors, "")

    def test_redirection_truncation_and_prompt_preservation(self):
        with tempfile.TemporaryDirectory() as directory:
            target = pathlib.Path(directory) / "output with spaces.txt"
            target.write_text("old content that must disappear\n")
            output, errors = self.run_shell(
                'echo new > "output with spaces.txt"\npwd\necho visible\nexit\n', directory,
            )
            self.assertEqual(target.read_text(), "new\n")
            self.assertEqual(output.splitlines(), [str(pathlib.Path(directory).resolve()), "visible"])
            self.assertEqual(errors, "")

    def test_combined_input_output_redirection(self):
        with tempfile.TemporaryDirectory() as directory:
            source = pathlib.Path(directory) / "input.txt"
            source.write_text("first\nsecond\n")
            output, errors = self.run_shell("cat < input.txt > copy.txt\necho done\nexit\n", directory)
            self.assertEqual((pathlib.Path(directory) / "copy.txt").read_text(), source.read_text())
            self.assertEqual(output.splitlines(), ["done"])
            self.assertEqual(errors, "")

    def test_redirection_failure_recovery(self):
        with tempfile.TemporaryDirectory() as directory:
            for command in ["cat < absent.txt", "echo hidden > absent-dir/output.txt"]:
                with self.subTest(command=command):
                    output, errors = self.run_shell(command + "\necho recovered\npwd\nexit\n", directory)
                    self.assertEqual(output.splitlines(), ["recovered", str(pathlib.Path(directory).resolve())])
                    self.assertTrue(errors.strip())
                    self.assertFalse((pathlib.Path(directory) / "absent.txt").exists())
                    self.assertFalse((pathlib.Path(directory) / "absent-dir").exists())

    def test_argument_validation_and_empty_command(self):
        with tempfile.TemporaryDirectory() as directory:
            for command in ['pwd extra', 'help extra', 'exit extra', 'cd / /tmp', '""']:
                with self.subTest(command=command):
                    output, errors = self.run_shell(command + "\npwd\nexit\n", directory)
                    self.assertEqual(output.splitlines(), [str(pathlib.Path(directory).resolve())])
                    self.assertTrue(errors.strip())

    def test_help_and_final_line(self):
        with tempfile.TemporaryDirectory() as directory:
            output, errors = self.run_shell("help\nhelp\nexit\n", directory)
            self.assertEqual(output.count("Mini Shell - available commands"), 2)
            self.assertEqual(errors, "")
            output, errors = self.run_shell("pwd", directory)
            self.assertEqual(output.splitlines(), [str(pathlib.Path(directory).resolve())])
            self.assertEqual(errors, "")


if __name__ == "__main__":
    unittest.main()
