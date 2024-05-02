#include "Chaos.h"

void inputIterations(unsigned int NMin, long unsigned int NMax) {

 	char line[30];

    /* CHOICE OF N, the number of iterations */
    printf(ANSI_COLOR_GREEN "\nPlease enter the number of iterations you want the program to perform for the map. ");
    printf("Your choice should be in the interval [%d, %ld].\n\n" ANSI_COLOR_RESET, NMin, NMax);
CHOOSE_ITERATION:
	if (fgets(line, sizeof(line), stdin)) {
		if(line[0] == '\n') {
        	N = NDefault;
        	printf(ANSI_COLOR_GREEN "The default value will be used.\n\n" ANSI_COLOR_RESET);
        }
		else if (sscanf(line, "%ld", &N) != 1) {
			printf(ANSI_COLOR_GREEN "Please enter a valid number.\n\n" ANSI_COLOR_RESET);
			goto CHOOSE_ITERATION;
		}
		else if ((N < NMin) || (N > NMax)) {
			printf(ANSI_COLOR_GREEN "\nPlease enter an acceptable amount of iterations.\n\n" ANSI_COLOR_RESET);
			goto CHOOSE_ITERATION;
		}
	}

}
