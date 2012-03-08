#ifndef SUFFIX_ARR_H
#define SUFFIX_ARR_H

#include <inttypes.h>
#include <algorithm>
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

inline bool LE(uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t f) {
    if (a < d) {
        return true;
    }
    if (a == d) {
        if (b < e) {
            return true;
        }
        if (b == e) {
            return c <= f;
        }
    }
    return false;
}

/* go from triplets to rank array */
uint32_t *ranks(triplet *T, size_t sz, size_t *retsz, bool *unique) {
    /* remember old positions */
    for (size_t i=0; i < sz; i++) {
        T[i].label = i;
    }
    vprint("T", T, sz);
    lsd_sort(T, sz);
    vprint("Tsort", T, sz);

    uint32_t *ranks = new uint32_t[sz + 2];
    *retsz = sz;
    uint32_t cur_rank = 0;
    triplet prev = triplet(999999,999999,999999);
    *unique = true;
    for (size_t i=0; i < sz; i++) {
        if (prev != T[i]) {
            cur_rank += 1;
            prev = T[i];
        } else {
            *unique = false;
        }
        ranks[T[i].label] = cur_rank;
    }
    return ranks;
}

uint32_t *toSA(uint32_t *rankedLabels, size_t sz, size_t *retsz) {
    uint32_t *SA = new uint32_t[*retsz = sz + 1];
    SA[0] = sz;
    for (size_t i=0; i < sz; i++) {
        SA[rankedLabels[i]] = i;
    }
    return SA;
}

uint32_t *idc3(uint32_t *T, size_t n, size_t *retsz) {
    vprint("args", T, n);
    T[n++] = 0;

    // step 0: construct a sample
    size_t B0len = (n+1)/3;
    #define toC(i) ((i < B0len) ? (1 + 3*i) : (2 + 3*(i-B0len)))

    // step 1: sort sample suffixes
    triplet *R = new triplet[n * 3 / 2 + 3];
    triplet *Rptr = R;
    for (int j=1; j <= 2; j++) {
        size_t i = j;
        for (; i < n - 2; i += 3) {
            *Rptr++ = triplet(T[i], T[i+1], T[i+2]);
        }
        for (; i < n - 1; i += 3) {
            *Rptr++ = triplet(T[i], T[i+1], 0);
        }
        for (; i < n; i += 3) {
            *Rptr++ = triplet(T[i], 0, 0);
        }
    }
    bool unique = false;
    size_t Rsz = Rptr - R;
    size_t Rrsz, SARsz;
    uint32_t *Rr = ranks(R, Rsz, &Rrsz, &unique);
    vprint("Rr", Rr, Rrsz)
    delete[] R;
    uint32_t *SAR = unique ? toSA(Rr, Rrsz, &SARsz) : idc3(Rr, Rrsz, &SARsz);
    vprint("SAR", SAR, SARsz)
    delete[] Rr;
    uint32_t *rank = new uint32_t[n+2];
    for (size_t i=1; i < SARsz; i++) {
        rank[toC(SAR[i])] = i;
    }

    // step 2: sort nonsample suffixes
    triplet *SB0 = new triplet[n/3 + 3];
    triplet *SB0ptr = SB0;
    for (size_t i=0; i < n; i += 3) {
        *SB0ptr++ = triplet(T[i], rank[i+1], i);
    }
    size_t SB0sz = SB0ptr - SB0;
    lsd_sort(SB0, SB0sz);

    // step 3: merge
    uint32_t *Sc = new uint32_t[SARsz - 1];
    size_t Scsz = SARsz - 1;
    uint32_t *buf = new uint32_t[n + 1];
    uint32_t *buf_ptr = buf;
    for (size_t i=1; i < SARsz; i++) {
        Sc[i-1] = toC(SAR[i]);
    }
    delete[] SAR;
    size_t sbi = 0;
    size_t sci = 0;
    while (sbi < SB0sz && sci < Scsz) {
        uint32_t i = Sc[sci];
        uint32_t j = SB0[sbi].arr[2];
        if (i % 3 == 1) {
            if (T[i] < T[j] || (T[i] == T[j] && rank[i+1] <= rank[j+1])) {
                *buf_ptr++ = i;
                sci++;
            } else {
                *buf_ptr++ = j;
                sbi++;
            }
        } else if (i % 3 == 2) {
            if (LE(T[i], T[i+1], rank[i+2], T[j], T[j+1], rank[j+2])) {
                *buf_ptr++ = i;
                sci++;
            } else {
                *buf_ptr++ = j;
                sbi++;
            }
        } else {
            assert (false);
        }
    }
    while (sci < Scsz) {
        uint32_t i = Sc[sci++];
        *buf_ptr++ = i;
    }
    while (sbi < SB0sz) {
        uint32_t j = SB0[sbi++].arr[2];
        *buf_ptr++ = j;
    }
    delete[] Sc;
    delete[] SB0;
    delete[] rank;
    *retsz = buf_ptr - buf;
    return buf;
}

/* go from string to rank array */
uint32_t *ranks(const char *T, size_t sz) {
    char *arr = new char[sz];
    char_sort_to(T, sz, arr);

    uint32_t *rank = new uint32_t[256];
    char prev = '\0';
    uint32_t cur_rank = 1;
    for (size_t i=0; i < sz; i++) {
        if (arr[i] != prev) {
            prev = arr[i];
            rank[(uint32_t)arr[i]] = cur_rank;
            cur_rank += 1;
        }
    }

    uint32_t *rr = new uint32_t[sz + 3];
    rr[sz] = 0;
    rr[sz+1] = 0;
    rr[sz+2] = 0;
    uint32_t *rrptr = rr;
    for (size_t i=0; i < sz; i++) {
        *rrptr++ = rank[(uint8_t)T[i]];
    }

    delete[] arr;
    return rr;
}

uint32_t *gen_suffix_array(string *input, size_t *retsz) {
    uint32_t *v = ranks(input->c_str(), input->length());
    uint32_t *sa = idc3(v, input->length(), retsz);
    delete[] v;
    return sa;
}

#endif
