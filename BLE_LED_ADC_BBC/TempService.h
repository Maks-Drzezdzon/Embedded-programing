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
public:                     //this is being added as the ID of the service
    const static uint16_t TEMP_SERVICE_UUID              = 0xA014;// new values
    const static uint16_t TEMP_VALUE_CHARACTERISTIC_UUID = 0xA015;//new values

    TempService(BLEDevice &_ble) :
        ble(_ble), Temperature(TEMP_VALUE_CHARACTERISTIC_UUID, 0)
    {
        GattCharacteristic *charTable[] = {&Temperature};
        GattService         TempService(TEMP_SERVICE_UUID, charTable, sizeof(charTable) / sizeof(GattCharacteristic *));
        ble.addService(TempService);
    }

    GattAttribute::Handle_t getValueHandle() const {
        return Temperature.getValueHandle();
    }
    void updateValue(uint16_t newValue) {
        ble.gattServer().write(this->getValueHandle(), (uint8_t *)&newValue, sizeof(uint16_t));
    }
private:
    BLEDevice                         &ble;
    ReadWriteGattCharacteristic<int>  Temperature;
};

#endif /* #ifndef __BLE_LED_SERVICE_H__ */
