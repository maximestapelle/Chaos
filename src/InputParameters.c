#include "Chaos.h"

void inputParameters() {

 	char line[30];

    /* CHOICE OF the parameters */
    /* For al choices but Attractor, the range of the first parameters is already known. We still have to fetch the value of this first parameter, in the case of Attractor */
    if (numberParameters == 1) {
    	if (userAction != 3) return;
    	else {
    		printf(ANSI_COLOR_GREEN "Please enter a value for the parameter.\n\n"ANSI_COLOR_RESET);
CHOOSE_PARAM1:
			if (fgets(line, sizeof(line), stdin)) {
				if(line[0] == '\n') {
					userMapValues.parameters[0] = defaultMapValues.parameters[0];
					printf(ANSI_COLOR_GREEN "The default value will be used for parameter %s.\n\n" ANSI_COLOR_RESET, parametersNames[0]);
				}
				else if (sscanf(line, "%f", &userMapValues.parameters[0]) != 1) {
					printf(ANSI_COLOR_GREEN "Please enter a valid number.\n\n" ANSI_COLOR_RESET);
					goto CHOOSE_PARAM1;
				}
			}
    	}
    }
    else if (userAction != 3) {
    	if (numberParameters == 2) {
	    	printf(ANSI_COLOR_GREEN "Please enter a value for the remaining parameter.\n\n"ANSI_COLOR_RESET);
	    }
	    else printf(ANSI_COLOR_GREEN "Please enter a value for the remaining parameters.\n\n"ANSI_COLOR_RESET);
    }
    else {
    	printf(ANSI_COLOR_GREEN "Please enter values for the parameters, separated by Enter.\n\n"ANSI_COLOR_RESET);
CHOOSE_PARAM1bis:
		if (fgets(line, sizeof(line), stdin)) {
			if(line[0] == '\n') {
				userMapValues.parameters[0] = defaultMapValues.parameters[0];
				printf(ANSI_COLOR_GREEN "The default value will be used for parameter %s.\n\n" ANSI_COLOR_RESET, parametersNames[0]);
			}
			else if (sscanf(line, "%f", &userMapValues.parameters[0]) != 1) {
				printf(ANSI_COLOR_GREEN "Please enter a valid number.\n\n" ANSI_COLOR_RESET);
				goto CHOOSE_PARAM1bis;
			}
		}
    }
    /* At this stage we have handled the first parameter. The following loop will handle the rest, if any. */
	for (size_t p = 1; p < numberParameters; p++) {
CHOOSE_PARAMS:
		if (fgets(line, sizeof(line), stdin)) {
			if(line[0] == '\n') {
				userMapValues.parameters[p] = defaultMapValues.parameters[p];
				printf(ANSI_COLOR_GREEN "The default value will be used for variable %s.\n\n" ANSI_COLOR_RESET, parametersNames[p]);
			}
			else if (sscanf(line, "%f", &userMapValues.parameters[p]) != 1) {
				printf(ANSI_COLOR_GREEN "Please enter a valid number.\n\n" ANSI_COLOR_RESET);
				goto CHOOSE_PARAMS;
			}
		}
	}

}
