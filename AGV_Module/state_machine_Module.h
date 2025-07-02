
#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

typedef enum {
    STATE_STARTUP,
    STATE_RIJDEN,
    STATE_CHECK_BLOK,
    STATE_DRAAIEN,
    STATE_NOODSTOP
} State;

extern volatile State huidige_state;

#endif
