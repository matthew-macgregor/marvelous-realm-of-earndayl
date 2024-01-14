#ifndef DIRECTIONS_H
#define DIRECTIONS_H

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
