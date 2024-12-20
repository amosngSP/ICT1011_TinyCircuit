enum status{
  NONE, //no status
  PREGAME, //countdown from 5 to 0 for game start
  HIDING, //hiding stage
  SEEKING, //seeking stage
  END //game end
};

enum playerType {
  HIDER,
  SEEKER,
  ELIMINATED,
  NEITHER
};

enum powerUps{
  NIL,
  FREEZE_SEEKER,
  FIND_HIDERS
};

enum screenStatus{
  S_EMPTY,
  S_DISCONNECTED,
  S_CONNECTED,
  S_CAUGHT
};

enum buttonPressed{
  UPRIGHT,
  DOWNRIGHT,
  UPLEFT,
  DOWNLEFT,
  EMPTY
};