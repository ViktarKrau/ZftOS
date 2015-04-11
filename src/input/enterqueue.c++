#include "enterqueue.h"

#define RIGHT_SHIFT_PRESSED 0x36
#define LEFT_SHIFT_PRESSED 0x2A
#define LEFT_SHIFT_RELEASED 0xAA
#define RIGHT_SHIFT_RELEASED 0xB6



volatile uint8_t EnterQueue::queue[ENTER_QUEUE_SIZE];
uint32_t EnterQueue::offset = 0;
uint32_t EnterQueue::position = 0;
bool EnterQueue::isHandlerSet = false;
bool EnterQueue::shiftHold = false;
void (*EnterQueue::handler)(uint8_t keycode);



void enter_queue_push(uint8_t keycode) {
	EnterQueue::push(keycode);
}



void EnterQueue::push(uint8_t keycode) {
	if(isHandlerSet) {
		return handler(keycode);
	}
	if (keycode == RIGHT_SHIFT_RELEASED || keycode == LEFT_SHIFT_RELEASED) {
		shiftHold = false;
		return;
	}
	if (keycode == LEFT_SHIFT_PRESSED || keycode == RIGHT_SHIFT_PRESSED) {
		shiftHold = true;
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



bool EnterQueue::isShiftHold(){
	return shiftHold;
}



void EnterQueue::flush() {
	position = offset;
}
