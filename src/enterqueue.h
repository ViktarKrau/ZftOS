#include <stdbool.h>
#include "zftdef.h"

#ifndef ENTER_QUEUE_H
#define ENTER_QUEUE_H

#define ENTER_QUEUE_SIZE 0x400

void enter_queue_push(uint8_t keycode);

class EnterQueue
{
public:
	EnterQueue();
	static void push(uint8_t keycode);
	static uint8_t pop();
	//static void initialize();
	static void setKeyboardHandler(void (*_handler)(uint8_t keycode));
	static void resetKeyboardHandler();
	static bool isShiftHold();
	~EnterQueue();
private:
	static bool shiftHold;
	static void (*handler)(uint8_t keycode);
	static bool isHandlerSet;
	static volatile uint8_t queue[ENTER_QUEUE_SIZE];
	static uint32_t offset;
	static uint32_t position;
};



#endif /*ENTER_QUEUE_H*/