#include "zftdef.h"
#include "enterqueue.h"

#ifndef INPUT_STREAM_H
#define INPUT_STREAM_H

class InputStream
{
public:
	InputStream();
	uint8_t getkey();
	char getchar();
	void gets(char* buffer, size_t size);
	void get_word(char* buffer, size_t size);
	uint8_t getkey_nolock();
	char getchar_nolock();
	~InputStream();
private:
};

#endif	/*INPUT_STREAM_H*/