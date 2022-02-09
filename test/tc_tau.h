#ifndef TC_TAU_H
#define TC_TAU_H

#define TC_TAU_MAX_LEN 1024

struct tc_tau {
    const char * r1[TC_TAU_MAX_LEN];
    const char * r2[TC_TAU_MAX_LEN];
    int n;
    double tau;
};

struct tc_tau tc_tau[] = {
    {
        .r1 = { "a", "b", "c", "d", "e", "f", "g", "h", "i", "j" },
        .r2 = { "a", "b", "c", "d", "e", "f", "g", "h", "i", "j" },
        .n = 10,
        .tau = 1.0
    },
    {
        .r1 = { "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", 
            "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", 
            "w", "x", "y", "z"},
        .r2 = { "g", "n", "u", "q", "c", "y", "m", "d", "k", "p", "w", 
            "l", "j", "x", "f", "r", "h", "a", "b", "z", "e", "t", "v", 
            "i", "o", "s"},
        .n = 26,
        .tau = 0.015384
    },
    {
        /* non-conjoint */
        .r1 = {"a", "b", "c", "d", "e"},
        .r2 = {"a", "c", "d", "e", "f"},   
        .n = 5,
        .tau = TAU_NONCONJOINT_ERROR
    }
};

#endif /* TC_TAU_H */
