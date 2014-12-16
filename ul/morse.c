#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/******************************************************

  Created by Konstantino Sparakis
  
  Usage:
    ./morse [flag] OR [Text to Morse]
        -l (list what is currently being printed)
 
	
  Examples:
	./morse -l
		Lists all timers and how many seconds untill termination

	./morse SOS
	     Translates string "SOS" into morse code on the led
	
******************************************************/

void printManPage(void);

int main(int argc, char **argv) {
	
	char line[3000];
	int ii, count = 0;
	
	/* Check to see if mknod has ran
	   Assumes that morse is tied to /dev/t2Morse */
	FILE * pFile;
	pFile = fopen("/dev/t2Morse", "r+");
	if (pFile==NULL) {
		fputs("t2Morse module isn't loaded ....note major = 60....\n",stderr);
		return -1;
	}
	
	
	// Check if in read mode
	if (argc == 2 && strcmp(argv[1], "-l") == 0) {
		
		if(fgets(line, 3000, pFile) != NULL)
		{
            		printf("%s\n",line);		
		}
		
	}

	// Check if in write mode
	else if (argc > 2  == 0) {
		char out[256] = ""; 
		int i;
		for (i=1; i<argc; i++)
		{
			strcat(out, " ");
			strcat(out, argv[i]);
		}
		printf("Text %s to morse...\n", out);
		fputs(out, pFile);
	}


	// Otherwise invalid
	else {
		printManPage();
	}

	fclose(pFile);
	return 0;
}
void printManPage() {
	printf("Error: invalid use.\n");
	printf(" morseUl [-l] \n");
	printf(" -l: prints last message and time constant\n");
	printf(" morseUL [TEXT TO MORSE]]\n");	
	printf(" converts text to morse \n");
}
