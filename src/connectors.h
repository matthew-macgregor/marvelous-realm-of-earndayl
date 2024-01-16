#ifndef CONNECTORS_H
#define CONNECTORS_H
#include "directions.h"
#include "entry.h"

typedef struct Connector {
    const Entry *entry_a;
    const Direction direction;
    const Entry *entry_b;
} Connector;
typedef Connector* ConnectorArrayPtr;

size_t connector_get_connector_count(void);
ConnectorArrayPtr connector_get_connectors(void);
Connector *connector_get_connector_in_direction(const Entry *current_loc, const Direction direction);
entry_id connector_get_location_id_in_direction(const Entry *current_loc, const Direction direction);

/**
 * Notes
 * -----
 * A Connector is defined (currently) as a one-way link between two locations, 
 * explicitly defined with a direction.
 * 
 * It has the shape:
 * 
 *  Location1 -> WEST -> Location2
 * 
 * Despite a temptation to define variants, such as bi-directional, I decided 
 * that this added unnecessary complexity when some duplication will solve it
 * (and ultimately this code will be generated).
 *
 */

#ifdef TEST
int connectors_test_main(void);
#endif // TEST
#endif // CONNECTIONS_H
