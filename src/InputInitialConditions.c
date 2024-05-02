#include "Chaos.h"

void inputIC() {

 	char line[30];

    /* CHOICE OF the initial conditions */
    if (dimension == 1) {
    	printf(ANSI_COLOR_GREEN "Please enter a value for the initial condition.\n\n"ANSI_COLOR_RESET);
    }
    else {
    	printf(ANSI_COLOR_GREEN "Please enter values for the initial conditions, separated by Enter.\n\n"ANSI_COLOR_RESET);
    }
	for (size_t d = 0; d < dimension; d++) {
CHOOSE_IC:
		if (fgets(line, sizeof(line), stdin)) {
			if(line[0] == '\n') {
				userMapValues.IC[d] = defaultMapValues.IC[d];
				printf(ANSI_COLOR_GREEN "The default value will be used for variable %s.\n\n" ANSI_COLOR_RESET, variablesNames[d]);
			}
			else if (sscanf(line, "%f", &userMapValues.IC[d]) != 1) {
				printf(ANSI_COLOR_GREEN "Please enter a valid number.\n\n" ANSI_COLOR_RESET);
				goto CHOOSE_IC;
			}
		}
	}

}
