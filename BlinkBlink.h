#ifndef BLINKBLINK_H
#define BLINKBLINK_H

#define DEBUG 1
#define DEBUG_CH 7

#define MAX_EVENTS 100

#define OUTPUTS 8

#define ON      100
#define OFF     0

#define NONE     0x0000

#define RC1_LOW  0x0001
#define RC1_MID  0x0002
#define RC1_HIGH 0x0004

#define RC2_LOW  0x0008
#define RC2_MID  0x0010
#define RC2_HIGH 0x0020

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

uint8_t pins[OUTPUTS] = { 3,5,6,9,10,11,12,13 };

#define RCMID 1500
#define RCOFFSET 300

#define IS_RC_HIGH(x) ((x) >= RCMID+RCOFFSET)
#define IS_RC_MID(x) ((x) > RCMID-RCOFFSET && (x) < RCMID+RCOFFSET)
#define IS_RC_LOW(x) ((x) < RCMID-RCOFFSET)

#endif
