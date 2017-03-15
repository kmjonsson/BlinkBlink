#include "PinChangeInterrupt.h"

#include "BlinkBlink.h"
#include "BlinkBlinkSeq.h"

event *events[OUTPUTS] = {
  ch1event,
  ch2event,
  ch3event,
  ch4event,
  ch5event,
  ch6event,
  ch7event,
  ch8event
};

// Code...

#define RC1PIN A1

volatile unsigned long rc1stamp=0;
volatile uint16_t v_rc1=0;

void rc1Check() {
  unsigned long m = micros();
  if(m - rc1stamp > 100000) {
    v_rc1=0;
  }  
}
void rc1Calc() {
  unsigned long m = micros();
  if(digitalRead(RC1PIN)) {
    rc1stamp = m;
  } else {
    if(m - rc1stamp > 2500 || m-rc1stamp < 900) {
      v_rc1=0;
    } else {
      v_rc1 = m-rc1stamp;
    }
  }
}

event curr_events[OUTPUTS];

void setup() {
  Serial.begin(57600);
  Serial.println("BlinkBlink");
  for(int i=0;i<OUTPUTS;i++) {
    pinMode(pins[i], OUTPUT);
    memcpy(&curr_events[i],&blank,sizeof(event));
  }
  pinMode(RC1PIN,INPUT);  
  attachPCINT(digitalPinToPCINT(RC1PIN), rc1Calc, CHANGE);
  delay(500);
}

void next_event(int i) {
  static int16_t curr_event[OUTPUTS] = { -2, -2, -2, -2, -2, -2, -2, -2 };

  uint16_t rc1 = v_rc1;
  uint16_t rc2 = v_rc1;

  if(curr_event[i] == -2) {
    if(DEBUG && i == DEBUG_CH) { Serial.println("INIT"); }
    int j;
    for(int j=0;j<MAX_EVENTS;j++) {
      if(events[i][j].flags == EOE) {
        curr_event[i] = j-1;
        break;
      }
    }
    if(j == MAX_EVENTS) {
      if(DEBUG && i == DEBUG_CH) { Serial.print("ERROR: Ch"); Serial.print(i); Serial.println(" has no EOE"); }
      curr_event[i] = -1;
      memcpy(&curr_events[i],&blank,sizeof(event));
      return;
    }
    if(DEBUG && i == DEBUG_CH) { Serial.println("INIT DONE"); }
  }

  while(1) {
    // Next event
    curr_event[i]++;

    if(curr_event[i] >= MAX_EVENTS) {
      if(DEBUG && i == DEBUG_CH) { Serial.print("ERROR: Ch"); Serial.print(i); Serial.println(" has no EOE"); }
      memcpy(&curr_events[i],&blank,sizeof(event));
      return;
    }   
  
    // End of events
    if(events[i][curr_event[i]].flags == EOE) {
      if(DEBUG && i == DEBUG_CH) { Serial.print("EOE - "); Serial.print(curr_event[i]); Serial.println(""); }
      curr_event[i] = -1;
      return;
    }
    
    if((events[i][curr_event[i]].flags & RC1_HIGH) == RC1_HIGH && !IS_RC_HIGH(rc1)) { continue; }
    if((events[i][curr_event[i]].flags & RC1_MID)  == RC1_MID  && !IS_RC_MID(rc1))  { continue; }
    if((events[i][curr_event[i]].flags & RC1_LOW)  == RC1_LOW  && !IS_RC_LOW(rc1))  { continue; }
    if((events[i][curr_event[i]].flags & RC2_HIGH) == RC2_HIGH && !IS_RC_HIGH(rc2)) { continue; }
    if((events[i][curr_event[i]].flags & RC2_MID)  == RC2_MID  && !IS_RC_MID(rc2))  { continue; }
    if((events[i][curr_event[i]].flags & RC2_LOW)  == RC2_LOW  && !IS_RC_LOW(rc2))  { continue; }
    break;
  }
  // Copy next event
  memcpy(&curr_events[i],&events[i][curr_event[i]],sizeof(event));
  if(DEBUG && i == DEBUG_CH) {
    Serial.print(i);
    Serial.print(" - ");
    Serial.print(curr_event[i]);
    Serial.print(" - ");
    Serial.print(curr_events[i].ms);
    Serial.print(" - ");
  }
  // If NO_RC do:
  if(curr_events[i].flags != EOE) {
    if(DEBUG && i == DEBUG_CH) { Serial.println(curr_events[i].level); }
    analogWrite(pins[i],map(curr_events[i].level,0,100,0,255));
    //if(curr_events[i].level == ON) { digitalWrite(pins[i],HIGH); }
    //if(curr_events[i].level == OFF) { digitalWrite(pins[i],LOW); }
  }
}

void loop() {

/*
  rc1Check();
  Serial.println(v_rc1);
  Serial.println(IS_RC_HIGH(v_rc1));
  Serial.println(IS_RC_MID(v_rc1));
  Serial.println(IS_RC_LOW(v_rc1));
  delay(300);
  return;
*/
  static unsigned long last_millis = 0;
  // Wait for next ms.
  while(last_millis == millis());
  last_millis = millis();

  rc1Check();

  // Loop the loop :-)
  for(int i=0;i<OUTPUTS;i++) {
    // Next Event
    if(curr_events[i].ms == 0) {
      next_event(i);
    } else {
      // Countdown
      curr_events[i].ms--;
    }
  }
}
