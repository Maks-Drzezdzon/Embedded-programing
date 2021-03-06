#ifndef __BLE_MAG_SERVICE_H__
#define __BLE_MAG_SERVICE_H__

class MagService {
public:
    //this is being added as the ID of the service
    //declaring register addresses
    const static uint16_t MAG_SERVICE_UUID = 0xA010;
    const static uint16_t MAG_X_CHARACTERISTIC_UUID = 0xA011;
    const static uint16_t MAG_Y_CHARACTERISTIC_UUID = 0xA012;
    const static uint16_t MAG_Z_CHARACTERISTIC_UUID = 0xA013;

    MagService(BLEDevice &_ble, int16_t initialValueForMAGCharacteristic) :
    //constructor for service
        ble(_ble), 
        MagX(MAG_X_CHARACTERISTIC_UUID, &initialValueForMAGCharacteristic),
        MagY(MAG_Y_CHARACTERISTIC_UUID, &initialValueForMAGCharacteristic),
        MagZ(MAG_Z_CHARACTERISTIC_UUID, &initialValueForMAGCharacteristic)
        //creates a ref for each axis
        //binds UUID with each characteristic defined  at the bottom of the service
        //by passing the values to each ref variable
    {
        GattCharacteristic *charTable[] = {&MagX,&MagY,&MagZ};
        
        GattService         MagService(MAG_SERVICE_UUID, charTable, sizeof(charTable) / sizeof(GattCharacteristic *));
        ble.addService(MagService);
    }
    
    //attributes are the smallest data entities defined by gatt
    //handle is used to handle attributes on a gatt server
    GattAttribute::Handle_t getValueHandle() const {
         //returns data stored in MagX
        return MagX.getValueHandle();
    }
    
    
    void updateMagX(uint16_t newValue) {
        //writes data passed through the newValue variable
        //uses the gattserver accessor to write the data over ble
        //this is repeated for each axis
        ble.gattServer().write(MagX.getValueHandle(), (uint8_t *)&newValue, sizeof(uint16_t));
    }
    void updateMagY(uint16_t newValue) {
        ble.gattServer().write(MagY.getValueHandle(), (uint8_t *)&newValue, sizeof(uint16_t));
    }
    void updateMagZ(uint16_t newValue) {
        ble.gattServer().write(MagZ.getValueHandle(), (uint8_t *)&newValue, sizeof(uint16_t));
    }

private:
    //defines the BLEDvice as &ble
    BLEDevice &ble;
    //defines the characteristics of each variable for the Service
    //characteristic is  the name give to grouped pieces of user data
    //these are further orgonised by a hierarchy of sections refered to as services
    //this allows for data to be sent and read 
    ReadOnlyGattCharacteristic<int16_t>  MagX;
    ReadOnlyGattCharacteristic<int16_t>  MagY;
    ReadOnlyGattCharacteristic<int16_t>  MagZ;
};

#endif /* #ifndef __BLE_MAG_SERVICE_H__ */