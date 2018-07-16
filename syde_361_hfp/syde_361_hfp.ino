
#include <MIDI.h>

// See https://www.pjrc.com/teensy/td_midi.html for more MIDI commands you can use
// Use a program like https://www.snoize.com/MIDIMonitor/ to monitor your MIDI communication
// bus to see if what you're sending is actually working

int channel = 1; // Defines the MIDI channel to send messages on (values from 1-16)
int velocity = 100; // Defines the velocity that the note plays at (values from 0-127)

const int FSR1Pin = 32;
const int FSR2Pin = 31;
const int FSR3Pin = 36;
const int FSR4Pin = 38;
const int FSR5Pin = 14;
const int FSR6Pin = 15;
const int FSR7Pin = 17;
const int FSR8Pin = 19;
const int FSR9Pin = A22;
const int FSR10Pin = 23;

const int PiezzoPin1 = 33;
const int PiezzoPin2 = 34;
const int PiezzoPin3 = 35;
const int PiezzoPin4 = 37;
const int PiezzoPin5 = 39;
const int PiezzoPin6 = A21;
const int PiezzoPin7 = 16;
const int PiezzoPin8 = 18;
const int PiezzoPin9 = 22;
const int PiezzoPin10 = 21;

const int select_button = 7;

const int pitchNob = 20;


const int resetButton = 6;

const int led_1 = 2;
const int led_2 = 3;
const int led_3 = 4;
const int led_4 = 5;

int FSR1IsOn = 0;
int FSR2IsOn = 0;
int FSR3IsOn = 0;
int FSR4IsOn = 0;
int FSR5IsOn = 0;
int FSR6IsOn = 0;
int FSR7IsOn = 0;
int FSR8IsOn = 0;
int FSR9IsOn = 0;
int FSR10IsOn = 0;
int Piezzo1IsOn = 0;
int Piezzo2IsOn = 0;
int Piezzo3IsOn = 0;
int Piezzo4IsOn = 0;
int Piezzo5IsOn = 0;
int Piezzo6IsOn = 0;
int Piezzo7IsOn = 0;
int Piezzo8IsOn = 0;
int Piezzo9IsOn = 0;
int Piezzo10IsOn = 0;
int curr_led = 2;

int lastButtonState = 0;
int lastResetState = 0;
int knobZero = 0;
int bend = 0;

const int debounceValue = 100;

const int dampeningThreshold = 350;
const int hittingThreshold = 1000;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(select_button, INPUT);
  pinMode(resetButton, INPUT);
  pinMode(curr_led, OUTPUT);
  pinMode(curr_led + 1, OUTPUT);
  pinMode(curr_led + 2, OUTPUT);
  pinMode(curr_led + 3, OUTPUT);
  digitalWrite(curr_led, HIGH);
}

void loop() {
  int FSR1Reading = analogRead(FSR1Pin); //key 1 dampen
  int FSR2Reading = analogRead(FSR2Pin); //key 2 dampen
  int FSR3Reading = analogRead(FSR3Pin); //key 3 dampen
  int FSR4Reading = analogRead(FSR4Pin); //key 4 dampen
  int FSR5Reading = analogRead(FSR5Pin); //key 5 dampen
  int FSR6Reading = analogRead(FSR6Pin); //key 6 dampen
  int FSR7Reading = analogRead(FSR7Pin); //key 7 dampen
  int FSR8Reading = analogRead(FSR8Pin); //key 8 dampen
  int FSR9Reading = analogRead(FSR9Pin); //key 9 dampen
  int FSR10Reading = analogRead(FSR10Pin); //key 10 dampen
  
  int Piezzo1Reading = analogRead(PiezzoPin1); //key 1
  int Piezzo2Reading = analogRead(PiezzoPin2); //key 2
  int Piezzo3Reading = analogRead(PiezzoPin3); //key 3
  int Piezzo4Reading = analogRead(PiezzoPin4); //key 4
  int Piezzo5Reading = analogRead(PiezzoPin5); //key 5
  int Piezzo6Reading = analogRead(PiezzoPin6); //key 6
  int Piezzo7Reading = analogRead(PiezzoPin7); //key 7
  int Piezzo8Reading = analogRead(PiezzoPin8); //key 8
  int Piezzo9Reading = analogRead(PiezzoPin9); //key 9
  int Piezzo10Reading = analogRead(PiezzoPin10); //key 10
  
  int select_button_reading = digitalRead(select_button);

  int pitchNobReading = analogRead(pitchNob);
  int resetButtonReading = digitalRead(resetButton);
  
    Serial.print("pitchNob ");
    Serial.println(pitchNobReading);
//  Serial.print("FSR6Pin: ");
//  Serial.println(FSR6Reading);
//  Serial.print("FSR3Pin: ");
//  Serial.println(Piezzo8Reading);
  bend = -2*knobZero + 2*pitchNobReading;
  usbMIDI.sendPitchBend(bend,channel);

  if (resetButtonReading == HIGH && lastResetState == 0)
  {
    knobZero = pitchNobReading;
    usbMIDI.sendPitchBend(0,channel);
  }
  
  if (select_button_reading == HIGH && lastButtonState == 0)
  {
    digitalWrite(curr_led, 0);
    curr_led += 1;
    if (curr_led >= 6) {
      curr_led = 2;
    }
    
    channel %= 4;
    channel += 1; 
    
    digitalWrite(curr_led, 1);
  }
  lastButtonState = select_button_reading;

  
  // KEY1 Damp and sound
  if(Piezzo1Reading >= hittingThreshold && FSR1Reading <= dampeningThreshold && Piezzo1IsOn <= 0) //key 1 sound
  {
    velocity = map(Piezzo1Reading, hittingThreshold, 1023,0,100);
   // Serial.print("FSR6Pin: ");
  //  Serial.println(FSR6Reading);
    Piezzo1IsOn = debounceValue;
    usbMIDI.sendNoteOff(70, velocity, channel);
    usbMIDI.sendNoteOn(70, velocity, channel);
  }
  else if(Piezzo1Reading < hittingThreshold)
  {
    Piezzo1IsOn--;
  }
  else if(Piezzo1IsOn <= -1000)
  {
    usbMIDI.sendNoteOff(70, velocity, channel);
  }

  if(FSR1Reading > dampeningThreshold && Piezzo1Reading < hittingThreshold && FSR1IsOn <= 0) //key 1 damp
  {
    velocity = map(FSR1Reading, dampeningThreshold, 1023,0,100);
    FSR1IsOn = debounceValue;
    usbMIDI.sendNoteOff(70, velocity, channel);
  }
  else if(FSR1Reading < dampeningThreshold)
  {
    FSR1IsOn--;
  }

  // KEY2 damp and sound
  if(Piezzo2Reading >= hittingThreshold && FSR2Reading <= dampeningThreshold && Piezzo2IsOn <= 0) //key 2 sound
  {
    velocity = map(Piezzo2Reading, hittingThreshold, 1023,0,100);
//    Serial.print("Piezzo2Reading: ");
//    Serial.println(Piezzo2Reading);
//    Serial.print("Piezzo1Reading: ");
//    Serial.println(Piezzo1Reading);
//    Serial.print("Piezzo3Reading: ");
//    Serial.println(Piezzo3Reading);
    Piezzo2IsOn = debounceValue;
    usbMIDI.sendNoteOff(73, velocity, channel);
    usbMIDI.sendNoteOn(73, velocity, channel);
  }
  else if(Piezzo2Reading < hittingThreshold)
  {
    Piezzo2IsOn--;
  }
  else if(Piezzo2IsOn <= -1000)
  {
    usbMIDI.sendNoteOff(73, velocity, channel);
  }

  if(FSR2Reading > dampeningThreshold && Piezzo2Reading < hittingThreshold && FSR2IsOn <= 0) //key 2 damp
  {
    velocity = map(FSR2Reading, dampeningThreshold, 1023,0,100);
    FSR2IsOn = debounceValue;
    usbMIDI.sendNoteOff(73, velocity, channel);
  }
  else if(FSR2Reading < dampeningThreshold)
  {
    FSR2IsOn--;
  }

  // Key3 damp and sound
  if(Piezzo3Reading >= hittingThreshold && FSR3Reading <= dampeningThreshold && Piezzo3IsOn <= 0) //key 3 sound
  {
    velocity = map(Piezzo3Reading, hittingThreshold, 1023,0,100);
//  Serial.print("FSR6Pin: ");
//  Serial.println(FSR6Reading);
    Piezzo3IsOn = debounceValue;
    usbMIDI.sendNoteOff(75, velocity, channel);
    usbMIDI.sendNoteOn(75, velocity, channel);
  }
  else if(Piezzo3Reading < hittingThreshold)
  {
    Piezzo3IsOn--;
  }
  else if(Piezzo3IsOn <= -1000)
  {
    usbMIDI.sendNoteOff(75, velocity, channel);
  }

  if(FSR3Reading > dampeningThreshold && Piezzo3Reading < hittingThreshold && FSR3IsOn <= 0) //key 3 damp
  {
    velocity = map(FSR3Reading, dampeningThreshold, 1023,0,100);
    FSR3IsOn = debounceValue;
    usbMIDI.sendNoteOff(75, velocity, channel);
  }
  else if(FSR3Reading < dampeningThreshold)
  {
    FSR3IsOn--;
  }

  //KEY4 damp and sound
  if(Piezzo4Reading >= hittingThreshold && FSR4Reading <= dampeningThreshold && Piezzo4IsOn <= 0) //key 4 sound
  {
    velocity = map(Piezzo1Reading, hittingThreshold, 1023,0,100);
    //Serial.println(velocity);
    Piezzo4IsOn = debounceValue;
    usbMIDI.sendNoteOff(77, velocity, channel);
    usbMIDI.sendNoteOn(77, velocity, channel);
  }
  else if(Piezzo4Reading < hittingThreshold)
  {
    Piezzo4IsOn--;
  }
  else if(Piezzo4IsOn <= -1000)
  {
    usbMIDI.sendNoteOff(77, velocity, channel);
  }

  if(FSR4Reading > dampeningThreshold && Piezzo4Reading < hittingThreshold && FSR4IsOn <= 0) //key 4 damp
  {
    velocity = map(FSR4Reading, dampeningThreshold, 1023,0,100);
    FSR4IsOn = debounceValue;
    usbMIDI.sendNoteOff(77, velocity, channel);
  }
  else if(FSR4Reading < dampeningThreshold)
  {
    FSR4IsOn--;
  }

  //KEY5 damp and sound
  if(Piezzo5Reading >= hittingThreshold && FSR5Reading <= dampeningThreshold && Piezzo5IsOn <= 0) //key 5 sound
  {
    velocity = map(Piezzo5Reading, hittingThreshold, 1023,0,100);
    //Serial.println(velocity);
    Piezzo5IsOn = debounceValue;
    usbMIDI.sendNoteOff(80, velocity, channel);
    usbMIDI.sendNoteOn(80, velocity, channel);
  }
  else if(Piezzo5Reading < hittingThreshold)
  {
    Piezzo5IsOn--;
  }
  else if(Piezzo5IsOn <= -1000)
  {
    usbMIDI.sendNoteOff(80, velocity, channel);
  }

  if(FSR5Reading > dampeningThreshold && Piezzo5Reading < hittingThreshold && FSR5IsOn <= 0) //key 5 damp
  {
    velocity = map(FSR5Reading, dampeningThreshold, 1023,0,100);
    FSR5IsOn = debounceValue;
    usbMIDI.sendNoteOff(80, velocity, channel);
  }
  else if(FSR5Reading < dampeningThreshold)
  {
    FSR5IsOn--;
  }

  //KEY6 damp and sound
  if(Piezzo6Reading >= hittingThreshold && FSR6Reading <= dampeningThreshold && Piezzo6IsOn <= 0) //key 6 sound
  {
    velocity = map(Piezzo6Reading, hittingThreshold, 1023,0,100);
    //Serial.println(velocity);
    Piezzo6IsOn = debounceValue;
    usbMIDI.sendNoteOff(82, velocity, channel);
    usbMIDI.sendNoteOn(82, velocity, channel);
  }
  else if(Piezzo6Reading < hittingThreshold)
  {
    Piezzo6IsOn--;
  }
  else if(Piezzo6IsOn <= -1000)
  {
    usbMIDI.sendNoteOff(82, velocity, channel);
  }

  if(FSR6Reading > dampeningThreshold && Piezzo6Reading < hittingThreshold && FSR6IsOn <= 0) //key 6 damp
  {
    velocity = map(FSR6Reading, dampeningThreshold, 1023,0,100);
    FSR6IsOn = debounceValue;
    usbMIDI.sendNoteOff(82, velocity, channel);
  }
  else if(FSR6Reading < dampeningThreshold)
  {
    FSR6IsOn--;
  }

  //KEY7 damp and sound
  if(Piezzo7Reading >= hittingThreshold && FSR7Reading <= dampeningThreshold && Piezzo7IsOn <= 0) //key 7 sound
  {
    velocity = map(Piezzo7Reading, hittingThreshold, 1023,0,100);
    //Serial.println(velocity);
    Piezzo7IsOn = debounceValue;
    usbMIDI.sendNoteOff(85, velocity, channel);
    usbMIDI.sendNoteOn(85, velocity, channel);
  }
  else if(Piezzo7Reading < hittingThreshold)
  {
    Piezzo7IsOn--;
  }
  else if(Piezzo7IsOn <= -1000)
  {
    usbMIDI.sendNoteOff(85, velocity, channel);
  }

  if(FSR7Reading > dampeningThreshold && Piezzo7Reading < hittingThreshold && FSR7IsOn <= 0) //key 7 damp
  {
    velocity = map(FSR7Reading, dampeningThreshold, 1023,0,100);
    FSR7IsOn = debounceValue;
    usbMIDI.sendNoteOff(85, velocity, channel);
  }
  else if(FSR7Reading < dampeningThreshold)
  {
    FSR7IsOn--;
  }

  //KEY8 damp and sound
  if(Piezzo8Reading >= hittingThreshold && FSR8Reading <= dampeningThreshold && Piezzo8IsOn <= 0) //key 8 sound
  {
    velocity = map(Piezzo8Reading, hittingThreshold, 1023,0,100);
    //Serial.println(velocity);
    Piezzo8IsOn = debounceValue;
    usbMIDI.sendNoteOff(87, velocity, channel);
    usbMIDI.sendNoteOn(87, velocity, channel);
  }
  else if(Piezzo8Reading < hittingThreshold)
  {
    Piezzo8IsOn--;
  }
  else if(Piezzo8IsOn <= -1000)
  {
    usbMIDI.sendNoteOff(87, velocity, channel);
  }

  if(FSR8Reading > dampeningThreshold && Piezzo8Reading < hittingThreshold && FSR8IsOn <= 0) //key 8 damp
  {
    velocity = map(FSR8Reading, dampeningThreshold, 1023,0,100);
    FSR8IsOn = debounceValue;
    usbMIDI.sendNoteOff(87, velocity, channel);
  }
  else if(FSR8Reading < dampeningThreshold)
  {
    FSR8IsOn--;
  }

  //KEY9 damp and sound
  if(Piezzo9Reading >= hittingThreshold && FSR9Reading <= dampeningThreshold && Piezzo9IsOn <= 0) //key 9 sound
  {
    velocity = map(Piezzo9Reading, hittingThreshold, 1023,0,100);
    //Serial.println(velocity);
    Piezzo9IsOn = debounceValue;
    usbMIDI.sendNoteOff(89, velocity, channel);
    usbMIDI.sendNoteOn(89, velocity, channel);
  }
  else if(Piezzo9Reading < hittingThreshold)
  {
    Piezzo9IsOn--;
  }
  else if(Piezzo9IsOn <= -1000)
  {
    usbMIDI.sendNoteOff(89, velocity, channel);
  }

  if(FSR9Reading > dampeningThreshold && Piezzo9Reading < hittingThreshold && FSR9IsOn <= 0) //key 9 damp
  {
    velocity = map(FSR9Reading, dampeningThreshold, 1023,0,100);
    FSR9IsOn = debounceValue;
    usbMIDI.sendNoteOff(89, velocity, channel);
  }
  else if(FSR9Reading < dampeningThreshold)
  {
    FSR9IsOn--;
  }

  //KEY10 damp and sound
  if(Piezzo10Reading >= hittingThreshold && FSR10Reading <= dampeningThreshold && Piezzo10IsOn <= 0) //key 10 sound
  {
    velocity = map(Piezzo10Reading, hittingThreshold, 1023,0,100);
    //Serial.println(velocity);
    Piezzo10IsOn = debounceValue;
    usbMIDI.sendNoteOff(92, velocity, channel);
    usbMIDI.sendNoteOn(92, velocity, channel);
  }
  else if(Piezzo10Reading < hittingThreshold)
  {
    Piezzo10IsOn--;
  }
  else if(Piezzo10IsOn <= -1000)
  {
    usbMIDI.sendNoteOff(92, velocity, channel);
  }

  if(FSR10Reading > dampeningThreshold && Piezzo10Reading < hittingThreshold && FSR10IsOn <= 0) //key 10 damp
  {
    velocity = map(FSR10Reading, dampeningThreshold, 1023,0,100);
    FSR10IsOn = debounceValue;
    usbMIDI.sendNoteOff(92, velocity, channel);
  }
  else if(FSR10Reading < dampeningThreshold)
  {
    FSR10IsOn--;
  }

  
}


