#include <avr/wdt.h>

#include "PinChangeInterrupt.h"

#include "BlinkBlink.h"
#include "BlinkBlinkSeq.h"

// Code...

volatile unsigned long rcstamp=0;
volatile uint16_t v_rc=0;

void rcCheck() {
  unsigned long m = micros();
  if(m - rcstamp > 100000) {
    v_rc=0;
  }  
}

void rcCalc() {
  unsigned long m = micros();
  if(digitalRead(RCPIN)) {
    rcstamp = m;
  } else {
    if(m - rcstamp > 2500 || m-rcstamp < 900) {
      v_rc=0;
    } else {
      v_rc = m-rcstamp;
    }
  }
}

event curr_events[OUTPUTS];

void setup() {
  Serial.begin(57600);
  Serial.println(INIT_MSG);
  for(int i=0;i<OUTPUTS;i++) {
    pinMode(pins[i], OUTPUT);
    memcpy(&curr_events[i],&blank,sizeof(event));
  }
  pinMode(RCPIN,INPUT);  
  attachPCINT(digitalPinToPCINT(RCPIN), rcCalc, CHANGE);
  delay(500);
  wdt_enable(WDTO_2S);
}

void next_event(int i) {
  static int16_t curr_event[OUTPUTS] = { -2, -2, -2, -2, -2, -2, -2, -2 };

  uint16_t rc = v_rc;

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

    
    if((events[i][curr_event[i]].flags & RC_HIGH) == RC_HIGH && !IS_RC_HIGH(rc)) { continue; }
    if((events[i][curr_event[i]].flags & RC_MID)  == RC_MID  && !IS_RC_MID(rc))  { continue; }
    if((events[i][curr_event[i]].flags & RC_LOW)  == RC_LOW  && !IS_RC_LOW(rc))  { continue; }
    if((events[i][curr_event[i]].flags & RC_NA)   == RC_NA   && !IS_RC_NA(rc))   { continue; }
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
    analogWrite(pins[i],map(curr_events[i].level,OFF,ON,0,255));
    //if(curr_events[i].level == ON) { digitalWrite(pins[i],HIGH); }
    //if(curr_events[i].level == OFF) { digitalWrite(pins[i],LOW); }
  }
}

void dumpCfg(void) {
  Serial.print("// OUTPUTS = "); Serial.println(OUTPUTS);

  for(int i=0;i<OUTPUTS;i++) {    
    Serial.print("event ch"); Serial.print(i+1); Serial.println("event[] = {");
    for(int j=0;events[i][j].flags != EOE;j++) {
      Serial.print("    { "); Serial.print(events[i][j].ms); Serial.print(", ");
      if(events[i][j].level == OFF) {
        Serial.print("OFF");
      } 
      if(events[i][j].level == ON) {
        Serial.print("ON");
      } 
      if(events[i][j].level != ON && events[i][j].level != OFF) {
        Serial.print(events[i][j].level);        
      } 
      Serial.print(", ");
      char p = 0;
      if(events[i][j].flags == NONE) {
        Serial.print("NONE");
        p = 1;
      }
      if(events[i][j].flags & RC_HIGH) {
        if(p) Serial.print("|");
        Serial.print("RC_HIGH");       
      }
      if(events[i][j].flags & RC_MID) {
        if(p) Serial.print("|");
        Serial.print("RC_MID");       
      }
      if(events[i][j].flags & RC_LOW) {
        if(p) Serial.print("|");
        Serial.print("RC_LOW");       
      }
      if(events[i][j].flags & RC_NA) {
        if(p) Serial.print("|");
        Serial.print("RC_NA");       
      }
      Serial.println("},");      
    }
    Serial.println("    { 0   , OFF, EOE },\n};");
  }
  Serial.println("");
  Serial.println("event *events[OUTPUTS] = {");
  for(int i=0;i<OUTPUTS;i++) {        
    Serial.print("  ch"); Serial.print(i+1); Serial.println("event,");    
  }
  Serial.println("};");
}

void loop() {
  static unsigned long last_millis = 0;
  // Wait for next ms.
  while(last_millis == millis()) { wdt_reset(); }
  last_millis = millis();

  rcCheck();

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
  
  // Serial receiver.
  if(Serial.available()) {
    char c = Serial.read();
    // Print init/help msg.
    if(c == '?') {
      Serial.println(INIT_MSG);
    }
    // Dump the config
    if(c == 'd') {
      dumpCfg();
    }
  }
}
