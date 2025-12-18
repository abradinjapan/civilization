#ifndef CIV__world
#define CIV__world

/* Include */
// tile
#include "tile.h"

/* Define */
// world dimensions
typedef BASIC__u64 WORLD__tile_axis;
typedef WORLD__tile_axis WORLD__tile_x;
typedef WORLD__tile_axis WORLD__tile_y;
#define WORLD__tile_axis__null 0

/* World */
// game world
typedef struct WORLD__world {
    // world dimensions by tiles
    WORLD__tile_x width;
    WORLD__tile_y height;

    // world tile buffer
    BASIC__buffer tile_allocation;
} WORLD__world;

// create game world
WORLD__world WORLD__create__world(WORLD__tile_x width, WORLD__tile_y height, BASIC__buffer tile_allocation) {
    WORLD__world output;

    // setup values
    output.width = width;
    output.height = height;
    output.tile_allocation = tile_allocation;

    return output;
}

// create null world
WORLD__world WORLD__create_null__world() {
    return WORLD__create__world(WORLD__tile_axis__null, WORLD__tile_axis__null, BASIC__create_null__buffer());
}

#endif
