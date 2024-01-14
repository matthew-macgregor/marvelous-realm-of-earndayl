#ifndef CONNECTORS_DATA_TEST_H
#define CONNECTORS_DATA_TEST_H
#include "connectors.h"
#include "entry_ids_test.h"

static Connector connectors[] = {
    { E_ENTRY_CAVE, EAST, E_EASTERN_PASSAGE },
    { E_EASTERN_PASSAGE, WEST, E_ENTRY_CAVE },
    { E_ENTRY_CAVE, WEST, E_WESTERN_PASSAGE },
    { E_WESTERN_PASSAGE, EAST, E_ENTRY_CAVE }
};

#define CONNECTOR_COUNT (long)(sizeof connectors / sizeof *connectors)

#endif
