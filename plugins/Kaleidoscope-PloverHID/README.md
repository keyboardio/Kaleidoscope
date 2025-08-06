# PloverHID

This plugin implements the Plover HID protocol for stenography, providing an alternative to the existing GeminiPR serial protocol. The Plover HID protocol offers several advantages:

- **Better Bluetooth Support**: Native HID works seamlessly over Bluetooth Low Energy
- **Real-time Key Events**: Sends individual key press/release events instead of complete chords
- **Extended Key Support**: Supports up to 64 keys vs. the limited GeminiPR key set
- **Better Host Integration**: No need for serial port configuration in Plover
- **Future-Proof**: More extensible for features like pressure-sensitive keys

## Protocol Details

The Plover HID protocol uses:
- **Vendor-defined HID Usage Page**: 0xFF50 ("S")
- **Usage**: 0x564C ("TN") 
- Together they spell "STN" for stenography
- **Report ID**: 0x50 (80 decimal)
- **Report Format**: 8 bytes representing 64 key states as a bitmap

## Key Layout

The plugin defines 64 keys following the Plover HID specification:

### Standard Steno Keys (0-22)
```
S_L T_L K_L P_L W_L H_L R_L A O STAR E U F_R R_R P_R B_R L_R G_R T_R S_R D_R Z_R NUM
```

### Extended Keys (23-63)
```
X1 through X41
```

## Using the Plugin

To use the plugin, include the header in your sketch and add PloverHID keys to your keymap:

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-PloverHID.h>

// In your keymap, use PH() macro for Plover HID keys:
PH(S_L), PH(T_L), PH(K_L), PH(P_L), PH(W_L), PH(H_L), PH(R_L),
PH(A), PH(O), PH(STAR), PH(E), PH(U),
PH(F_R), PH(R_R), PH(P_R), PH(B_R), PH(L_R), PH(G_R), PH(T_R), PH(S_R), PH(D_R), PH(Z_R), PH(NUM)

KALEIDOSCOPE_INIT_PLUGINS(PloverHID);

void setup() {
  Kaleidoscope.setup();
}
```

## Plover Configuration

In Plover, select "Plover HID" as your machine type. The plugin will be automatically detected when the keyboard is connected.

## Compatibility

This plugin is compatible with:
- USB connections
- Bluetooth Low Energy connections  
- All Kaleidoscope-supported hardware platforms

## Comparison with GeminiPR

| Feature | PloverHID | GeminiPR |
|---------|-----------|----------|
| Connection | HID (USB/BLE) | Serial (USB CDC) |
| Key Events | Real-time press/release | Chord on release |
| Key Count | 64 keys | ~43 keys |
| Bluetooth | Native support | Requires custom protocol |
| Host Setup | Automatic | Manual serial port config |
| Latency | Lower | Higher |

Both protocols can coexist in the same firmware if needed.
