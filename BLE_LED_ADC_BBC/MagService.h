// Additional service to extend the basic LED example for the BBC microbit.
#ifndef __BLE_ADC_SERVICE_H__
#define __BLE_ADC_SERVICE_H__

class MagService {
public:
    //assigning pins to axis IDs
    const static uint16_t MAG_SERVICE_UUID          = 0xA009;
    const static uint16_t MAG_X_CHARACTERISTIC_UUID = 0xA013;
    const static uint16_t MAG_Y_CHARACTERISTIC_UUID = 0xA014;
    const static uint16_t MAG_Z_CHARACTERISTIC_UUID = 0xA015;
    
    MAGService(BLEDevice &_ble, uint16_t initialValueForMAGCharacteristic) : // change para for acce
        ble(_ble)
        ,MagX(MAG_X_CHARACTERISTIC_UUID, &initialValueForACCCharacteristic) 
        ,MagY(MAG_Y_CHARACTERISTIC_UUID, &initialValueForACCCharacteristic)
        ,MagZ(MAG_Z_CHARACTERISTIC_UUID, &initialValueForACCCharacteristic)
    {
        GattCharacteristic *charTable[] = {&MagX, &MagY ,&MagZ}; //adding axis variables to chartable array
        GattService         MagService(MAG_SERVICE_UUID, charTable, sizeof(charTable) / sizeof(GattCharacteristic *));
        //adding service to bluetooth
        ble.addService(MagService);
    }

    GattAttribute::Handle_t getValueHandle() const {
        return AccX.getValueHandle();
    }
    void updateMagX(uint16_t newValue) {
        ble.gattServer().write(MagX.getValueHandle(), (uint8_t *)&newValue, sizeof(uint16_t));
    }
    
    void updateMagY(uint16_t newValue) {
        ble.gattServer().write(MagY.getValueHandle(), (uint8_t *)&newValue, sizeof(uint16_t));
    }
    
    void updateMagZ(uint16_t newValue) {
        ble.gattServer().write(MagZ.getValueHandle(), (uint8_t *)&newValue, sizeof(uint16_t));
    }

private:
    BLEDevice                         &ble;
    //read only data
    ReadGattCharacteristic<uint16_t>  MagX;
    ReadGattCharacteristic<uint16_t>  MagY;
    ReadGattCharacteristic<uint16_t>  MagZ;

    
};

#endif /* #ifndef __BLE_MAG_SERVICE_H__ */
