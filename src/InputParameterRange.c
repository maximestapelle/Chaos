#include "Chaos.h"

/*
 * The inputParameterRange function will ask the user to input the parameter Range for Lyapunov and Bifurcation.
 * If the user presses Return, the default values will be stored.
 *
 * The function is called from ChaosMain. 
 */

void inputParameterRange() {

 	char line[30];

    /* CHOICE OF the parameter for bifurcations and lyapunov exponents */
	printf(ANSI_COLOR_GREEN "Please enter the minimum and maximum values for the parameter %s, separated by Enter.\n\n"ANSI_COLOR_RESET, parametersNames[0]);

	for (size_t i = 0; i < 2; i++) {
CHOOSE_PARAMRANGE:
		if (fgets(line, sizeof(line), stdin)) {
			if(line[0] == '\n') {
				userMapValues.parameterRange[i] = defaultMapValues.parameterRange[i];
				switch (i) {
					case 0:
						printf(ANSI_COLOR_GREEN "The default minimum value will be used.\n\n" ANSI_COLOR_RESET);
						break;
					case 1:
						printf(ANSI_COLOR_GREEN "The default maximum value will be used.\n\n" ANSI_COLOR_RESET);
						break;
				}
			}
			else if (sscanf(line, "%f", &userMapValues.parameterRange[i]) != 1) {
				printf(ANSI_COLOR_GREEN "Please enter a valid number.\n\n" ANSI_COLOR_RESET);
				goto CHOOSE_PARAMRANGE;
			}
		}
	}

}
