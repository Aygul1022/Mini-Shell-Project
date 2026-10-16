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
            output, errors = self.run_shell("echo hello | cat | wc -c\nexit\n", directory)
            self.assertEqual(output, "")
            self.assertIn("pipelines await executor integration", errors)

    def test_external_redirection(self):
        with tempfile.TemporaryDirectory() as directory:
            output, errors = self.run_shell(
                "echo first > output.txt\necho second >> output.txt\ncat < output.txt\nexit\n", directory,
            )
            self.assertEqual(output, "")
            self.assertFalse((pathlib.Path(directory) / "output.txt").exists())
            self.assertEqual(errors.count("redirection awaits executor integration"), 3)

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
