
#include "../zftdef.h"

#ifndef _ZFTOS_DEV_BITMAP_H_
#define _ZFTOS_DEV_BITMAP_H_


class Bitmap{
public:
    Bitmap(size_t size);
    bool get(size_t pos);
    void set(size_t pos);
    void reset(size_t pos);
    void setval(size_t pos, bool value);
    ~Bitmap();
private:
    byte_ptr_t map;
    size_t bitmapLength;
};


#endif //_ZFTOS_DEV_BITMAP_H_
