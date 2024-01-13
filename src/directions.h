#ifndef DIRECTIONS_H
#define DIRECTIONS_H

typedef unsigned short int bp_direction;
typedef enum BitpackDirection {
    BP_NONE         = 0,
    BP_NORTH        = 1,
    BP_EAST         = (BP_NORTH << 1),
    BP_SOUTH        = (BP_NORTH << 2),
    BP_WEST         = (BP_NORTH << 3),
    BP_NORTHEAST    = (BP_NORTH << 4),
    BP_NORTHWEST    = (BP_NORTH << 5),
    BP_SOUTHEAST    = (BP_NORTH << 6),
    BP_SOUTHWEST    = (BP_NORTH << 7)
} BitpackDirection;

typedef enum Direction {
    DIRECTION_UNKNOWN = -1,
    NORTH = 0,
    EAST,
    SOUTH,
    WEST,
    NORTHEAST,
    NORTHWEST,
    SOUTHEAST,
    SOUTHWEST
} Direction;

extern Direction direction_text_to_direction(const char *direction);
extern const char* direction_to_text(const Direction d);
extern const char* direction_to_abbr(const Direction d);
#define is_valid_direction(x) ((x) > 0 && (x) < 8)

#ifdef TEST
int directions_test_main(void);
#endif // TEST
#endif // DIRECTIONS_H
