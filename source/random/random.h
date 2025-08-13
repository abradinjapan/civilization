#ifndef CIV__random
#define CIV__random

/* Include */
// basic
#include "../basic.h"

/* Define */
// seed type
typedef BASIC__u64 RANDOM__seed;
typedef BASIC__u64 RANDOM__iteration;
typedef BASIC__u64 RANDOM__number;

/* Context */
// context for one random generator
typedef struct RANDOM__context {
    RANDOM__seed seed;
    RANDOM__iteration iteration;
} RANDOM__context;

// create custom context
RANDOM__context RANDOM__create__context(RANDOM__seed seed, RANDOM__iteration iteration) {
    RANDOM__context output;

    // setup output
    output.seed = seed;
    output.iteration = iteration;

    return output;
}

// create null context
RANDOM__context RANDOM__create_null__context() {
    return RANDOM__create__context(0, 0);
}

/* Generators */
// generate one number from a specific seed and iteration
RANDOM__number RANDOM__generate_number__mark_1(RANDOM__context* context, RANDOM__iteration iteration) {
    return (*context).seed * iteration * 987654321;
}

// generate one random number and advance the iteration by 1
RANDOM__number RANDOM__generate_number_and_advance__mark_1(RANDOM__context* context) {
    RANDOM__number output;

    // generate number
    output = RANDOM__generate_number__mark_1(context, (*context).iteration);

    // increment iteration
    (*context).iteration++;

    return output;
}

#endif
