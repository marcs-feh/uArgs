#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "uargs.h"

void helpMsg(const char* _){
	printf("Usage: bin [-h/-b N]\n"
				 "\t-b N    make N bananas\n"
				 "\t-s      smile\n"
				 "\t-h      display this help message\n"
		);
}

void bananaCallback(const char* arg){
 	// Functions must handle cases where there's no arg
	if(arg == NULL) {
		helpMsg(NULL);
		exit(EXIT_FAILURE);
	}
	int n = atoi(arg);
	if(n == 0){
		printf("Zero banana? have a fresh bread then 🍞");
	}
	for(int i = 0; i < n; i++)
		printf("🍌 ");
	printf("\n");
}

void smile(const char* _){
	printf(":)\n");
}

int main(int argc, const char** argv){
	uArgsRegistry r = uArgs_newReg();
	uArgs_pushFlag(&r, (uArgsFlag){"h", helpMsg}); 	  // Short flag -h
	uArgs_pushFlag(&r, (uArgsFlag){"help", helpMsg}); // Long flag --help
	uArgs_pushFlag(&r, (uArgsFlag){"b", bananaCallback});
	uArgs_pushFlag(&r, (uArgsFlag){"S", smile});
	uArgs_pushFlag(&r, (uArgsFlag){" -no", NULL}); 			 // Bad flags will fail to push
	
	// Fail on not enough args
	if(argc < 2){
		helpMsg(NULL);
		return EXIT_FAILURE;
	}
	uArgs_execFlags(&r, argv + 1, argc - 1); // +1 and -1 to ignore program name
	
	return 0;
}

