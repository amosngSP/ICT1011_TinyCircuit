#include <ArduinoJson.h>
#include <Wire.h>
#include <SPI.h>
#include <STBLE.h>
#include <TinyScreen.h>

//Debug output adds extra flash and memory requirements!
#ifndef BLE_DEBUG
#define BLE_DEBUG true
#endif

//Serial

#define SerialMonitorInterface SerialUSB


enum status{
  NONE,
  START,
  END
};



uint8_t ble_rx_buffer[1024];
uint8_t ble_rx_buffer_len = 0;
uint8_t ble_connection_state = false;
#define PIPE_UART_OVER_BTLE_UART_TX_TX 0

//screen
TinyScreen display = TinyScreen(TinyScreenDefault);
int secs = 70;
void setup() {
  //Serial Monitor
  SerialMonitorInterface.begin(9600);
  while (!SerialMonitorInterface); //This line will block until a serial monitor is opened with TinyScreen+!
  // put your setup code here, to run once:
  randomSeed(analogRead(0));
  BLEsetup();
  displaySetup();
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  aci_loop();
  homeScreen();
  if(ble_rx_buffer_len){
    //check if data is available
    JsonDocument doc;
    //char* json_data = "{\"action\":\"start\",\"playertype\":\"hider\"}";
    char* json_data = ((char*)ble_rx_buffer);
    SerialMonitorInterface.println(json_data);
    //deserialize the JSON document
    DeserializationError error = deserializeJson(doc, json_data);
    if(error)
    {
      //error handling code if the json serializing fails
      return;
    }
    enum status game_status = gameStatus(doc["action"]);
    SerialMonitorInterface.println(doc["action"].as<String>());
    ble_rx_buffer_len = 0;//clear afer reading
  }
  // countdownTimer(secs);
  // secs--;
   delay(1000);
}