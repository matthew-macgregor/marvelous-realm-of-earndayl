#ifndef GAME_DATA_CONNECTORS_H
#define GAME_DATA_CONNECTORS_H
#include "connectors.h"
#include "entry_ids.h"

static Connector connectors[] = {
    { E_ENTRY_CAVE, EAST, E_EASTERN_PASSAGE },
    { E_EASTERN_PASSAGE, WEST, E_ENTRY_CAVE },
    { E_ENTRY_CAVE, EAST, E_WESTERN_PASSAGE },
    { E_WESTERN_PASSAGE, WEST, E_ENTRY_CAVE }
};

#define CONNECTOR_COUNT (long)(sizeof connectors / sizeof *connectors)

#endif
