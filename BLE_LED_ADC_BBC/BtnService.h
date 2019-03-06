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

#ifndef __BLE_BTN_SERVICE_H__
#define __BLE_BTN_SERVICE_H__

class BtnService {
public:
     //this is being added as the ID of the service
    //declaring register addresses
    const static uint16_t BTN_SERVICE_UUID              = 0xA002;
    const static uint16_t BTN_STATE_CHARACTERISTIC_UUID = 0xA003;

    BtnService(BLEDevice &_ble, uint16_t initialValueForBtnCharacteristic) :
        ble(_ble), btnState(BTN_STATE_CHARACTERISTIC_UUID, &initialValueForBtnCharacteristic)
    {
        GattCharacteristic *charTable[] = {&btnState};
        GattService         btnService(BTN_SERVICE_UUID, charTable, sizeof(charTable) / sizeof(GattCharacteristic *));
        ble.addService(btnService);
    }

    GattAttribute::Handle_t getValueHandle() const {
        return btnState.getValueHandle();
    }
    void updateBtn(uint16_t newValue) {
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