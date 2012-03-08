#ifndef BWT_T
#define BWT_T

#include <assert.h>
#include <inttypes.h>
#include <memory.h>
#include "suffix_array.h"
#include "sorts.h"

using namespace std;

string *bwt(string *input) {
    size_t sz;
    uint32_t *sa = gen_suffix_array(input, &sz);

    /* construct bwt from suffix array */
    char *bwt = new char[input->length()];
    char *orig = bwt;
    for (size_t i=1; i <= input->length(); i++) {
        if (sa[i] > 0) {
            *bwt++ = input->at(sa[i] - 1);
        } else {
            *bwt++ = input->at(input->length() - 1);
        }
    }

    return new string(orig, bwt - orig);
}

string *ibwt(string *L) {
    char *F = new char[L->length()];
    uint32_t *C = new uint32_t[L->length()];
    char_sort_to(L->c_str(), L->length(), F);

    /* marks first occurrence of char in F */
    int32_t M[256];
    memset(M, 0xff, sizeof(int32_t)*256);
    for (size_t i=0; i < L->length(); i++) {
        uint8_t k = F[i];
        if (M[k] < 0) {
            M[k] = i;
        }
    }

    /* marks number of occurences of L[j] in L for all j */
    uint32_t tmp[256];
    memset(tmp, 0, sizeof(int32_t)*256);
    memset(C, 0, sizeof(uint32_t)*L->length());
    for (size_t i=0; i < L->length(); i++) {
        uint8_t k = (*L)[i];
        C[i] = tmp[k]++;
    }

    /* compute ibwt backwards */
    char *buf = new char[L->length()];
    char *ptr = buf + L->length() - 1;
    *ptr-- = F[0];
    uint32_t j = 0;
    for (size_t i=1; i < L->length(); i++) {
        *ptr-- = (*L)[j];
        j = M[(uint8_t)(*L)[j]] + C[j];
    }

    delete[] F;
    delete[] C;
    string *s = new string(buf, L->length());
    delete[] buf;
    return s;
}

#endif