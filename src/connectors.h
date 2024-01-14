#ifndef CONNECTORS_H
#define CONNECTORS_H
#include "directions.h"
#include "location.h"
#include "entry.h"

typedef struct Connector {
    const location_id location_a;
    const Direction direction;
    const location_id location_b;
} Connector;

Connector *connector_get_connector_in_direction(const location_id current_loc, const Direction direction);
location_id connector_get_location_in_direction(const location_id current_loc, const Direction direction);

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
 * I also opted for an id-based lookup for all objects rather than a more
 * efficient pointer-based one. I like the idea that these objects are
 * trivially serializeable, but I'm still on the fence.
 */

#ifdef TEST
int connectors_test_main(void);
#endif // TEST
#endif // CONNECTIONS_H
