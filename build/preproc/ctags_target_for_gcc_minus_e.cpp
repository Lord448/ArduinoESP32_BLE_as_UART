# 1 "/home/lord448/Escritorio/Repos/OwnLibs/ArduinoESP32_BLEasUART/ArduinoESP32_BLEasUART.ino"
/**

 * @author Pedro Rojo (pedroeroca@outlook.com); Lord448 @ github.com

 * @brief 

 * @version 0.1

 * @date 2023-06-08

 * 

 * @copyright Copyright (c) 2023

 */
# 10 "/home/lord448/Escritorio/Repos/OwnLibs/ArduinoESP32_BLEasUART/ArduinoESP32_BLEasUART.ino"
# 11 "/home/lord448/Escritorio/Repos/OwnLibs/ArduinoESP32_BLEasUART/ArduinoESP32_BLEasUART.ino" 2
# 12 "/home/lord448/Escritorio/Repos/OwnLibs/ArduinoESP32_BLEasUART/ArduinoESP32_BLEasUART.ino" 2
# 13 "/home/lord448/Escritorio/Repos/OwnLibs/ArduinoESP32_BLEasUART/ArduinoESP32_BLEasUART.ino" 2
# 14 "/home/lord448/Escritorio/Repos/OwnLibs/ArduinoESP32_BLEasUART/ArduinoESP32_BLEasUART.ino" 2
# 15 "/home/lord448/Escritorio/Repos/OwnLibs/ArduinoESP32_BLEasUART/ArduinoESP32_BLEasUART.ino" 2




# 18 "/home/lord448/Escritorio/Repos/OwnLibs/ArduinoESP32_BLEasUART/ArduinoESP32_BLEasUART.ino"
// See the following for generating UUIDs:
// https://www.uuidgenerator.net/




BLEServer *pServer = 
# 24 "/home/lord448/Escritorio/Repos/OwnLibs/ArduinoESP32_BLEasUART/ArduinoESP32_BLEasUART.ino" 3 4
                    __null
# 24 "/home/lord448/Escritorio/Repos/OwnLibs/ArduinoESP32_BLEasUART/ArduinoESP32_BLEasUART.ino"
                        ;
BLECharacteristic *pTxCharacteristic;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint8_t txValue = 0;
char Buffer[10];

void sendData(char *buffer, BLECharacteristic *pTXCharacteristic);

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string rxValue = pCharacteristic->getValue();

      if (rxValue.length() > 0) {
        Serial.print("Received Value: ");
        for (int i = 0; i < rxValue.length(); i++)
          Serial.print(rxValue[i]);
      }
    }
};


void setup() {
  Serial.begin(115200);

  // Create the BLE Device
  BLEDevice::init("UART Service");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService("670b1f26-0a44-11ee-be56-0242ac120002" /* UART service UUID*/);

  // Create a BLE Characteristic
  pTxCharacteristic = pService->createCharacteristic(
          "058804de-0a45-11ee-be56-0242ac120002",
          BLECharacteristic::PROPERTY_NOTIFY
         );

  pTxCharacteristic->addDescriptor(new BLE2902());

  BLECharacteristic *pRxCharacteristic = pService->createCharacteristic(
            "006e861c-0a45-11ee-be56-0242ac120002",
           BLECharacteristic::PROPERTY_WRITE
          );

  pRxCharacteristic->setCallbacks(new MyCallbacks());

  // Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");
}

void loop() {

    if (deviceConnected) {
        sprintf(Buffer, "%d", txValue);
        sendData(Buffer, pTxCharacteristic);
        txValue++;
 }

    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
  // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
}

/**

 * @brief Send data to the desired BLE characteristic in UTF-8

 * 

 * @param buffer String data to send 

 * @param pTXCharacteristic Characteristic that will be modified

 */
# 120 "/home/lord448/Escritorio/Repos/OwnLibs/ArduinoESP32_BLEasUART/ArduinoESP32_BLEasUART.ino"
void sendData(char *buffer, BLECharacteristic *pTXCharacteristic) {
    char charTX;
    for(uint32_t i = 0; i <= strlen(buffer); i++) {
        if(i != strlen(buffer))
            charTX = buffer[i];
        else
            charTX = '\n';
        pTXCharacteristic -> setValue((uint8_t *)&charTX, sizeof(uint8_t));
        pTXCharacteristic -> notify();
        delay(15); // bluetooth stack will go into congestion, if too many packets are sent, 10ms min
    }
}
