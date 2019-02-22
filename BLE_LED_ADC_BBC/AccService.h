// Additional service to extend the basic LED example for the BBC microbit.
#ifndef __BLE_ADC_SERVICE_H__
#define __BLE_ADC_SERVICE_H__

class AccService {
public:
    const static uint16_t ACC_SERVICE_UUID              = 0xA009;
    const static uint16_t ACC_VALUE_CHARACTERISTIC_UUID = 0xA008;

    ACCService(BLEDevice &_ble, uint16_t initialValueForACCCharacteristic) : // change para for acce
        ble(_ble), Accelerometer(ACC_VALUE_CHARACTERISTIC_UUID, &initialValueForADCCharacteristic)
    {
        GattCharacteristic *charTable[] = {&Accelerometer};
        GattService         AccService(ACC_SERVICE_UUID, charTable, sizeof(charTable) / sizeof(GattCharacteristic *));
        ble.addService(AccService);
    }

    GattAttribute::Handle_t getValueHandle() const {
        return Accelerometer.getValueHandle();
    }
    void updateACCValue(uint16_t newValue) {
        ble.gattServer().write(Accelerometer.getValueHandle(), (uint8_t *)&newValue, sizeof(uint16_t));
    }

private:
    BLEDevice                         &ble;
    ReadWriteGattCharacteristic<uint16_t>  Accelerometer;
};

#endif /* #ifndef __BLE_ADC_SERVICE_H__ */
