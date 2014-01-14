#include <EEPROM.h>


void save_current_layer(byte layer)
{
    EEPROM.write(EEPROM_LAYER_LOCATION, layer);
}

byte load_current_layer()
{
    byte layer =  EEPROM.read(EEPROM_LAYER_LOCATION);
    if (layer >= LAYERS ) {
        return 0; // undefined positions get saved as 255
    }
    return layer;
}



