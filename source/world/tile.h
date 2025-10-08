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
typedef BASIC__u8 TILE__height;
typedef BASIC__u16 TILE__factory_count;
typedef BASIC__u16 TILE__fortification_level;
typedef BASIC__u32 TILE__population;

// tile
typedef struct TILE__tile {
    // basic terrain
    TILE__features basic_terrain;
    TILE__height height;

    // population data
    TILE__population population;

    // production data
    TILE__factory_count factory_count;

    // military data
    TILE__fortification_level fort_level;
} TILE__tile;

#endif
