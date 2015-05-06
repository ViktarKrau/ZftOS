#include "enterqueue.h"
#include "../kernel.h"
#include "../iob.h"



#define RIGHT_SHIFT_PRESSED 0x36
#define LEFT_SHIFT_PRESSED 0x2A
#define LEFT_SHIFT_RELEASED 0xAA
#define RIGHT_SHIFT_RELEASED 0xB6
#define ENABLE_SCROLL_LOCK_HIGHLIGHT_BIT 0x01
#define ENABLE_NUM_LOCK_HIGHLIGHT_BIT 0x02
#define ENABLE_CAPS_LOCK_HIGHLIGHT_BIT 0x04
#define DISABLE 0x0
#define KEYBOARD_BUFFER_READ_ITERATIONS_LIMIT 5000
#define KEYBOARD_WRITE_TRY_LIMIT 5
#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define WRITE_MASKS_COMMAND 0xED
#define CAPS_LOCK_RELEASED (58 + 0x80)



volatile uint8_t EnterQueue::queue[ENTER_QUEUE_SIZE];
uint32_t EnterQueue::offset = 0;
uint32_t EnterQueue::position = 0;
bool EnterQueue::isHandlerSet = false;
bool EnterQueue::shiftHold = false;
bool EnterQueue::scrollEnabled = false;
bool EnterQueue::capsEnabled = false;
bool EnterQueue::numEnabled = false;
void (*EnterQueue::handler)(uint8_t keycode);



void enter_queue_push(uint8_t keycode) {
	EnterQueue::push(keycode);
}



void EnterQueue::push(uint8_t keycode) {
	if(isHandlerSet) {
		return handler(keycode);
	}
	if (keycode == RIGHT_SHIFT_RELEASED
		|| keycode == LEFT_SHIFT_RELEASED) {
		shiftHold = false;
		return;
	}
	if (keycode == LEFT_SHIFT_PRESSED
		|| keycode == RIGHT_SHIFT_PRESSED) {
		shiftHold = true;
		return;
	}
	if (keycode == CAPS_LOCK_RELEASED) {
		blinkKey(scrollEnabled, numEnabled, !capsEnabled);
		return;
	}
	queue[position] = keycode;
	position++;
	if (position == ENTER_QUEUE_SIZE) {
		position = 0;
	}
	if (position == offset) {
		offset++;
	}
}



uint8_t EnterQueue::pop() {
	if(offset == position) {
		return 0;
	}
	offset++;
	if(offset == ENTER_QUEUE_SIZE){
		offset = 0;
		return queue[ENTER_QUEUE_SIZE - 1];
	}
	return queue[offset - 1];
}



void EnterQueue::setKeyboardHandler(void (*_handler)(uint8_t keycode)) {
	handler = _handler;
	isHandlerSet = true;
}



void EnterQueue::resetKeyboardHandler() {
	isHandlerSet = false;
}



bool EnterQueue::isShiftHold() {
	return (shiftHold ^ capsEnabled);
}



void EnterQueue::flush() {
	position = offset;
}




bool EnterQueue::waitKBBuffer() {
	for (uint32_t i = 0; i < KEYBOARD_BUFFER_READ_ITERATIONS_LIMIT; ++i) {
		if ((inb(KEYBOARD_STATUS_PORT) & 0x02) == 0x00) {
			return true;
		}
	}
	Kernel::out << "ERROR WRITING IN KB PORT";
	return false;
}



void EnterQueue::blinkKey(bool scroll, bool num, bool caps) {
	capsEnabled = caps;
	numEnabled = num;
	scrollEnabled = scroll;
	if (!writeToKbPort(WRITE_MASKS_COMMAND)){
		return;
	}
	writeToKbPort((uint8_t)
						   (((scroll)? ENABLE_SCROLL_LOCK_HIGHLIGHT_BIT : DISABLE)
							| ((num)? ENABLE_NUM_LOCK_HIGHLIGHT_BIT     : DISABLE)
							| ((caps)? ENABLE_CAPS_LOCK_HIGHLIGHT_BIT   : DISABLE)));
}



EnterQueue::EnterQueue() {

}



EnterQueue::~EnterQueue() {

}



bool EnterQueue::writeToKbPort(uint8_t value) {
	int iterations = KEYBOARD_WRITE_TRY_LIMIT;
	bool success = waitKBBuffer();
	if (!success) {
		Kernel::out << "\n\nBLINKING ABORTED\n";
		return false;
	}

	do {
		iterations--;
		outb(KEYBOARD_DATA_PORT, value);
	} while (inb(KEYBOARD_DATA_PORT) == 0xFE && iterations > 0);
	if (iterations <= 0) {
		Kernel::out << "ERROR";
		return false;
	}
	return true;
}
