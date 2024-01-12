#ifndef ROOMS_H
#define ROOMS_H

#include <stdlib.h>
#include <string.h>
#include "output.h"

struct Room {
   const char *description;
   const char *traits;
} rooms[] = {
   {"a narrow cave with wet walls", "start"},
   {"the eastern passage", "cave"},
   {"the western passage", "cave"}
};

#define room_count   (sizeof rooms / sizeof *rooms)
#endif