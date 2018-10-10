Important changes in TypingBreaks
=================================

Older versions of the plugin used to provide EEPROM storage for the settings
only optionally, when it was explicitly enabled via the
`TypingBreaks.enableEEPROM()` method. Similarly, the Focus hooks were optional
too.

Both of them are unconditionally enabled now, because they add so much to the
plugin. This means that any calls to `TypingBreaks.enableEEPROM()` can be safely
removed, the method is a no-op by now.
