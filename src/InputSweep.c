#include "Chaos.h"

/*
 * The inputSweep function will ask the user to input the number of divisions for the principal parameter.
 * It will display the allowed range from the database and verify that the input is in this range.
 * If the user presses Return, the default value will be stored.
 *
 * The function is called from ChaosMain. 
 */

void inputSweep(const unsigned int SMin, const long unsigned  int SMax) {

 	char line[30];

    /* CHOICE OF N, the number of iterations */
    printf(ANSI_COLOR_GREEN "\nPlease enter the number of divisions you want the principal parameter. ");
    printf("Your choice should be in the interval [%d, %ld].\n\n" ANSI_COLOR_RESET, SMin, SMax);
CHOOSE_SWEEP:
	if (fgets(line, sizeof(line), stdin)) {
		if(line[0] == '\n') {
        	S = SDefault;
        	printf(ANSI_COLOR_GREEN "The default value will be used.\n\n" ANSI_COLOR_RESET);
        }
		else if (sscanf(line, "%ld", &S) != 1) {
			printf(ANSI_COLOR_GREEN "Please enter a valid number.\n\n" ANSI_COLOR_RESET);
			goto CHOOSE_SWEEP;
		}
		else if ((S < SMin) || (S > SMax)) {
			printf(ANSI_COLOR_GREEN "\nPlease enter an acceptable number of divisions.\n\n" ANSI_COLOR_RESET);
			goto CHOOSE_SWEEP;
		}
	}

}
