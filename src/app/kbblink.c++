#include "kbblink.h"
#include "../kernel.h"
#include "../iob.h"
#include "../string.h"
#include "../time.h"


#define BUFFER_SIZE 8
#define SPACE_RELEASED_KEY_CODE (57 + 0x80)
#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define WRITE_MASKS_COMMAND 0xED
#define ENABLE_SCROLL_LOCK_HIGHLIGHT_BIT 0x01
#define ENABLE_NUM_LOCK_HIGHLIGHT_BIT 0x02
#define ENABLE_CAPS_LOCK_HIGHLIGHT_BIT 0x04
#define DISABLE 0x0
#define DELAY_TIMEOUT 500
#define FULL_BLINK_CYCLE 8
#define BLINK_CYCLE_ITERATIONS 6
#define KEYBOARD_BUFFER_READ_ITERATIONS_LIMIT 5000
#define KEYBOARD_WRITE_TRY_LIMIT 4



static volatile bool endedShowKeycodeMode;



int KbBlink::run(Vector<char*> args) {
    Kernel::out.setStatus("\t\t\t\tKEYBOARD BLINK app");
    printArgs(args);
    while(true) {
        Kernel::out.clear();
        Kernel::out << "Kb & Blink main menu:\n"
                       << "\t\t1. Show keycodes\n"
                       << "\t\t2. Blink\n"
                       << "\t\t3. Quit\n";
        switch (Kernel::in.getchar()) {
            case '1':
                showKeycodes();
                break;
            case '2':
                blink();
                break;
            case '3':
                return 0;
            default:
                Kernel::out << "Wrong key\n";
                break;
        }
        Kernel::in.getchar();
    }
}



static void keyPressNewHandler(uint8_t code) {
    char buffer[BUFFER_SIZE];
    uint_to_string(code, buffer, 0x10);
    Kernel::out << "0x" << buffer << " ";
    if (code == SPACE_RELEASED_KEY_CODE) {
        endedShowKeycodeMode = true;
    }
}



void KbBlink::showKeycodes() {
    Kernel::out << "\nRELEASE SPACE TO EXIT KEYCODES DEMONSTRATIONS MODE\n";
    endedShowKeycodeMode = false;
    EnterQueue::setKeyboardHandler(keyPressNewHandler);
    while (!endedShowKeycodeMode) {
        halt();
    }
    EnterQueue::resetKeyboardHandler();
    Kernel::out << "\n\nFINISHED KEYCODES DEMONSTRATION\n\n";
}



void KbBlink::blink() {
    Kernel::out << "\nBlinking " << (uint64_t)BLINK_CYCLE_ITERATIONS << " times.\n";

    blinkKey(true, true, true);
    Time::delay(10 * DELAY_TIMEOUT);
    Kernel::out << "FINISHED HIGHLIGHTING ALL";

    for (uint8_t j = 0; j < BLINK_CYCLE_ITERATIONS; ++j) {
        Kernel::out << "\nBlink N" << (uint8_t)(j + 1);
        for (uint8_t i = 0; i < FULL_BLINK_CYCLE; ++i) {
            Time::delay(DELAY_TIMEOUT);
            blinkKey((bool) (i & ENABLE_SCROLL_LOCK_HIGHLIGHT_BIT)
                    , (bool) (i & ENABLE_NUM_LOCK_HIGHLIGHT_BIT)
                    , (bool) (i & ENABLE_CAPS_LOCK_HIGHLIGHT_BIT));
        }
    }
    Kernel::out << "\nFINISHED BLINKING\n";
    Kernel::in.flush();
}



void KbBlink::blinkKey(bool scroll, bool num, bool caps) {
    int iterations = KEYBOARD_WRITE_TRY_LIMIT;

    bool success = waitKBBuffer();
    if (!success) {
        Kernel::out << "\n\nBLINKING ABORTED\n";
        return;
    }

    do {
        iterations--;
        outb(KEYBOARD_DATA_PORT, WRITE_MASKS_COMMAND);
    } while (inb(KEYBOARD_DATA_PORT) == 0xFE && iterations >= 0);
    if (iterations <= 0) {
        Kernel::out << "ERROR";
    }
    iterations = KEYBOARD_WRITE_TRY_LIMIT;

    success = waitKBBuffer();
    if (!success) {
        Kernel::out << "\n\nBLINKING ABORTED\n";
        return;
    }

    do {
        iterations--;
        outb(KEYBOARD_DATA_PORT, (uint8_t)
                (((scroll)? ENABLE_SCROLL_LOCK_HIGHLIGHT_BIT : DISABLE)
                 | ((num)? ENABLE_NUM_LOCK_HIGHLIGHT_BIT     : DISABLE)
                 | ((caps)? ENABLE_SCROLL_LOCK_HIGHLIGHT_BIT : DISABLE)));
    } while (inb(KEYBOARD_DATA_PORT) == 0xFE && iterations >= 0);
    if (iterations <= 0) {
        Kernel::out << "ERROR";
    }
}



KbBlink::KbBlink() : Executable(true) {
}



bool KbBlink::waitKBBuffer() {
    for (uint32_t i = 0; i < KEYBOARD_BUFFER_READ_ITERATIONS_LIMIT; ++i) {
        if ((inb(KEYBOARD_STATUS_PORT) & 0x02) == 0x00) {
            return true;
        }
    }
    Kernel::out << "ERROR WRITING IN KB PORT";
    return false;
}
