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
    def test_oneshot_auto_mods_persistence(self):
        _, _ = run_focus_command("version")

        # Initially try to erase eeprom and expect an error
        _, returncode = run_focus_command("eeprom.erase", hide_stderr=True)
        self.assertNotEqual(returncode, 0, "Eeprom erase should fail but did not.")

        #erasing eeprom takes a moment
        time.sleep(5)

        # Verify initial oneshot.auto_mods is '1'
        initial_mode, _ = run_focus_command("oneshot.auto_mods")
        self.assertEqual(initial_mode, '0', "Initial oneshot.auto_mods should be '0'")
        
        # Change oneshot.auto_mods to 0 and verify
        run_focus_command("oneshot.auto_mods 0")
        mode_after_setting_to_0, _ = run_focus_command("oneshot.auto_mods")
        self.assertEqual(mode_after_setting_to_0, '0', "oneshot.auto_mods should be '0' after setting to 0")
        
        # Reset device and expect an error
        _, returncode = run_focus_command("device.reset", hide_stderr=True)
        self.assertNotEqual(returncode, 0, "Device reset should fail but did not.")
        time.sleep(5)  # Wait for the device to potentially reset

        # Verify oneshot.auto_mods is still 0 after reset
        mode_after_reset, _ = run_focus_command("oneshot.auto_mods")
        self.assertEqual(mode_after_reset, '0', "oneshot.auto_mods should remain '0' after reset")
        
        # Change oneshot.auto_mods to 1 and verify
        run_focus_command("oneshot.auto_mods 1")
        mode_after_setting_to_1, _ = run_focus_command("oneshot.auto_mods")
        self.assertEqual(mode_after_setting_to_1, '1', "oneshot.auto_mods should be '1' after setting to 1")
        
        # Reset device again and expect an error
        _, returncode = run_focus_command("device.reset", hide_stderr=True)
        self.assertNotEqual(returncode, 0, "Device reset should fail but did not.")
        time.sleep(5)  # Wait for the device to potentially reset
        
        # Verify oneshot.auto_mods is still 1 after reset
        mode_final, _ = run_focus_command("oneshot.auto_mods")
        self.assertEqual(mode_final, '1', "oneshot.auto_mods should remain '1' after final reset")
        
        # Try to erase eeprom again and expect an error
        _, returncode = run_focus_command("eeprom.erase", hide_stderr=True)
        self.assertNotEqual(returncode, 0, "Eeprom erase should fail but did not.")
        
        time.sleep(5)  # Wait for the device to potentially reset

        # Verify oneshot.auto_mods is still 1 after attempting to erase eeprom
        mode_after_erase, _ = run_focus_command("oneshot.auto_mods")
        self.assertEqual(mode_after_erase, '0', "oneshot.auto_mods should remain '0' after attempting to erase eeprom")

if __name__ == '__main__':
    unittest.main()
