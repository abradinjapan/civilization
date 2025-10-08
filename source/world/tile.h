#ifndef CIV__tile
#define CIV__tile

/* Includes */
// basic types
#include "../basic.h"

/* Tile */
// tile options
typedef enum TILE__to {
    // foundational types
    TILE__to__invalid = 0x0,
    TILE__to__impassable = 0x1,
    TILE__to__ocean = 0x2,
    TILE__to__land = 0x3,

    // land types
    TILE__to__land__arctic = 0x0 << 2,
    TILE__to__land__tundra = 0x1 << 2,
    TILE__to__land__grassy = 0x2 << 2,
    TILE__to__land__desert = 0x3 << 2,

    // terrain features
    TILE__to__land__features__open_sky = 0x0 << 4,
    TILE__to__land__features__marsh = 0x1 << 4,
    TILE__to__land__features__forest = 0x2 << 4,
    TILE__to__land__features__jungle = 0x3 << 4,
} TILE__to;

// tile features
typedef BASIC__u8 TILE__features;
typedef BASIC__s8 TILE__height;
typedef BASIC__u16 TILE__factory_count;
typedef BASIC__u16 TILE__fort_level;
typedef BASIC__u32 TILE__population;
typedef BASIC__u32 TILE__culture;

// other tile defines
#define TILE__height__ocean_height 0
#define TILE__culture__invalid 0

// tile
typedef struct TILE__tile {
    // basic terrain
    TILE__features basic_terrain;
    TILE__height height;

    // people data
    TILE__population population;
    TILE__culture culture;

    // production data
    TILE__factory_count factory_count;

    // military data
    TILE__fort_level fort_level;
} TILE__tile;

// create tile
TILE__tile TILE__create__tile(TILE__features basic_terrain, TILE__height height, TILE__population population, TILE__culture culture, TILE__factory_count factory_count, TILE__fort_level fort_level) {
    TILE__tile output;

    // setup output
    output.basic_terrain = basic_terrain;
    output.height = height;
    output.population = population;
    output.culture = culture;
    output.factory_count = factory_count;
    output.fort_level = fort_level;

    return output;
}

// create null tile
TILE__tile TILE__create_null__tile() {
    return TILE__create__tile(TILE__to__invalid | TILE__to__land__arctic | TILE__to__land__features__open_sky, TILE__height__ocean_height, 0, TILE__culture__invalid, 0, 0);
}

#endif
