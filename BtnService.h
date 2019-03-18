#ifndef __BLE_BTN_SERVICE_H__
#define __BLE_BTN_SERVICE_H__
//defines name for service
class BtnService {
public:
     //this is being added as the ID of the service
    //declaring register addresses
    //UUIDs are 16byte numbers and must be globally unique
    const static uint16_t BTN_SERVICE_UUID              = 0xA002;
    const static uint16_t BTN_STATE_CHARACTERISTIC_UUID = 0xA003;

    BtnService(BLEDevice &_ble, uint16_t initialValueForBtnCharacteristic) :
        //binds UUID with each characteristic defined  at the bottom of the service
        //by passing the values to each ref variable
        //the initial value for temp is set to 0
        //constructor for service
        ble(_ble), btnState(BTN_STATE_CHARACTERISTIC_UUID, &initialValueForBtnCharacteristic)
    {   
        //uses the GattCharacteristic function from GattCharacteristic class
        //creates an array and stores sensor values
        GattCharacteristic *charTable[] = {&btnState};
        //a gatt service defines the way that two ble devices transfer data 
        //using services and characteristics
        
        //this takes the service UUID and values from the table array 
        //sizeof is an operator in c that is used with when dealing with int floa or char data types
        //it is used to return the amount of memory allocated to that data type
        GattService         btnService(BTN_SERVICE_UUID, charTable, sizeof(charTable) / sizeof(GattCharacteristic *));
        //creates ble service and passes btnService with its data
        ble.addService(btnService);
    }
     //attributes are the smallest data entities defined by gatt
    //handle is used to handle attributes on a gatt server
    GattAttribute::Handle_t getValueHandle() const {
        //returns data stored in Temperature
        return btnState.getValueHandle();
    }
    void updateBtn(uint16_t newValue) {
        //writes data passed through the newValue variable
        //uses the gattserver accessor to write the data over ble
        ble.gattServer().write(btnState.getValueHandle(), (uint8_t *)&newValue, sizeof(uint16_t));
    }

private:
    //defines the BLEDvice as &ble
    BLEDevice                              &ble;
    //defines the characteristics of each variable for the Service
    //characteristic is  the name give to grouped pieces of user data
    //these are further orgonised by a hierarchy of sections refered to as services 
    //this allows for data to be sent and read
    ReadWriteGattCharacteristic<uint16_t>  btnState;
};

#endif /* #ifndef __BLE_BTN_SERVICE_H__ */