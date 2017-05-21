// Copyright 2012 Eric Liang

#ifndef BWT_H_
#define BWT_H_

#include <assert.h>
#include <inttypes.h>
#include <memory.h>

#include "suffix_array.h"
#include "sorts.h"

using std::string;

string bwt(string &input);
string *ibwt(string *L);

#endif  // BWT_H_
