#ifndef __BLE_ACCEL_SERVICE_H__
#define __BLE_ACCEL_SERVICE_H__
//defines name for service

class ACCELService {
public:
     //this is being added as the ID of the service
    //declaring register addresses
    //UUIDs are 16byte numbers and must be globally unique
    const static uint16_t ACCEL_SERVICE_UUID = 0xA006;
    const static uint16_t ACCEL_X_CHARACTERISTIC_UUID = 0xA007;
    const static uint16_t ACCEL_Y_CHARACTERISTIC_UUID = 0xA008;
    const static uint16_t ACCEL_Z_CHARACTERISTIC_UUID = 0xA009;
    //constructor for service
    ACCELService(BLEDevice &_ble, int16_t initialValueForACCELCharacteristic) :
        ble(_ble)
        //creates a ref for each axis
        //binds UUID with each characteristic defined  at the bottom of the service
        //by passing the values to each ref variable
        ,AccelX(ACCEL_X_CHARACTERISTIC_UUID, &initialValueForACCELCharacteristic)
        ,AccelY(ACCEL_Y_CHARACTERISTIC_UUID, &initialValueForACCELCharacteristic)
        ,AccelZ(ACCEL_Z_CHARACTERISTIC_UUID, &initialValueForACCELCharacteristic)
    {   
        //uses the GattCharacteristic function from GattCharacteristic class
        //creates an array and stores sensor values
        GattCharacteristic *charTable[] = {&AccelX,&AccelY,&AccelZ};
        //a gatt service defines the way that two ble devices transfer data 
        //using services and characteristics
        
        //this takes the service UUID and values from the table array 
        //sizeof is an operator in c that is used with when dealing with int floa or char data types
        //it is used to return the amount of memory allocated to that data type
        GattService         AccelService(ACCEL_SERVICE_UUID, charTable, sizeof(charTable) / sizeof(GattCharacteristic *));
        
        //creates ble service and passes AccelService with its data
        ble.addService(AccelService);
    }
    
    //attributes are the smallest data entities defined by gatt
    //handle is used to handle attributes on a gatt server
    GattAttribute::Handle_t getValueHandle() const {
        //returns data stored in AccelX
        return AccelX.getValueHandle();
    }
    
    void updateAccelX(uint16_t newValue) {
        //writes data passed through the newValue variable
        //uses the gattserver accessor to write the data over ble
        ble.gattServer().write(AccelX.getValueHandle(), (uint8_t *)&newValue, sizeof(uint16_t));
    }
    
    void updateAccelY(uint16_t newValue) {
        //writes data passed through the newValue variable
        //uses the gattserver accessor to write the data over ble
        ble.gattServer().write(AccelY.getValueHandle(), (uint8_t *)&newValue, sizeof(uint16_t));
    }
    //stores variable of type uint16_t in newValue 
    //this variable only exists in this function thats why it can be repeated in the other 3
    void updateAccelZ(uint16_t newValue) {
        ble.gattServer().write(AccelZ.getValueHandle(), (uint8_t *)&newValue, sizeof(uint16_t));
    }

private:
    //defines the BLEDvice as &ble
    BLEDevice &ble;
    //defines the characteristics of each accel axis for the ACCELService
    //characteristic is  the name give to grouped pieces of user data
    //these are further orgonised by a hierarchy of sections refered to as services 
    //this allows for data to only be read
    ReadOnlyGattCharacteristic<int16_t>  AccelX;
    ReadOnlyGattCharacteristic<int16_t>  AccelY;
    ReadOnlyGattCharacteristic<int16_t>  AccelZ;
};

#endif /* #ifndef __BLE_ACCEL_SERVICE_H__ */

