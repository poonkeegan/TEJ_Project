int dataPin = 4;
int latchPin = 3;
int clockPin = 2;
int wirelessIn = 0;
int wirelessOut = 1;
int secondNum = 5;
int minuteNum = 3;
int hourNum = 1;
int minuteButton = 12;
int hourButton = 11;
int alarmSet = 13;
int buzzer = 8;
int pushInt = 200;
boolean noise = false;
unsigned long previousMillis = 0;        // will store last time LED was updated
unsigned long previousPush = 0;        // will store last time hour button pushed
unsigned long previousPushMin = 0;        // will store last time minute button pushed

const long interval = 1000;           // interval at which to blink (milliseconds)
byte ZERO = 1; 
byte ONE = 199;
byte TWO = 34;
byte THREE = 130;
byte FOUR = 196;
byte FIVE = 136;
byte SIX = 8;
byte SEVEN = 195;
byte EIGHT = 0;
byte NINE = 128;
byte SegDisplay;
int clock[6];
int alarm[6];
void setup() {
// set the digital pin as output:
  pinMode(wirelessIn, INPUT);
  pinMode(wirelessOut, OUTPUT);    
  pinMode(clockPin, OUTPUT);    
  pinMode(latchPin, OUTPUT);    
  pinMode(dataPin, OUTPUT);    
  pinMode(5, OUTPUT);    
  pinMode(6, OUTPUT);    
  pinMode(7, OUTPUT);    
  pinMode(8, OUTPUT);    
  pinMode(9, OUTPUT);    
  pinMode(10, INPUT);    
  pinMode(minuteButton, INPUT);    
  pinMode(hourButton, INPUT);    
  pinMode(alarmSet, INPUT);   
  for(int i = 0; i < 6; i++){
    clock[i] = 0;
    alarm[i] = 0;
  }
  Serial.begin(9600); 
}

void loop()
{
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis >= interval) {
    
    previousMillis = currentMillis;   
    addClockVal(secondNum, 1);
    
  }
  
  int displayClock[6];
  boolean alarmButtonOn = false;
  if(digitalRead(alarmSet)){
    //if alarm button not pressed set clock to be shifted out
    for(int i = 0; i < 6; i++){
      displayClock[i] = clock[i];
    }
  }else{
    alarmButtonOn = true;
    for(int i = 0; i < 6; i++){
      //otherwise set alarm values to shift out
      displayClock[i] = alarm[i];
    }
  }
  
 //hour button check
  if(currentMillis - previousPush >= pushInt){
    if(!digitalRead(hourButton)){
      if(alarmButtonOn){
        addAlarmVal(hourNum, 1);
      }else{
        addClockVal(hourNum, 1);
      }
      previousPush = currentMillis;
      //delay for holding down button
    }
  }else if(digitalRead(hourButton)){
    previousPush = currentMillis;
    //if button is released can press again with no delay
  }
  
  //minute button check
  if(currentMillis - previousPushMin >= pushInt){
    if(!digitalRead(minuteButton)){
      if(alarmButtonOn){
        addAlarmVal(minuteNum, 1);
      }else{
        addClockVal(minuteNum, 1);
      }
      previousPushMin = currentMillis;
    }
  }else if(digitalRead(minuteButton)){
    previousPushMin = currentMillis;
  }

  //shift out clock values
  digitalWrite(latchPin, LOW); 
  for(int i = 0; i <6; i++){
    SegDisplay = displayNum(displayClock[i]);
    shiftOut(dataPin, clockPin, MSBFIRST, SegDisplay);
  }
  digitalWrite(latchPin, HIGH);
  
  //buzz check
  if(!digitalRead(10)){
     noise = false;
  }
  if(noise)
  {
    digitalWrite(8, HIGH);
    //if the buzz is removed, then clock stops rapidly changing when alarm goes off, no other fixed currently known
  }else{
    noTone(8);
  }
  
}
void addClockVal(int clockNum, int addVal){
  //add seconds
  clock[clockNum]+=addVal;
  if (clock[5] > 9){
    clock[5] = 0;
    clock[4]++;
    if(clock[4] > 5){
      clock[4] = 0;
      clock[3]++;
    }
  }
  //add minutes
  if (clock[3] > 9){
    clock[3] = 0;
    clock[2]++;
    if(clock[2] > 5){
      clock[2] = 0;
      clock[1]++;
    }
  }//add hours
  if (clock[0] == 0||clock[0] == 1){
    if(clock[1] > 9){
    clock[1] = 0;
    clock[0]++;
    }
  }
  else if (clock[1] > 3){
    clock[1] = 0;
    clock[0] = 0;
  }
  
    if(!noise){
      if(clock[0] == alarm[0] && clock[1] == alarm[1] && clock[2] == alarm[2] && clock[3] == alarm[3] && clock[4] == alarm[4] && clock[5] == alarm[5]){
        noise = true;
      }
    }
}


void addAlarmVal(int clockNum, int addVal){
  //add seconds
  alarm[clockNum]+=addVal;
  if (alarm[5] > 9){
    alarm[5] = 0;
    alarm[4]++;
    if(alarm[4] > 5){
      alarm[4] = 0;
      alarm[3]++;
    }
  }
  //add minutes
  if (alarm[3] > 9){
    alarm[3] = 0;
    alarm[2]++;
    if(alarm[2] > 5){
      alarm[2] = 0;
      alarm[1]++;
    }
  }//add hours
  if (alarm[0] == 0||alarm[0] == 1){
    if(alarm[1] > 9){
    alarm[1] = 0;
    alarm[0]++;
    }
  }
  else if (alarm[1] > 3){
    alarm[1] = 0;
    alarm[0] = 0;
  }
}



byte displayNum(int numberVal){
//values that the shift registers changes to led digits
  if(numberVal == 0){
    return 1;
  }
  else if(numberVal==1){
    return 199;
  }
  else if(numberVal == 2){
    return 34;
  }
  else if(numberVal == 3){
    return 130;
  }
  else if(numberVal == 4){
    return 196;
  }
  else if(numberVal == 5){
    return 136;
  }
  else if(numberVal == 6){
    return 8;
  }
  else if(numberVal == 7){
    return 195;
  }
  else if(numberVal == 8){
    return  0;
  }
  else if(numberVal == 9){
    return 128;
  }
  
}


