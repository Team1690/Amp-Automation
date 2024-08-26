#include <Arduino.h>

int pinBeamBreakerAmp = 5; // is 5
int pinBeamBreakerSpeaker = 3; // is 3
int BeamBreakerAmp;
int LastBeamBreakerAmp = 1;
int BeamBreakerSpeaker;
int LastBeamBreakerSpeaker = 1;
int pinButtonPress = 4;
uint8_t pinButtonLed  = 6;
unsigned long lastAmpNoteTime = 0;
unsigned long lastSpeakerNoteTime = 0;
unsigned long ampTimerStart = 0;
int couterAmp = 0;
int totalScore = 0;
int buttonPress;
int lastButtonPress = 0;
int Ap = 9;
int Bp = 10;
int Cp = 11;
int Dp = 12;


enum class AmpState
{
  noNote, 
  oneNote, 
  readyForAmp, 
  amplified 
};

AmpState ampState = AmpState::noNote;

void setup()
{
  pinMode(pinButtonPress, INPUT_PULLUP);
  pinMode(pinBeamBreakerAmp, INPUT_PULLUP);
  pinMode(pinBeamBreakerSpeaker, INPUT_PULLUP);
  pinMode(pinButtonLed, OUTPUT);
  //pin segement
  pinMode(Ap, OUTPUT);
  pinMode(Bp, OUTPUT);
  pinMode(Cp, OUTPUT);
  pinMode(Dp, OUTPUT);
  Serial.begin(9600);  

  segWrite(0);
}

void loop()
{
  BeamBreakerAmp = digitalRead(pinBeamBreakerAmp);
  // check if new note enter amp
  // only one note allowed per second
  if (BeamBreakerAmp == 0 && LastBeamBreakerAmp == 1 && millis()- lastAmpNoteTime > 1000)
  {
    lastAmpNoteTime = millis();
    totalScore += 1;
    Serial.println(totalScore);
    if (ampState ==AmpState::noNote)
    {
      ampState =AmpState::oneNote;
      analogWrite(pinButtonLed, 50);
      segWrite(1);
    }
    else if (ampState == AmpState::oneNote)
    {
      ampState = AmpState::readyForAmp;
      digitalWrite(pinButtonLed, 1);// turn on button led
      segWrite(2);
    }
  }
  LastBeamBreakerAmp = BeamBreakerAmp;

  buttonPress = digitalRead(pinButtonPress);
  if(buttonPress == 0 && lastButtonPress == 1)
  {
    if(ampState == AmpState::readyForAmp){
      Serial.println("AMPLIFIED");
      ampState = AmpState::amplified;
      ampTimerStart = millis();
    } else if(ampState == AmpState::noNote){
      segWrite(totalScore);
    }
  }
  lastButtonPress = buttonPress;
  if (ampState == AmpState::amplified)
  {
    int nomSecond = (millis() - ampTimerStart)/1000;
    if (nomSecond % 2 == 0)
    {
      digitalWrite(pinButtonLed, 0);
    }
    else
    {
      digitalWrite(pinButtonLed, 1); 
    }

    //10 second countdown
    if (nomSecond > 9)
    {
      Serial.println("AMPLIFIED END \n\n\n");
      ampState = AmpState::noNote;
      digitalWrite(pinButtonLed, 0);

    } 
    segWrite(9-nomSecond);
    delay(300);
  } 

  // count speaker point
  BeamBreakerSpeaker = digitalRead(pinBeamBreakerSpeaker);

  if (BeamBreakerSpeaker == 0 && LastBeamBreakerSpeaker == 1 && millis()- lastSpeakerNoteTime > 1000)
  {
    lastSpeakerNoteTime = millis();
    if (ampState != AmpState::amplified)
    {
        totalScore += 2;
    }
    else {
        totalScore += 5;
    }
    Serial.println(totalScore);
    Serial.println("Speaker");
  }
  LastBeamBreakerSpeaker = BeamBreakerSpeaker;

}
//BCD to sevent segement
void segWrite(int n){
  ///Decimal-Binary Conversion Function

  //Turns off all LEDs
  digitalWrite(Dp, LOW);
  digitalWrite(Cp, LOW);
  digitalWrite(Bp, LOW);
  digitalWrite(Ap, LOW);

  ///Decimal Conversion
  ///Can be optimized :)
  if(n >= 8){
    digitalWrite(Dp, HIGH);
    n = n - 8 ;
  }
  if(n >= 4){  
    digitalWrite(Cp, HIGH);
    n = n - 4;
  } 
  if(n>=2){
    digitalWrite(Bp, HIGH);
    n = n - 2;
  }
  if(n>=1){
    digitalWrite(Ap, HIGH);
    n = n - 1;
}}