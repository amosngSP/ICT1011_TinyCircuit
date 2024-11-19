#include <ArduinoJson.h>
#include <Wire.h>
#include <SPI.h>
#include <STBLE.h>
#include <TinyScreen.h>
#include "ICT1011_TinyCircuits.h"
//Debug output adds extra flash and memory requirements!
#ifndef BLE_DEBUG
#define BLE_DEBUG true
#endif

#ifdef BLE_DEBUG
#define SerialMonitorInterface SerialUSB
#endif

//Serial



//uint8_t ble_rx_buffer[1024];  // Define the buffer

#define PIPE_UART_OVER_BTLE_UART_TX_TX 0


uint8_t ble_rx_buffer[1024];  // Declare, not define
uint8_t ble_rx_buffer_len = 0;

//screen
TinyScreen display = TinyScreen(TinyScreenDefault);

//buttons
uint8_t buttonReleased = 1;

