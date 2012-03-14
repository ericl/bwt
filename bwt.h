#ifndef BWT_T
#define BWT_T

#include <assert.h>
#include <inttypes.h>
#include <memory.h>
#include "suffix_array.h"
#include "sorts.h"

using namespace std;

string *bwt(string *input);
string *ibwt(string *L);

#endif
