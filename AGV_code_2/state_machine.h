
#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

typedef enum {
    STATE_STARTUP,
    STATE_RIJDEN,
    STATE_DRAAIEN,
    STATE_STOPPEN,
    STATE_SCHUIF_RECHTS,
    STATE_SCHUIF_LINKS,
    STATE_NOODSTOP
} State;

extern volatile State huidige_state;

#endif
