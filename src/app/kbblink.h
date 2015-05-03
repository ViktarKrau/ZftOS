#include "../exec/executable.h"



#ifndef ZFTOS_DEV_KBBLINK_H
#define ZFTOS_DEV_KBBLINK_H



class KbBlink : public Executable {
public:
    KbBlink();
    virtual int run(Vector<char*> args) override;
    enum KeyLights { SCROLL_LOCK = 0, NUM_LOCK = 1, CAPS_LOCK = 2};
    static void blinkKey(bool scroll, bool num, bool caps);
    bool scrollEnabled, numEnabled, capsEnabled;
private:
    static bool waitKBBuffer();
    void showKeycodes();
    void blink();


};

#endif //ZFTOS_DEV_KBBLINK_H
