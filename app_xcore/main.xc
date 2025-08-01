#include <stdio.h>

extern "C"{
	void run_midi(void);
}

int main(void){
	printf("Hello world!\n");

	run_midi();

	return 0;
}