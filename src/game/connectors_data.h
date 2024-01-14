#ifndef GAMEP_DATA_CONNECTORS_H
#define GAMEP_DATA_CONNECTORS_H
#include "connectors.h"
#include "game/entries_data.h"

static Connector connectors[] = {
    { EP_ENTRY_CAVE, EAST, EP_EASTERN_PASSAGE },
    { EP_EASTERN_PASSAGE, WEST, EP_ENTRY_CAVE },
    { EP_ENTRY_CAVE, WEST, EP_WESTERN_PASSAGE },
    { EP_WESTERN_PASSAGE, EAST, EP_ENTRY_CAVE }
};

#define CONNECTOR_COUNT (long)(sizeof connectors / sizeof *connectors)

#endif
