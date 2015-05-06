#include "kbblink.h"
#include "../kernel.h"
#include "../iob.h"
#include "../string.h"
#include "../time.h"



#define BUFFER_SIZE 8
#define SPACE_RELEASED_KEY_CODE (57 + 0x80)
#define ENABLE_SCROLL_LOCK_HIGHLIGHT_BIT 0x01
#define ENABLE_NUM_LOCK_HIGHLIGHT_BIT 0x02
#define ENABLE_CAPS_LOCK_HIGHLIGHT_BIT 0x04
#define DELAY_TIMEOUT 150
#define FULL_BLINK_CYCLE 8
#define BLINK_CYCLE_ITERATIONS 6



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
    EnterQueue::blinkKey(true, true, true);
    Time::delay(8 * DELAY_TIMEOUT);
    for (uint8_t j = 0; j < BLINK_CYCLE_ITERATIONS; ++j) {
        Kernel::out << "\nBlink N" << (uint8_t)(j + 1);
        for (uint8_t i = 0; i < FULL_BLINK_CYCLE; ++i) {
            Time::delay(DELAY_TIMEOUT);
            EnterQueue::blinkKey((bool) (i & ENABLE_SCROLL_LOCK_HIGHLIGHT_BIT)
                    , (bool) (i & ENABLE_NUM_LOCK_HIGHLIGHT_BIT)
                    , (bool) (i & ENABLE_CAPS_LOCK_HIGHLIGHT_BIT));
        }
    }
    Kernel::out << "\nFINISHED BLINKING\n";
    Kernel::in.flush();
}



KbBlink::KbBlink() : Executable(true) {
}


