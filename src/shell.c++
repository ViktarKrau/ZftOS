#include "shell.h++"
#include "string.h"

const char helptext[] = "\tZunft OS ver. 0.02\n"
						"\tList of supported commands\n"
						"\t\thelp  --  views this message\n"
						"\t\texit  --  power off\n"
						"\t\treboot -- restart pc\n"
						"\t\tcolor -- change output color\n"
						"\t\tcowsay ... -- cow says what you want it to\n";

Shell::Shell(){
}
Shell::~Shell(){
}
void Shell::getCommand(){
	Kernel::out->puts("\n > ");
	size_t index = 0;
	while(true){
		char c = Kernel::in->getchar();
		if(c == '\n'){
			buff[index] = 0;
			if(!is_empty(buff)){
				Kernel::out->newLine();
				return;
			}
			else{
				return getCommand();
			}
		}
		else if(c == '\b'){
			if(index){
				buff[index] = 0;
				index--;
				Kernel::out->putchar('\b');
			}
		}
		else{
			if(index < 59){
				buff[index++] = c;
				Kernel::out->putchar(c);
			}
		}
	}
}
void Shell::run(){
	Kernel::out->clear();

	Kernel::out->puts("                         WELCOME TO ZUNFT OS SHELL!" 
						"\nType \"help\" to get list of available commands");
	while(true){
		getCommand();
		if(!strcmp(buff, "help")){
			Kernel::out->putsln(helptext);
		}
		else if(!strcmp(buff, "exit")){
			Kernel::exit(Kernel::SHUTDOWN);
		}
		else if(!strcmp(buff, "reboot")){
			Kernel::exit(Kernel::REBOOT);
		}
		else if(!strcmp(buff, "color")){
			Kernel::out->setColor(Kernel::out->getColor() + 1);
		}
		else{
			Kernel::out->puts("Command invalid");
		}
	}
}