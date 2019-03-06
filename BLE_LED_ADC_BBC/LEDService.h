#ifndef __BLE_LED_SERVICE_H__
#define __BLE_LED_SERVICE_H__

class LEDService {
public:
     //this is being added as the ID of the service
    //declaring register addresses
    //UUIDs are 16byte numbers and must be globally unique
    const static uint16_t LED_SERVICE_UUID              = 0xA000;
    const static uint16_t LED_STATE_CHARACTERISTIC_UUID = 0xA001;

    LEDService(BLEDevice &_ble, bool initialValueForLEDCharacteristic) :
         //creates a ref for each axis
        //binds UUID with each characteristic defined  at the bottom of the service
        //by passing the values to each ref variable
        ble(_ble), ledState(LED_STATE_CHARACTERISTIC_UUID, &initialValueForLEDCharacteristic)
    {   
        //uses the GattCharacteristic function from GattCharacteristic class
        //creates an array and stores sensor values
        GattCharacteristic *charTable[] = {&ledState};
        //a gatt service defines the way that two ble devices transfer data 
        //using services and characteristics
        
        //this takes the service UUID and values from the table array 
        //sizeof is an operator in c that is used with when dealing with int floa or char data types
        //it is used to return the amount of memory allocated to that data type
        GattService         ledService(LED_SERVICE_UUID, charTable, sizeof(charTable) / sizeof(GattCharacteristic *));
        //creates ble service and passes AccelService with its data
        ble.addService(ledService);
    }
    //attributes are the smallest data entities defined by gatt
    //handle is used to handle attributes on a gatt server
    GattAttribute::Handle_t getValueHandle() const {
        //returns data stored in ADCValue
        return ledState.getValueHandle();
    }

private:
    //defines the BLEDvice as &ble
    BLEDevice                         &ble;
    //defines the characteristics of each variable for the Service
    //characteristic is  the name give to grouped pieces of user data
    //these are further orgonised by a hierarchy of sections refered to as services
    //this allows for data to be sent and read 
    //its of type bool because an led can only gave 2 states on or off true or false
    ReadWriteGattCharacteristic<bool>  ledState;
};

#endif /* #ifndef __BLE_LED_SERVICE_H__ */
