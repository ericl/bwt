#include "sorts.h"

using namespace std;

triplet::triplet(uint32_t a, uint32_t b, uint32_t c) {
    arr[0] = a;
    arr[1] = b;
    arr[2] = c;
};

bool
triplet::operator<(const triplet r) const {
    if (arr[0] < r.arr[0]) {
        return true;
    } else if (arr[0] == r.arr[0]) {
        if (arr[1] < r.arr[1]) {
            return true;
        } else if (arr[1] == r.arr[1]) {
            return arr[2] < r.arr[2];
        }
    }
    return false;
}

bool triplet::operator==(const triplet r) const {
    return arr[0] == r.arr[0] && arr[1] == r.arr[1] && arr[2] == r.arr[2];
}

bool
triplet::operator!=(const triplet r) const {
    return arr[0] != r.arr[0] || arr[1] != r.arr[1] || arr[2] != r.arr[2];
}

ostream&
operator<<(ostream &out, const triplet&x) {
    out << "(" << x.arr[0] << ", " << x.arr[1] << ", " << x.arr[2] << ")";
    return out;
}

void char_sort_to(const char *base, size_t sz, char *output) {
    uint32_t buckets[256];
    memset(buckets, 0, sizeof(uint32_t)*256);
    size_t i = 0;
    for (; i + 4 < sz; i += 4) {
        const char *ptr = base + i;
        buckets[(uint8_t)ptr[0]]++;
        buckets[(uint8_t)ptr[1]]++;
        buckets[(uint8_t)ptr[2]]++;
        buckets[(uint8_t)ptr[3]]++;
    }
    for (; i < sz; i++) {
        const char *ptr = base + i;
        buckets[(uint8_t)ptr[0]]++;
    }
    for (size_t i=0; i < 256; i++) {
        memset(output, (char)i, buckets[i]);
        output += buckets[i];
    }
}

bool all_zeros(triplet *T, size_t sz, int pass) {
    for (size_t i=0; i < sz; i++) {
        uint8_t *arr = (uint8_t*)T[i].arr;
        if (arr[pass] != 0) {
            return false;
        }
    }
    return true;
}

void lsd_sort(triplet *T, size_t sz) {
    int passes[] = {8,9,10,11,4,5,6,7,0,1,2,3};
    triplet *old = T;
    triplet *tmp = new triplet[sz];

    // precompute bucket sizes for good cache locality
    uint32_t buckets[256 * 12];
    memset(buckets, 0, sizeof(uint32_t) * 256 * 12);
    for (size_t i=0; i < sz; i++) {
        uint8_t *arr = (uint8_t*)T[i].arr;
        for (int p=0; p < 12; p++) {
            buckets[(p << 8) + arr[p]]++;
        }
    }

    for (int p=0; p < 12; p++) {
        int col = passes[p];

        // set up bucket pointers
        triplet* bptrs[256];
        triplet *U = tmp;
        bool allsame = false;
        for (int i=0; i < 256; i++) {
            uint32_t *base = buckets + (col << 8);
            uint32_t count = base[i];
            if (count == sz) {
                allsame = true;
                break;
            }
            bptrs[i] = U;
            U += count;
        }

        if (allsame) {
            continue;
        }

        // push into buckets
        for (size_t i=0; i < sz; i++) {
            uint8_t *arr = (uint8_t*)old[i].arr;
            *bptrs[arr[col]]++ = old[i];
        }

        triplet *swap = tmp;
        tmp = old;
        old = swap;
    }
    
    if (old != T) {
        memcpy(T, old, sz * sizeof(triplet));
        delete[] old;
    } else {
        delete[] tmp;
    }
}
