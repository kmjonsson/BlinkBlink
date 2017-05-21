#ifndef BLINKBLINK_H
#define BLINKBLINK_H

// Uncomment this for use with version #1
//#define V1
// Uncomment this for use with version #5
#define V5

#ifdef V5
#define RCPIN A0
#else
#define RCPIN A1
#endif

#define INIT_MSG "BlinkBlink v0.3 ? = help, d = dump"

#define DEBUG 0
#define DEBUG_CH 7

#define MAX_EVENTS 100

#define OUTPUTS 8

#define ON      255
#define OFF     0

#define NONE     0x0000

#define RC_LOW  0x0001
#define RC_MID  0x0002
#define RC_HIGH 0x0004
#define RC_NA   0x0008

#define EOE     0xFFFF

#define B10 0x01 

typedef struct _event {
  uint16_t ms;
  uint8_t  level;
  uint16_t flags;
} event;

event blank[] = {
  { 0, OFF, EOE }
};

#ifdef V5
uint8_t pins[OUTPUTS] = { 9,6,5,3,10,11,12,13 };
#else
uint8_t pins[OUTPUTS] = { 3,5,6,9,10,11,12,13 };
#endif

#define RCMID 1500
#define RCOFFSET 300

#define IS_RC_HIGH(x) ((x) >= RCMID+RCOFFSET)
#define IS_RC_MID(x) ((x) > RCMID-RCOFFSET && (x) < RCMID+RCOFFSET)
#define IS_RC_LOW(x) ((x) < RCMID-RCOFFSET && (x) > 0)
#define IS_RC_NA(x) ((x) == 0)

#endif
