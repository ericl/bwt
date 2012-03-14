#ifndef SUFFIX_ARR_H
#define SUFFIX_ARR_H

#include <inttypes.h>
#include <algorithm>
#include <assert.h>
#include "sorts.h"

using namespace std;

#define DC3_MIN_CHARS 5
//#define DEBUG
#ifdef DEBUG
#define vprint(x,v,sz)\
    cout << x << " ";\
    for (size_t i=0; i < sz; i++) {\
        cout << v[i] << " ";\
    }\
    cout << endl;
#else
#define vprint(x,v,sz)
#endif

uint32_t *gen_suffix_array(string *input, size_t *retsz);

#endif
