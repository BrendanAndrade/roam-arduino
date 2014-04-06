const byte soundPin = 6;
const byte vibePin = 5;
const byte buttonPin = 2;

const int alarmPeriod = 500; //Period of alarm pulses in ms
const int soundFreq = 2170; //Frequency of sound

boolean alarmHIGH = true;
boolean warnHIGH = true;
unsigned long lastAlarmTime;
unsigned long lastWarnTime;

unsigned long lastMessageTime;
const unsigned long disconnectTimeout = 4000; // ms

volatile boolean pressActive = false; 

boolean sound = true;
boolean vibe = true;
boolean alarmOn = false;
boolean warnOn = false;

unsigned long aTime;

void setup(){
  
  pinMode(soundPin, OUTPUT);
  pinMode(vibePin, OUTPUT);
  pinMode(buttonPin, INPUT);
  
  attachInterrupt(0, emergencyPress, FALLING);
  
  Serial.begin(115200);  
  
  aTime = millis();
  
}

void loop(){
  
  while(Serial.available() > 0){
    char inChar = Serial.read();
    
    readChar(inChar);
    lastMessageTime = millis();
    
  }
  
  if (millis() - lastMessageTime > disconnectTimeout){
    alarmOn = true;
    Serial.print('X');
  }   
    
  if (alarmOn == true){
    unsigned long currentTime = millis();
    if (currentTime - lastAlarmTime > alarmPeriod*0.5){
      alarmToggle();
      lastAlarmTime = currentTime;
    }
  }
  
  if (warnOn == true){
    unsigned long currentTime = millis();
    if (currentTime - lastWarnTime > alarmPeriod*0.5){
        warnToggle();
        lastWarnTime = currentTime;
    }
  }
  
  if (pressActive == true){
    Serial.print('Y');
    pressActive = false;
  }
  /*
  while (aTime + 100 > millis()){
  }
  aTime = millis();
  Serial.print('A');
  */ 
}

void readChar(char inChar){
  switch (inChar) {
    case 'A':
      Serial.print('A');
      break;
    case 'H':
      warnOn = false;
      break;
    case 'I':
      warnOn = true;
      break;
    case 'J':
      sound = true;
      vibe = true;
      break;
    case 'K':
      sound = true;
      vibe = false;
      break;
    case 'L':
      sound = false;
      vibe = true;
      break;
    case 'M':
      sound = false;
      vibe = false;
      break;
    case 'N':
      alarmOn = true;
      warnOn = false;
      break;
    case 'P':
      alarmKill();
      break;
  }  
}

void emergencyPress(){
  pressActive = true;    
}

void alarmToggle(){
  if (alarmHIGH == true){
    if (sound == true){
      soundOn();
    }
    if (vibe == true){
      vibeOn();
    }
    alarmHIGH = false;
  }
  else{
    soundOff();
    vibeOff();
    alarmHIGH = true;
  }
}

void warnToggle(){
  if (warnHIGH == true){
    vibeOn();
    warnHIGH = false;
  }
  else{
    vibeOff();
    warnHIGH = true;
  }
}
    

void alarmKill(){
  soundOff();
  vibeOff();
  alarmOn = false;
  alarmHIGH =true;
}

void soundOn(){
  tone(soundPin, soundFreq);
}

void soundOff(){
  noTone(soundPin);
}

void vibeOn(){
  digitalWrite(vibePin, HIGH);
}

void vibeOff(){
  digitalWrite(vibePin, LOW);
}
