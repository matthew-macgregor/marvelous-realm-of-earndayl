#ifndef CONNECTIONS_H
#define CONNECTIONS_H
#include "directions.h"
#include "location.h"
#include "entry.h"

typedef struct Connector {
    const location_id location_a;
    const Direction direction;
    const location_id location_b;
} Connector;

Connector *connector_get_connector_in_direction(location_id current_loc, Direction direction);
location_id connector_get_location_in_direction(location_id current_loc, const Direction direction);

#ifdef TEST
int connectors_test_main(void);
#endif // TEST
#endif // CONNECTIONS_H
