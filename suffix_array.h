// Copyright 2012 Eric Liang

#ifndef SUFFIX_ARRAY_H_
#define SUFFIX_ARRAY_H_

#include <assert.h>
#include <inttypes.h>

#include <algorithm>
#include <string>

#include "sorts.h"

#define DC3_MIN_CHARS 5
// #define DEBUG
#ifdef DEBUG
#define vprint(x, v, sz)\
    cout << x << " ";\
    for (size_t i = 0; i < sz; i++) {\
        cout << v[i] << " ";\
    }\
    cout << endl;
#else
#define vprint(x, v, sz)
#endif

uint32_t *gen_suffix_array(string *input, size_t *retsz);

#endif  // SUFFIX_ARRAY_H_
