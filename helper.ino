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
//without colour
void displayText(char* text,int height)
{
  int width=display.getPrintWidth(text);
  display.setCursor(48-(width/2),height);
  display.fontColor(white,black);
  display.print(text);
}
//with colour
void displayText(char* text,int height, int color)
{
  int width=display.getPrintWidth(text);
  display.setCursor(48-(width/2),height);
  display.fontColor(color,black);
  display.print(text);
}

void homeScreen(){
  display.clearScreen();
  displayText("HIDE",10);
  displayText("AND",20);
  displayText("SEEK",30);
  displayText("=========",40);
  if(connected)
  {
    displayText("Connected",50,green);
  }
  else
  {
    displayText("Not connected",50,red);
  }
  delay(1000);
}

void countdownTimer(int s){
  int min = (s >= 60) ? floor(s/60) : 0; //if secs more than or equal to 60, to do math and grab the minutes left
  int secs = (s >= 60) ? s-(floor(s/60)*60) : s; //if secs more than or equal to 60, to do math and grab the minutes left.
  char timer[10];
  sprintf(timer,"%02d:%02d", min,secs);
  display.clearScreen();
  displayText("Time Left",10);
  displayText(timer,20);
}

char * rcv_data(){
  if(ble_rx_buffer_len){
    //receive data
    char* data = ((char*)ble_rx_buffer);
    //flush buffer
    ble_rx_buffer_len = 0;
    //return data
    return data;
  } else {
    return "";
  }
}

status gameStatus(const char* s){
  if(s=="start")
  {
    return (status)START;
  } 
  else if (s=="end")
  {
    return (status)END;
  }
  else if (s=="interrupt")
  {
    return (status)INTERRUPT;
  }
}