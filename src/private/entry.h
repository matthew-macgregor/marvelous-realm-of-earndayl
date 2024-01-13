#ifndef PRIVATE_ENTRY_H
#define PRIVATE_ENTRY_H

struct Entry {
   const char *description;
   const char *traits;
   long exits[8]; // N/E/S/W NE/NW/SE/SW
};

#endif // PRIVATE_ENTRY_H
