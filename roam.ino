const byte soundPin = 11;
const byte vibePin = 13;
const byte buttonPin = 2;

const int alarmPeriod = 500; //Period of alarm pulses in ms
const int soundFreq = 2000; //Frequency of sound

boolean alarmHIGH = true;
unsigned long lastAlarmTime;

unsigned long lastMessageTime;
const unsigned long disconnectTimeout = 1000; // ms

volatile boolean pressActive = false; 

boolean sound = true;
boolean vibe = true;
boolean alarmOn = false;


void setup(){
  
  pinMode(soundPin, OUTPUT);
  pinMode(vibePin, OUTPUT);
  pinMode(buttonPin, INPUT);
  
  attachInterrupt(0, emergencyPress, FALLING);
  
  Serial.begin(115200);  
}

void loop(){
  
  if(Serial.available() > 0){
    char inChar = Serial.read();
    
    readChar(inChar);
    lastMessageTime = millis();
    
  }
  
  if (millis() - lastMessageTime > disconnectTimeout){
    alarmOn == true;
    Serial.print('X');
  }   
    
  if (alarmOn == true){
    unsigned long currentTime = millis();
    if (currentTime - lastAlarmTime > alarmPeriod*0.5){
      alarmToggle();
      lastAlarmTime = currentTime;  
    }
  }
  
  if (pressActive == true){
    Serial.print('Y');
    pressActive = false;
  }
    
  delay(10);
  
  
  
}

void readChar(char inChar){
  switch (inChar) {
    case 'A':
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
    soundOn();
    vibeOn();
    alarmHIGH = false;
  }
  else{
    soundOff();
    vibeOff();
    alarmHIGH = true;
  }
}

void alarmKill(){
  soundOff();
  vibeOff();
  alarmOn = false;
  alarmHIGH =true;
}

void soundOn(){
  if(sound == true){
    tone(soundPin, soundFreq);
  }
}

void soundOff(){
  noTone(soundPin);
}

void vibeOn(){
  if (vibe == true){
    digitalWrite(vibePin, HIGH);
  }
}

void vibeOff(){
  digitalWrite(vibePin, LOW);
}
