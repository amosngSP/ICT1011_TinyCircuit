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
  //mainGame();
  maingame();
  // countdownTimer(secs);
  // secs--;
   delay(1000);
}