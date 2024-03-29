#ifndef INVENTORY_H
#define INVENTORY_H
#include "game/game_data.h"

#define inv_get_inventory_entry() EP_INVENTORY
#define inv_contains_entry(e) ((e)->location == EP_INVENTORY)
bool inv_add_object_to_inventory(Entry *obj);
bool inv_remove_object_from_inventory(Entry *obj, Entry *entry);
int inv_get_encumbrance(void);

#ifdef TEST
int inventory_test_main(void);
#endif // TEST
#endif // INVENTORY_H
