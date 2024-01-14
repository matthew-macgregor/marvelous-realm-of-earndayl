#include "game/game_data.h"
#include "connectors.h"

extern Connector *connector_get_connector_in_direction(const Entry *current_loc, const Direction direction) {
    size_t conn_cnt = connector_get_connector_count();
    for (size_t i = 0; i < conn_cnt; i++) {
        Connector *conn = &connectors[i];
        if (conn->entry_a == current_loc && conn->direction == direction) {
            return conn;
        }
    }

    return NULL;
}

location_id connector_get_location_id_in_direction(const Entry *current_loc, const Direction direction) {
    Connector *conn = connector_get_connector_in_direction(current_loc, direction);
    return conn == NULL             ?   LOCATION_UNKNOWN :
           conn->entry_b == NULL    ?   LOCATION_UNKNOWN :
                                        conn->entry_b->location_id;
}

#ifdef TEST
#include "colors.h"
#include "minunit.h"
#include "commands.h"
#include "test_results.h"

static char *test_connector_get_location_in_direction(void) {
    mu_assert("connector_get_location_in_direction", 
        connector_get_location_id_in_direction(EP_ENTRY_CAVE, WEST) == E_WESTERN_PASSAGE);
    return 0;
}

static char *connectors_test_all_tests(void) {
    mu_run_test(test_connector_get_location_in_direction);

    // next test here
    return 0;
}

int connectors_test_main(void) {
    const char *result = connectors_test_all_tests();
    return report_test_results("Connectors", result);
}
#endif // TEST
