#include "kernel.h++"
#ifndef SHELL_H
#define SHELL_H


class Shell
{
public:
	Shell();
	void run();
	~Shell();
private:
	char buff[60];
	void getCommand();
};


#endif /*SHELL_H*/