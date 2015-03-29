#include "bitmap.h"
#include "zft_memory.h"

Bitmap::Bitmap(size_t size) {
    if (size % 8 != 0) {
        bitmapLength = size / 8;
    }
    else {
        bitmapLength = size / 8 + 1;
    }
    map = new byte_t[bitmapLength];
    memzero((void*) map, bitmapLength);
}

bool Bitmap::get(size_t pos) {
    return (bool) (map[pos / 8] & (0x1 << (pos % 8)));
}

void Bitmap::set(size_t pos) {
    map[pos / 8] |= (0x1 << (pos % 8));
}

void Bitmap::reset(size_t pos) {
    if (get(pos)) {
        map[pos / 8] ^= (0x1 << (pos % 8));
    }
}

void Bitmap::setval(size_t pos, bool value) {
    if (value) {
        set(pos);
    }
    else {
        bool exValue = get(pos);
        if (exValue) {
            map[pos / 8] ^= (0x1 << (pos % 8));
        }
    }
}


Bitmap::~Bitmap() {
    if (map) {
        delete map;
    }
}
