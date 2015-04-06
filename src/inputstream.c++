#include "inputstream.h"
#include "kbmap.h"
#include "iob.h"
#include "kernel.h"
#include "string.h"


uint8_t InputStream::getkey() {
	uint8_t keycode = 0;
	while(keycode == 0) {
		halt();
		keycode = EnterQueue::pop();
	}
	return keycode;
}



uint8_t InputStream::getkey_nolock() {
	return EnterQueue::pop();
}



char InputStream::getchar() {
	
	int16_t code;
	while (true) {
		code = getkey();
		code -= 0x80;
		if (code < 0)
			continue;
		if ((code != 29 && code != 42 && code != 54 && code < 56 && code > 1)
				|| code == 57 || code == 74 || code == 78) {
			if (EnterQueue::isShiftHold()) {
				return kbmap[code] - 'a' + 'A';
			}
			return kbmap[code];
		}
	}
}



char InputStream::getchar_nolock() {
	int16_t code;
	code = getkey_nolock();
	code -= 0x80;
	if(code <= 0)
		return 0;
	if((code != 29 && code != 42 && code != 54 && code < 56 && code > 1) || code == 57 || code == 74 || code == 78){
		if(EnterQueue::isShiftHold()){
				return kbmap[code] - 'a' + 'A';
		}
		return kbmap[code];
	}
	return 0;
}



InputStream::InputStream(){

}



InputStream::~InputStream(){
	
}



void InputStream::gets(char* buffer, size_t size) {
	size_t index = 0;
	while (true) {
		char c = getchar();
		if (c == '\n') {
			buffer[index] = 0;
			if(!is_empty(buffer)) {
				Kernel::out->newLine();
				return;
			}
			else {
				continue;
			}
		}
		else if (c == '\b') {
			if (index) {
				buffer[index] = 0;
				index--;
				Kernel::out->putchar('\b');
			}
		}
		else {
			if (index < size - 1) {
				buffer[index++] = c;
				Kernel::out->putchar(c);
			}
		}
	}
}



void InputStream::get_word(char *buffer, size_t size) {
	size_t index = 0;
	while (true) {
		char c = getchar();
		if (c == '\n' || c == ' ') {
			buffer[index] = 0;
			if (!is_empty(buffer)) {
				return;
			}
			else {
				continue;
			}
		}
		else if (c == '\b') {
			if (index) {
				buffer[index] = 0;
				index--;
				Kernel::out->putchar('\b');
			}
		}
		else {
			if (index < size - 1) {
				buffer[index++] = c;
				Kernel::out->putchar(c);
			}
		}
	}
}
