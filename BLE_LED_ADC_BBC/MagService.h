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

#ifndef __BLE_MAG_SERVICE_H__
#define __BLE_MAG_SERVICE_H__

class MagService {
public:
    const static uint16_t MAG_SERVICE_UUID = 0xA010;
    const static uint16_t MAG_X_CHARACTERISTIC_UUID = 0xA011;
    const static uint16_t MAG_Y_CHARACTERISTIC_UUID = 0xA012;
    const static uint16_t MAG_Z_CHARACTERISTIC_UUID = 0xA013;

    MagService(BLEDevice &_ble, int16_t initialValueForMAGCharacteristic) :
        ble(_ble), MagX(MAG_X_CHARACTERISTIC_UUID, &initialValueForMAGCharacteristic),MagY(MAG_Y_CHARACTERISTIC_UUID, &initialValueForMAGCharacteristic),MagZ(MAG_Z_CHARACTERISTIC_UUID, &initialValueForMAGCharacteristic)
    {
        GattCharacteristic *charTable[] = {&MagX,&MagY,&MagZ};
        GattService         MagService(MAG_SERVICE_UUID, charTable, sizeof(charTable) / sizeof(GattCharacteristic *));
        ble.addService(MagService);
    }

    GattAttribute::Handle_t getValueHandle() const {
        return MagX.getValueHandle();
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
    BLEDevice &ble;
    ReadOnlyGattCharacteristic<int16_t>  MagX;
    ReadOnlyGattCharacteristic<int16_t>  MagY;
    ReadOnlyGattCharacteristic<int16_t>  MagZ;
};

#endif /* #ifndef __BLE_MAG_SERVICE_H__ */