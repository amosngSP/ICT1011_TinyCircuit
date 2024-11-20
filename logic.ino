
//initial global declaration for use later
enum status game_status = NONE;
enum playerType player_type = NEITHER;
enum powerUps power_ups = NIL;
enum screenStatus screen_status = S_EMPTY;
enum playerType winner = NEITHER;

//buttons
const uint8_t upRightButton = TSButtonUpperRight;
const uint8_t upLeftButton = TSButtonUpperLeft;
const uint8_t downRightButton = TSButtonLowerRight;
const uint8_t downLeftButton = TSButtonLowerLeft;

//global variables we'll be using
int hiding_time = 0;
int seeker_time = 0;
int hiding_players = 0;

void maingame();
void header();
void pregame();
void homeScreen();
enum status gameStatus();

void maingame(){
  //no status set
  if(game_status==NONE)
  {
    //game not started
    //show home screen
    homeScreen();
  }
  else if(game_status==PREGAME)
  {
    pregame();
    //when the status is PREGAME
  }
  else if(game_status==HIDING&&player_type==HIDER)
  {
    hider_hiding();
    //when the status is HIDING, player is HIDER
  }
  else if(game_status==HIDING&&player_type==SEEKER)
  {
    seeker_hiding();
    //when the status is HIDING, player is SEEKER
  }
  else if(game_status==SEEKING&&player_type==HIDER)
  {
    hider_seeking();
    //when the status is SEEKING, player is HIDER, and player is not eliminated
  }
  else if(game_status==SEEKING&&player_type==SEEKER)
  {
    seeker_seeking();
    //when the status is SEEKING, player is SEEKER
  }
  else if(game_status==SEEKING&&player_type==ELIMINATED)
  {
    //player eliminated
    hider_eliminated();
  }
  else if (game_status==END)
  {
    //when the status is END
    game_end();
  }
#ifdef BLE_DEBUG
  if(ble_rx_buffer_len||SerialMonitorInterface.available()){
#else
  if(ble_rx_buffer_len){
#endif
  
    //check if data is available
    JsonDocument doc;
    //testing
    //char* json_data = "{\"game_action\":\"start\",\"hiding_time\":10,\"seeker_time\":10,\"hiding_players\":5,\"player_type\":\"hider\"}";

    //actual data
    char* json_data = rcvData();
    //SerialMonitorInterface.println(json_data);
    //deserialize the JSON document
    DeserializationError error = deserializeJson(doc, json_data);
    if(error)
    {
      //error handling code if the json serializing fails
      doc["error"] = "fail";
      PRINTF("Fail!\n");
      PRINTF(error.c_str());
      PRINTF("\n");
      PRINTF(json_data);
      PRINTF("\n");
      ble_rx_buffer_len = 0;//clear after reading
      return;
    }
    PRINTF("Success!");
    // enum status game_status = gameStatus(doc["action"]);
    // SerialMonitorInterface.println(doc["action"].as<String>());
    ble_rx_buffer_len = 0;//clear after reading

  if (doc["game_action"]!=NULL)
  {
    if(doc["game_action"]=="start")
    {
      //if start signal received, set to pregame stage.
      game_status = PREGAME;
      //set default to 120secs (2mins) if not set
      hiding_time = doc["hiding_time"] !=NULL ? doc["hiding_time"] : 120;
      //set default to 300secs (5mins) if not set
      seeker_time = doc["seeker_time"] !=NULL ? doc["seeker_time"] : 300;
      //get the number of players. set to 0 if not set
      hiding_players = doc["hiding_players"] != NULL ? doc["hiding_players"] : 0;
      player_type = doc["player_type"] == "hider" ? HIDER : SEEKER;
    }
    else if(doc["game_action"]=="stop")
    {
      //stop signal received from phone.
      game_status=END;
      //reset the rest of the game values
      if(doc["winner"]=="hider")
      {
        winner = HIDER;
      } 
      else 
      {
        winner = SEEKER;
      }
    }
    else if(doc["game_action"]=="caught")
    {
      if(doc["hiding_players"])
      {
        hiding_players=doc["hiding_players"];
      }
      else
      {
        hiding_players--;
      }
      player_caught();
    }
  }


  }
}
/*
Clears the screen prior and sets game header.
*/
void header(){
  display.clearScreen();
  displayText("HIDE",0);
  displayText("AND",10);
  displayText("SEEK",20);
  displayText("=========",30);
}

void pregame(){
  
//countdown timer
  for (int i = 5; i != -1;i--){
    display.clearScreen();
    displayText("GAME",0);
    displayText("IS",10);
    displayText("STARTING",20);
    char str[1];
    sprintf(str, "%d", i);
    displayText(str,40);
    delay(1000);
  }
  //set game status to HIDING
  game_status=HIDING;
  //get current seconds and add to the timers
  int seconds = rtc.getEpoch();
  hiding_time += seconds;
}
  
void hider_hiding(){
int i = hiding_time - rtc.getEpoch();
    if (i>=0)
    {
      
          header();
    displayText("YOU ARE: HIDING",40);
    char str[50];
    time_remaining(i, 50);
    }
else {
game_status=SEEKING;
//get current time and add to the timers
int seconds = rtc.getEpoch();
  seeker_time += seconds;
}
  
  
}

void hider_seeking(){
  int i = seeker_time - rtc.getEpoch();
  if(i>=0&&hiding_players!=0){
    clearDisplay();
    displayTextTopLeft("< Eliminate Player",0);
    players_remaining(20);
    displayText("YOU ARE: HIDING",40);
    
    time_remaining(i, 50);
    
    //eliminate hider
    if (checkButtons()==upLeftButton)
    {
      player_type=ELIMINATED;
      sendData("{\"eliminated\":1}");
      hiding_players--;
    }
  } else {
    //time ran down
    game_status=END;
    if(hiding_players==0)
    {
      winner = SEEKER;
    }
    else
    {
      winner = HIDER;
    }
  }
  
}

void seeker_hiding(){
int i = hiding_time - rtc.getEpoch();
    if (i>=0)
    {
      
          header();
    displayText("YOU ARE: SEEKING",40);
    char str[50];
    time_remaining(i, 50);
    }
else {
game_status=SEEKING;
//get current time and add to the timers
int seconds = rtc.getEpoch();
  seeker_time += seconds;
}
}

void seeker_seeking(){
  int i = seeker_time - rtc.getEpoch();
  if(i>=0&&hiding_players!=0){
    clearDisplay();
    players_remaining(20);
    displayText("YOU ARE: SEEKING",40);
    
    time_remaining(i, 50);
    
    //some functionality for buttons can be implemented here
    if (checkButtons()==upLeftButton)
    {
      
    }
  } else {
    //time ran down
    game_status=END;
    if(hiding_players==0)
    {
      winner = SEEKER;
    }
    else
    {
      winner = HIDER;
    }
  }
}

void hider_eliminated(){
  int i = seeker_time - rtc.getEpoch();
  if(i>=0&&hiding_players!=0){
  clearDisplay();
  displayText("ELIMINATED",20);
  displayText("Take the L",30);
  players_remaining(40);
  time_remaining(i, 50);
  }
  else {
    //time ran down
    game_status=END;
    if(hiding_players==0)
    {
      winner = SEEKER;
    }
    else
    {
      winner = HIDER;
    }
  }
}

void player_caught(){
  clearDisplay();
  #ifdef BLE_DEBUG
  PRINTF("A player was eliminated!");
  #endif
  if(player_type==HIDER){
    displayText("A PLAYER",20,red);
    displayText("WAS CAUGHT!",30,red);
  }
  else{
    displayText("A PLAYER",20,green);
    displayText("WAS CAUGHT!",30,green);
  }
  players_remaining(40);
  delay(3000);
}

void game_end(){
  header();
  displayText("GAME OVER",40);
  if(player_type==winner||(player_type==ELIMINATED&&winner==HIDER))
  {
    //if player_type==winner (HIDER==HIDER or SEEKER==SEEKER) or player is eliminated, and winner is hider.
    displayText("YOU WIN!",50,green);
  }
  else
  {
    displayText("YOU LOSE!",50,red);
  }
  game_status=NONE;
  //5 secs delay
  delay(5000);
  //reset the stats
  hiding_time=0;
  seeker_time=0;
  hiding_players=0;
  game_status = NONE;
  player_type = NEITHER;
  power_ups = NIL;
  screen_status=S_EMPTY;
}

void players_remaining(int offset)
{
  char s[50];
  sprintf(s,"Players Left: %d",hiding_players);
  displayText(s,offset);
}

void time_remaining(int i, int offset)
{
  int min = (i >= 60) ? floor(i/60) : 0;
    int secs = (i >= 60) ? i-(floor(i/60)*60) : i;
    char str[50];
    sprintf(str,"Time Left: %02d:%02d",min,secs);
    displayText(str,offset);
}
/*
Displays the home screen. Shows the device name and whether a device has been connected or not connected.
*/
void homeScreen(){
  
  if(connected)
  {
    if(screen_status!=S_CONNECTED)
    {
      header();
      displayText("Connected",40,green);
    screen_status=S_CONNECTED;
    }
    
  }
  else 
  {
    if (screen_status!=S_DISCONNECTED) 
    {
      header();
      displayText("Not connected",40,red);
    //displayText("Discoverable as",50);
    char devicename[15]="TinyCircuit-";
    strcat(devicename,randomString);
    displayText(devicename,50);
    screen_status=S_DISCONNECTED;
    }
    
  }
  //delay(1000);
}

 int checkButtons(){
   byte buttons = display.getButtons();
   if(buttonReleased && buttons){
    //a button was pressed
    int button = (uint8_t) buttons;
    PRINTF("Button was pressed!\n");
    int mapping_button = whatButton(button);
    switch (mapping_button)
    {
      case UPRIGHT:
      PRINTF("Up Right pressed!\n");
      break;
      case DOWNRIGHT:
      PRINTF("Down Right pressed!\n");
      break;
      case UPLEFT:
      PRINTF("Up Left pressed!\n");
      break;
      case DOWNLEFT:
      PRINTF("Down Left pressed!\n");
      break;
      default:
      PRINTF("No button pressed!\n");
    }
    buttonReleased = 0;
    return mapping_button;
   }
   if(!buttonReleased && !(buttons & 0x0f)) {
    //button not pressed.
    buttonReleased = 1;
    return 0;
   }
 }

int whatButton(int button)
{
  if (button==upRightButton)
  {
    return UPRIGHT;
  }
  else if (button==upLeftButton)
  {
    return UPLEFT;
  }
  else if (button==downRightButton)
  {
    return DOWNRIGHT;
  }
  else if (button==downLeftButton)
  {
    return DOWNLEFT;
  }
  //something else received
  return EMPTY;
}