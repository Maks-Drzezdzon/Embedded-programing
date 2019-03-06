/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __BLE_TEMP_SERVICE_H__
#define __BLE_TEMP_SERVICE_H__

class TempService {
public:   
    //this is being added as the ID of the service
    //declaring register addresses
    //UUIDs are 16byte numbers and must be globally unique
    const static uint16_t TEMP_SERVICE_UUID              = 0xA014;// new values
    const static uint16_t TEMP_VALUE_CHARACTERISTIC_UUID = 0xA015;//new values

    TempService(BLEDevice &_ble) :
        //creates a ref for each axis
        //binds UUID with each characteristic defined  at the bottom of the service
        //by passing the values to each ref variable
        //the initial value for temp is set to 0
        ble(_ble), Temperature(TEMP_VALUE_CHARACTERISTIC_UUID, 0)
    {   
        //uses the GattCharacteristic function from GattCharacteristic class
        //creates an array and stores sensor values
        GattCharacteristic *charTable[] = {&Temperature};
        //a gatt service defines the way that two ble devices transfer data 
        //using services and characteristics
        
        //this takes the service UUID and values from the table array 
        //sizeof is an operator in c that is used with when dealing with int floa or char data types
        //it is used to return the amount of memory allocated to that data type
        GattService         TempService(TEMP_SERVICE_UUID, charTable, sizeof(charTable) / sizeof(GattCharacteristic *));
        //creates ble service and passes AccelService with its data
        ble.addService(TempService);
    }
    
    //attributes are the smallest data entities defined by gatt
    //handle is used to handle attributes on a gatt server
    GattAttribute::Handle_t getValueHandle() const {
        //returns data stored in ADCValue
        return Temperature.getValueHandle();
    }
    void updateValue(uint16_t newValue) {
        //writes data passed through the newValue variable
        //uses the gattserver accessor to write the data over ble
        ble.gattServer().write(this->getValueHandle(), (uint8_t *)&newValue, sizeof(uint16_t));
    }
private:
    //defines the BLEDvice as &ble
    BLEDevice                         &ble;
    //defines the characteristics of each variable for the Service
    //characteristic is  the name give to grouped pieces of user data
    //these are further orgonised by a hierarchy of sections refered to as services
    //this allows for data to be sent and read 
    ReadWriteGattCharacteristic<int>  Temperature;
};

#endif /* #ifndef __BLE_LED_SERVICE_H__ */
