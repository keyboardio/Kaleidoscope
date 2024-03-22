import subprocess
import unittest
import time

def run_focus_command(command, hide_stderr=False):
    """Execute a focus send command, print the command and its output, and return its output."""
    stderr_setting = subprocess.DEVNULL if hide_stderr else subprocess.PIPE
    print(f"Running command: {command}")
    completed_process = subprocess.run(f"focus send {command}", shell=True, stdout=subprocess.PIPE, stderr=stderr_setting, text=True)
    print(f"Result: {completed_process.stdout.strip()}")
    if completed_process.returncode != 0 and not hide_stderr:
        print(f"Error: {completed_process.stderr.strip()}" if completed_process.stderr else "Command failed with no error output.")
    return completed_process.stdout.strip(), completed_process.returncode


class TestFocusCommands(unittest.TestCase):
    def test_focus_commands(self):
        # Test 'focus send version', log the result, and store as initial version
        initial_version, _ = run_focus_command("version")
        
        # Test 'focus send escape_oneshot.cancel_key' and log the result
        cancel_key_result_initial, _ = run_focus_command("escape_oneshot.cancel_key")
        
        run_focus_command("escape_oneshot.cancel_key 45")

        new_version, _ = run_focus_command("escape_oneshot.cancel_key")
        self.assertEqual(new_version, '45', "New version should be '45'")

        # Expect 'focus send device.reset' to error
        reset_result, returncode = run_focus_command("device.reset", hide_stderr=True)
        self.assertNotEqual(returncode, 0, "Expected non-zero exit code for device.reset")
        time.sleep(2)  # Add a 5-second delay here
        
        new_version, _ = run_focus_command("escape_oneshot.cancel_key")
        self.assertEqual(new_version, '45', "New version should be '45'")


        # Set new value for escape_oneshot.cancel_key and verify no action needed
        run_focus_command("escape_oneshot.cancel_key 30")
        
        # Fetch new version and ensure it's '30'
        new_version, _ = run_focus_command("escape_oneshot.cancel_key")
        self.assertEqual(new_version, '30', "New version should be '30'")
        
        # Expect 'focus send device.reset' to error
        reset_result, returncode = run_focus_command("device.reset", hide_stderr=True)
        self.assertNotEqual(returncode, 0, "Expected non-zero exit code for device.reset")
        time.sleep(2)  # Add a 5-second delay here
        
        # Test 'focus send escape_oneshot.cancel_key' again and ensure it matches new version
        final_version, _ = run_focus_command("escape_oneshot.cancel_key")
        self.assertEqual(final_version, new_version, "Final version should match new version after reset")

if __name__ == '__main__':
    unittest.main()
