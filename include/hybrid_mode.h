#ifndef HYBRID_MODE_H
#define HYBRID_MODE_H

/*
* File:        hybrid_mode.h
* Description: TBC
*/

// Includes
#include <project.h>
#include <include/can_comm.h>

// Macros, typedefs, and structs
#define FREEWHEEL_MODE 0u
#define TORQUE_MODE 2u
#define SPEED_MODE 3u
#define BRAKE_MODE 8u

typedef struct {
    uint8_t driving_mode;
    uint8_t duty_cycle;
} HY_output_t;

// Function declarations

#endif /* HYBRID_MODE_H */
/* [] */