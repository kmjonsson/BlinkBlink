#ifndef BLINKBLINKSEQ_H
#define BLINKBLINKSEQ_H

/*
 * Event:
 *      Tid (i ms)
 *      Nivå (0-100%), ON, OFF
 *      Flaggor:  NONE     = Inget speciellt
 *                RC1_LOW  = om RC1-ingång låg kör detta event.
 *                RC1_MID  = om RC1-ingång mellan kör detta event.
 *                RC1_HIGH = om RC1-ingång Hög kör detta event.
 *                EOE      = Slut på serie.
 *                
 * ch1 - ch6 -- har möjlighet att sätta nivå. 
 * ch7 - ch8 -- har enbart på & av (ON/OFF).
 * 
 */

event ch1event[] = {
    /*{ 100,  ON,  NONE },
    { 1000, OFF, NONE },
    { 100,  ON,  NONE },
    { 100,  OFF, NONE },
    { 100,  ON,  NONE },*/
    { 1000, OFF, NONE },
    { 0   , OFF, EOE  },
};

event ch2event[] = {
  /*
    { 100, OFF,  NONE },
    { 100, 10,  NONE },
    { 100, 20,  NONE },
    { 100, 30,  NONE },
    { 100, 40,  NONE },
    { 100, 50,  NONE },
    { 100, 60,  NONE },
    { 100, 70,  NONE },
    { 100, 80,  NONE },
    { 100, 90,  NONE },
    { 100, ON,  NONE },*/
    { 1000, OFF, NONE },
    { 0   , OFF, EOE  },
};

event ch3event[] = {
    //{ 1000, 50,  NONE },
    { 1000, OFF, NONE },
    { 0   , OFF, EOE },
};

event ch4event[] = {
    //{ 333, ON,  NONE },
    //{ 333, OFF, NONE },
    { 1000, OFF, NONE },
    { 0   , EOE },
};

event ch5event[] = {
    //{ 666, ON,  NONE },
    //{ 666, OFF, NONE },
    { 1000, OFF, NONE },
    { 0  , OFF, EOE },
};

event ch6event[] = {
    //{ 1000, ON,  NONE },
    //{ 1000, OFF, NONE },
    { 1000, OFF, NONE },
    { 0   , OFF, EOE },
};

event ch7event[] = {
    //{ 1333, ON, NONE },
    //{ 1333, OFF, NONE },
    { 1000, OFF, NONE },
    { 0   , OFF, EOE },
};

event ch8event[] = {
    { 500,  ON,  RC1_LOW },
    { 501,  ON,  RC1_LOW },
    { 502,  ON,  RC1_LOW },
    { 503,  ON,  RC1_LOW },
    { 104,   ON,  RC1_MID },
    { 105,   OFF,  RC1_MID },
    { 1006,  OFF,  RC1_HIGH },
    { 0   , OFF, EOE },
};

#endif
