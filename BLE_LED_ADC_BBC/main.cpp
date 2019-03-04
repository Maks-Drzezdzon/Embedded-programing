#include "mbed.h"
#include "ble/BLE.h"
#include "LEDService.h"
#include "AccService.h"
#include "BtnService.h"
#include "TempService.h"
#include "MagService.h"

DigitalOut col1(P0_4, 0);
DigitalOut alivenessLED(P0_13, 0);
DigitalOut actuatedLED(P0_14, 0);
DigitalOut BtnA(P0_17);
DigitalOut LED(P0_3);

InterruptIn BtnAIRQ(P0_17);
InterruptIn BtnBIRQ(P0_26);

const static char     DEVICE_NAME[] = "maks"; // name device
//add and bind services with UUID 
static const uint16_t uuid16_list[] = {LEDService::LED_SERVICE_UUID,ACCELService::ACCEL_SERVICE_UUID, MagService::MAG_SERVICE_UUID ,TempService::TEMP_SERVICE_UUID ,BtnService::BTN_SERVICE_UUID};

LEDService *ledServicePtr;
ACCELService *AccelServicePtr;
TempService *TempServicePtr; // new pointer for new service later used to create new obj
BtnService *BtnServicePtr;
MagService *MagServicePtr;
Ticker ticker;

//variable to hold the button flag
uint8_t btnFlag = 0;

// Accelerometer : MMA8653FC.  I2C address
I2C i2c(P0_30, P0_0); //MCU values ,SDA is on P0_30, SCL is on P0_0
const int MMA8653_ADDRESS = (0x1d<<1); //accel values
const int MMA8653_ID = 0x5a; //accel values

const int MAG3110_ADDRESS = (0x0E<<1); //mag values
const int MAG3110 = 0xc4; //mag values

void updateAccelerations(void);
void updateMagnetometer(void);  
void checkBtn(void); 

void disconnectionCallback(const Gap::DisconnectionCallbackParams_t *params)
{
    BLE::Instance().gap().startAdvertising();
}

void periodicCallback(void)
{
    alivenessLED = !alivenessLED; /* Do blinky on LED1 to indicate system aliveness. */
    updateAccelerations();
    updateMagnetometer();
    checkBtn();
}

/**
 * This callback allows the LEDService to receive updates to the ledState Characteristic.
 *
 * @param[in] params
 *     Information about the characterisitc being updated.
 */
void onDataWrittenCallback(const GattWriteCallbackParams *params) {
    if ((params->handle == ledServicePtr->getValueHandle()) && (params->len == 1)) {
        actuatedLED = *(params->data);
    }
}

void onDataReadCallback(const GattReadCallbackParams *params) {
   
}
/**
 * This function is called when the ble initialization process has failed
 */
void onBleInitError(BLE &ble, ble_error_t error)
{
    /* Initialization error handling should go here */
}

/**
 * Callback triggered when the ble initialization process has finished
 */
void bleInitComplete(BLE::InitializationCompleteCallbackContext *params)
{
    BLE&        ble   = params->ble;
    ble_error_t error = params->error;

    if (error != BLE_ERROR_NONE) {
        /* In case of error, forward the error handling to onBleInitError */
        onBleInitError(ble, error);
        return;
    }

    /* Ensure that it is the default instance of BLE */
    if(ble.getInstanceID() != BLE::DEFAULT_INSTANCE) {
        return;
    }
 
    ble.gap().onDisconnection(disconnectionCallback);
    ble.gattServer().onDataWritten(onDataWrittenCallback);
    ble.gattServer().onDataRead(onDataReadCallback);

    bool initialValueForLEDCharacteristic = false;
    ledServicePtr = new LEDService(ble, initialValueForLEDCharacteristic);
    AccelServicePtr = new ACCELService(ble,0);
    MagServicePtr = new MagService(ble,0);
    TempServicePtr = new TempService(ble); // creating obj of new service and passing ble 
    BtnServicePtr = new BtnService(ble,0);
    
    /* setup advertising */
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::BREDR_NOT_SUPPORTED | GapAdvertisingData::LE_GENERAL_DISCOVERABLE);
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_16BIT_SERVICE_IDS, (uint8_t *)uuid16_list, sizeof(uuid16_list));
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LOCAL_NAME, (uint8_t *)DEVICE_NAME, sizeof(DEVICE_NAME));
    ble.gap().setAdvertisingType(GapAdvertisingParams::ADV_CONNECTABLE_UNDIRECTED);
    ble.gap().setAdvertisingInterval(1000); /* 1000ms. */
    ble.gap().startAdvertising();
}

void checkBtn()
{
    BtnServicePtr->updateBtn(btnFlag);
}

void updateAccelerations()
{
    char Data[8]; // Declare a buffer for data transfer    
    int Status;
        int16_t X;
        Data[0]=0x01; // Register number 1 has the X data (2 bytes)
        Status = i2c.write(MMA8653_ADDRESS,Data,1,true);  // Write register number
        Status = i2c.read(MMA8653_ADDRESS,Data,2); // Read register contents
        X = Data[0];
        X = (X << 8) + Data[1];
        X = X >> 6; // only 10 bits of data are available
        
        int16_t Y;
        Data[0]=0x03; // Register number 3 has the Y data (2 bytes)
        Status = i2c.write(MMA8653_ADDRESS,Data,1,true);  // Write register number
        Status = i2c.read(MMA8653_ADDRESS,Data,2); // Read register contents
        Y = Data[0];
        Y = (Y << 8) + Data[1];
        Y = Y >> 6; // only 10 bits of data are available        
        
        int16_t Z;
        Data[0]=0x05; // Register number 1 has the Z data (2 bytes)
        Status = i2c.write(MMA8653_ADDRESS,Data,1,true);  // Write register number
        Status = i2c.read(MMA8653_ADDRESS,Data,2); // Read register contents
        Z = Data[0];
        Z = (Z << 8) + Data[1];
        Z = Z >> 6; // only 10 bits of data are available
        
        AccelServicePtr->updateAccelX(X);
        AccelServicePtr->updateAccelY(Y);
        AccelServicePtr->updateAccelZ(Z);
}

void updateMagnetometer()
{
    int Status;
    char Data[8]; // Declare a buffer for data transfer    
    int16_t XMSB, XLSB, X;
    Data[0]=0x01; // Register number 1 has the X data (2 bytes)
    Status = i2c.write(MAG3110_ADDRESS,Data,1,true);  // Write register number
    Status = i2c.read(MAG3110_ADDRESS,Data,2); // Read register contents
    XMSB = Data[0];
    XMSB = (XMSB << 8) + Data[1];
    
    Data[0]=0x02; // Register number 1 has the X data (2 bytes)
    Status = i2c.write(MAG3110_ADDRESS,Data,1,true);  // Write register number
    Status = i2c.read(MAG3110_ADDRESS,Data,2); // Read register contents
    XLSB = Data[0];
    
    X = XMSB + XLSB;
    //X = X >> 6; // only 10 bits of data are available       
    wait(0.2);
    
    int16_t YMSB, YLSB, Y;
    Data[0]=0x03; // Register number 3 has the Y data (2 bytes)
    Status = i2c.write(MAG3110_ADDRESS,Data,1,true);  // Write register number
    Status = i2c.read(MAG3110_ADDRESS,Data,2); // Read register contents
    YMSB = Data[0];
    YMSB = (YMSB << 8) + Data[1];
    
    Data[0]=0x04; // Register number 3 has the Y data (2 bytes)
    Status = i2c.write(MAG3110_ADDRESS,Data,1,true);  // Write register number
    Status = i2c.read(MAG3110_ADDRESS,Data,2); // Read register contents
    YLSB = Data[0];
    
    Y = YMSB + YLSB;           
    
    int16_t ZMSB, ZLSB, Z;
    Data[0]=0x05; // Register number 5 has the Z data (2 bytes)
    Status = i2c.write(MAG3110_ADDRESS,Data,1,true);  // Write register number
    Status = i2c.read(MAG3110_ADDRESS,Data,2); // Read register contents
    ZMSB = Data[0];
    ZMSB = (ZMSB << 8) + Data[1];
    
    Data[0]=0x06; // Register number 5 has the Z data (2 bytes)
    Status = i2c.write(MAG3110_ADDRESS,Data,1,true);  // Write register number
    Status = i2c.read(MAG3110_ADDRESS,Data,2); // Read register contents
    ZLSB = Data[0];

    Z = ZMSB + ZLSB;      
    
    MagServicePtr->updateMagX(X);
    MagServicePtr->updateMagY(Y);
    MagServicePtr->updateMagZ(Z); 
}



int Tempv = 0;

#define TEMP_BASE 0x4000C000
#define TEMP_START (*(volatile uint32_t *)(TEMP_BASE + 0))
#define TEMP_STOP (*(volatile uint32_t *)(TEMP_BASE + 4))
#define TEMP_DATARDY (*(volatile uint32_t *)(TEMP_BASE + 0x100))
#define TEMP_READING (*(volatile uint32_t *)(TEMP_BASE + 0x508))
int readTemperature()
{
    TEMP_START = 1; // enable the temperature sensor
    while (TEMP_DATARDY == 0);
    return TEMP_READING/4;
}

void btnA_handler()
{
    //Button A has been pressed, send the activation code
    btnFlag = 111;
}

void btnB_handler()
{
    //Button B has been pressed, send the deactivation code
    btnFlag = 222;
}

int main(void)
{    
    char Data[8]; // Declare a buffer for data transfer    
    int Status;
    ticker.attach(periodicCallback, 1); /* Blink LED every second */

    BLE &ble = BLE::Instance();
    ble.init(bleInitComplete);
    
    BtnAIRQ.rise(&btnA_handler);
    BtnBIRQ.rise(&btnB_handler);
    /* SpinWait for initialization to complete. This is necessary because the
     * BLE object is used in the main loop below. */
    while (ble.hasInitialized()  == false) { /* spin loop */ }

    // Wake the accelerometer from sleep mode by writing 1 to register number 0x2a    
    Data[0]=0x2a; 
    Data[1]=1;
    Status = i2c.write(MMA8653_ADDRESS,Data,2);  // Write data to register 
    
     // Wake the accelerometer from sleep mode by writing 1 to register number 0x2a    
    Data[0]=0x10; 
    Data[1]=1;
    Status = i2c.write(MAG3110_ADDRESS,Data,2);  // Write data to register   
    
    while (true) {        
        ble.waitForEvent();
        TempServicePtr->updateValue(readTemperature());  
    }
}
