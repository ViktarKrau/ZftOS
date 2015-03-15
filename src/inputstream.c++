#include "inputstream.h++"
#include "kbmap.h"
uint8_t InputStream::getkey(){
	char keycode = 0;
	while(keycode == 0){
		keycode = EnterQueue::pop();
	}
	return keycode;
}
uint8_t InputStream::getkey_nolock(){
	return EnterQueue::pop();
}
char InputStream::getchar(){
	int16_t code;
	while(true){
		code = getkey();
		code -= 0x80;
		if(code < 0)
			continue;
		if((code != 29 && code != 42 && code != 54 && code < 56 && code > 1) || code == 57 || code == 74 || code == 78){
			if(EnterQueue::isShiftHold()){
				return kbmap[code] - 'a' + 'A';
			}
			return kbmap[code];
		}
	}
}
char InputStream::getchar_nolock(){
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