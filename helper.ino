//as defined in the file
enum COLOUR {
  BLACK = 0x00,
  GRAY = 0x6D,
  WHITE = 0xFF,
  BLUE = 0xE0,
  DARK_BLUE = 0x60,
  RED = 0x03,
  DARK_RED = 0x01,
  GREEN = 0x1C,
  DARK_GREEN = 0x0C,
  BROWN = 0x32,
  DARK_BROWN = 0x22,
  YELLOW = 0x1F
};

//easier to use these
enum COLOUR black = BLACK;
enum COLOUR gray = GRAY;
enum COLOUR white = WHITE;
enum COLOUR red = RED;
enum COLOUR green = GREEN;

void displaySetup(){
    Wire.begin();//initialize I2C before we can initialize TinyScreen- not needed for TinyScreen+
  display.begin();
  //setBrightness(brightness);//sets main current level, valid levels are 0-15
  display.setBrightness(10);
  display.setFont(thinPixel7_10ptFontInfo);
}

void RTCSetup(){
  rtc.begin();
  rtc.setTime(hours, minutes, seconds);
  rtc.setDate(day, month, year);
}
//without colour
void displayText(char* text,int height)
{
  int width=display.getPrintWidth(text);
  display.setCursor(48-(width/2),height);
  display.fontColor(white,black);
  display.print(text);
  PRINTF(text);
  PRINTF("\n");
}
//with colour
void displayText(char* text,int height, int color)
{
  int width=display.getPrintWidth(text);
  display.setCursor(48-(width/2),height);
  display.fontColor(color,black);
  display.print(text);
  PRINTF(text);
  PRINTF("\n");
}
void displayTextTopLeft(char* text,int height)
{
  display.setCursor(0,height);
  display.fontColor(white,black);
  display.print(text);
}
void clearDisplay()
{
  display.clearScreen();
}

void countdownTimer(int s,int height){
  int min = (s >= 60) ? floor(s/60) : 0; //if secs more than or equal to 60, to do math and grab the minutes left
  int secs = (s >= 60) ? s-(floor(s/60)*60) : s; //if secs more than or equal to 60, to do math and grab the minutes left.
  char timer[10];
  sprintf(timer,"%02d:%02d", min,secs);
  display.clearScreen();
  displayText("Time Left",height);
  displayText(timer,height+10);
}
char total_data[1024];
char* rcvData()
{
  strcpy(total_data,"");
  int receiving = 0;
  
  if (ble_rx_buffer_len){
    char* received = (char*)ble_rx_buffer;
    ble_rx_buffer_len = 0; //reset buffer
    if(strcmp(received,"START")==0)
    {
      PRINTF("START signal received, waiting for data.\n");
      //received start signal, begin while loop to keep receiving buffer.
      while (1)
      {
        //a delay to prevent this code running all the time
        //delay(1000);
        
        if (ble_rx_buffer_len){
          received = (char*)ble_rx_buffer;
          //data received
          if(strcmp(received,"END")==0){
            PRINTF("END SIGNAL RECEIVED.\n");
            PRINTF("FULL DATA:");
            PRINTF(total_data);
            PRINTF("\n");
            return total_data;
          }
          else {
            PRINTF("RECEIVED:");
            PRINTF(received);
            PRINTF("\n");
            strcat(total_data,received);
          }
        }
        ble_rx_buffer_len = 0; //reset buffer
      }
    } else {
      return received;
    }
  }
  //serial input
  #ifdef BLE_DEBUG
  if(SerialMonitorInterface.available())
  {
    delay(10);
    while (SerialMonitorInterface.available()){
        strcat(total_data,(char*) SerialMonitorInterface.readString().c_str());
    }
    return total_data;
  }
  #endif
}

int sendData(char* str){
  int length = strlen(str);
  int success = lib_aci_send_data(PIPE_UART_OVER_BTLE_UART_TX_TX, (uint8_t*)str, length);

  
  if(success)
  {
    #ifdef BLE_DEBUG
    PRINTF("Data sent!\n")
    PRINTF(str);
    PRINTF("\n")
    #endif
    return 1;
  }
  else
  {
    #ifdef BLE_DEBUG
    PRINTF("Data not sent!\n");
    PRINTF(str);
    PRINTF("\n")
    #endif
    return 0;
  }
  
}

