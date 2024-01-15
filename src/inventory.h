#ifndef INVENTORY_H
#define INVENTORY_H
#include "game/game_data.h"
#include "objects.h"

#define inv_get_inventory_entry() EP_INVENTORY

bool inv_add_object_to_inventory(Object *obj);
bool inv_remove_object_from_inventory(Object *obj, Entry *entry);

#ifdef TEST
int inventory_test_main(void);
#endif // TEST
#endif // INVENTORY_H
