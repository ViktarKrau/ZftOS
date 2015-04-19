#include "../kernel.h"



#ifndef SHELL_H
#define SHELL_H



class Shell : public Executable
{
public:
	Shell();
	virtual int run(Vector<char*> args) override;
	void calc();
	void setGMT();
	void cowsay();
	void piano();
	void printTime();
	~Shell();
private:
	void runMemTest();
	char buff[60];
	void getCommand();
};



#endif /*SHELL_H*/