
//initial global declaration for use later
enum status game_status = NONE;
enum playerType player_type = NEITHER;
enum powerUps power_ups = NIL;
enum screenStatus screen_status = S_DISCONNECTED;

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
  else if(game_status==HIDING)
  {
    hiding();
    //when the status is HIDING
  }
  else if(game_status==SEEKING)
  {
    //when the status is SEEKING
  }
  else if (game_status==END)
  {
    //when the status is END
  }

  if(ble_rx_buffer_len){
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
      PRINTF((char*)ble_rx_buffer);
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
      hiding_time=0;
      seeker_time=0;
      hiding_players=0;
      game_status = NONE;
      player_type = NEITHER;
      power_ups = NIL;
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
}
  
void hiding(){
  header();
  displayText("Are you hiding?",40);
}



/*
Displays the home screen. Shows the device name and whether a device has been connected or not connected.
*/
void homeScreen(){
  header();
  if(connected)
  {
    displayText("Connected",40,green);
    screen_status=S_CONNECTED;
  }
  else 
  {
    displayText("Not connected",40,red);
    //displayText("Discoverable as",50);
    char devicename[15]="TinyCircuit-";
    strcat(devicename,randomString);
    displayText(devicename,50);
    screen_status=S_DISCONNECTED;
  }
  delay(1000);
}


