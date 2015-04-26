#include "../zftdef.h"
#include "enterqueue.h"



#ifndef INPUT_STREAM_H
#define INPUT_STREAM_H



#define NUMBER_BUFFER_SIZE 0x10



class InputStream
{
public:
	InputStream();
	uint8_t getkey();
	char getchar();
	void gets(char* buffer, size_t size);
	void getWord(char *buffer, size_t size);
	uint64_t getuint();
	int64_t getint();
	uint8_t getkey_nolock();
	char getchar_nolock();
	void flush();
	int64_t getLocation();
	~InputStream();
private:
	char buffer[NUMBER_BUFFER_SIZE];
};



#endif	/*INPUT_STREAM_H*/
