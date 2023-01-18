#pragma once

#include <cstdint>
#include <cstring>

#define PROBABLY_IN true
#define DEFINITELY_NOT false

template <typename K = int>
class bloomFilter {
private:
    static inline int murmur_32_scramble(int k) {
        k *= 0xcc9e2d51;
        k = (k << 15) | (k >> 17);
        k *= 0x1b873593;
        return k;
    }
    int murmur3_32(const uint8_t *key, size_t len, int seed) {
        int h = seed;
        int k;
        /* Read in groups of 4. */
        for (size_t i = len >> 2; i; i--) {
            // Here is a source of differing results across endiannesses.
            // A swap here has no effects on hash properties though.
            memcpy(&k, key, sizeof(int));
            key += sizeof(int);
            h ^= murmur_32_scramble(k);
            h = (h << 13) | (h >> 19);
            h = h * 5 + 0xe6546b64;
        }
        /* Read the rest. */
        k = 0;
        for (unsigned long i = len & 3; i; i--) {
            k <<= 8;
            k |= key[i - 1];
        }
        // A swap is *not* necessary here because the preceding loop already
        // places the low bytes in the low places according to whatever endianness
        // we use. Swaps only apply when the memory is copied in a chunk.
        h ^= murmur_32_scramble(k);
        /* Finalize. */
        h ^= len;
        h ^= h >> 16;
        h *= 0x85ebca6b;
        h ^= h >> 13;
        h *= 0xc2b2ae35;
        h ^= h >> 16;
        return h;
    }

    int maxElems;
    int bits;
    int hashFuncs = 7;
    int *filter;

public:
    bloomFilter(int size) : maxElems(size), bits(size * 10), filter(new int[size * 10]){};
    void insert(K elem) {
        for (int i = 0; i < hashFuncs; ++i) {
            filter[murmur3_32((uint8_t *)&elem, sizeof(elem), i) % maxElems] += 1;
        }
    };
    void remove(K elem) {
        for (int i = 0; i < hashFuncs; ++i) {
            filter[murmur3_32((uint8_t *)&elem, sizeof(elem), i) % maxElems] -= 1;
        }
    };
    bool contains(K elem) {
        for (int i = 0; i < hashFuncs; ++i) {
            if (filter[murmur3_32((uint8_t *)&elem, sizeof(elem), i) % maxElems] == 0) {
                return DEFINITELY_NOT;
            }
        }
        return PROBABLY_IN;
    };
    ~bloomFilter() {
        delete[] filter;
    };
};