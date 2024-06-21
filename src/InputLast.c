#include "Chaos.h"

/*
    - The inputIterations function will ask the user to input the number of iterations.
        It will display the allowed range from the database and verify that the input is in this range.
    - The inputIC function will ask the user to input the initial conditions.
    - The inputParameterRange function will ask the user to input the parameter Range for Lyapunov and Bifurcation.
    - The inputSweep function will ask the user to input the number of divisions for the principal parameter.
        It will display the allowed range from the database and verify that the input is in this range.
    - The inputParameters function will ask the user to input the parameters.

    For all functions, if the user presses Return, the default values will be stored.

    The functions are the last to be called by ChaosMain for User Input.
 */

void inputIterations(const unsigned int NMin, const long unsigned int NMax) {

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

void inputIC() {

 	char line[30];

    /*  Choice of the initial conditions */
    if (dimension == 1) {
    	printf(ANSI_COLOR_GREEN "Please enter a value for the initial condition.\n\n" ANSI_COLOR_RESET);
    }
    else {
    	printf( ANSI_COLOR_GREEN "Please enter values for the initial conditions, separated by Enter.\n\n"
                ANSI_COLOR_RESET);
    }
	for (size_t d = 0; d < dimension; d++) {
CHOOSE_IC:
		if (fgets(line, sizeof(line), stdin)) {
			if(line[0] == '\n') {
				userMapValues.IC[d] = defaultMapValues.IC[d];
				printf( ANSI_COLOR_GREEN "The default value will be used for variable %s.\n\n"
                        ANSI_COLOR_RESET, variablesNames[d]);
			}
			else if (sscanf(line, "%f", &userMapValues.IC[d]) != 1) {
				printf(ANSI_COLOR_GREEN "Please enter a valid number.\n\n" ANSI_COLOR_RESET);
				goto CHOOSE_IC;
			}
		}
	}

}

/*  Choice of the parameter for bifurcations and lyapunov exponents */
void inputParameterRange() {

 	char line[30];

	printf( ANSI_COLOR_GREEN "Please enter the minimum and maximum values for the parameter %s, separated by Enter.\n\n"
            ANSI_COLOR_RESET, parametersNames[0]);

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

void inputSweep(const unsigned int SMin, const long unsigned  int SMax) {

 	char line[30];

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

void inputParameters() {

 	char line[30];

    if (numberParameters == 1) {    /*  For all choices but Attractor, the range of the first parameters is
                                        already known. We still have to fetch the value of this first parameter,
                                        in the case of Attractor */
    	if (userAction != 3) return;
    	else {
    		printf(ANSI_COLOR_GREEN "Please enter a value for the parameter.\n\n"ANSI_COLOR_RESET);
CHOOSE_PARAM1:
			if (fgets(line, sizeof(line), stdin)) {
				if(line[0] == '\n') {
					userMapValues.parameters[0] = defaultMapValues.parameters[0];
					printf( ANSI_COLOR_GREEN "The default value will be used for parameter %s.\n\n"
                            ANSI_COLOR_RESET, parametersNames[0]);
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
				printf( ANSI_COLOR_GREEN "The default value will be used for parameter %s.\n\n"
                        ANSI_COLOR_RESET, parametersNames[0]);
			}
			else if (sscanf(line, "%f", &userMapValues.parameters[0]) != 1) {
				printf(ANSI_COLOR_GREEN "Please enter a valid number.\n\n" ANSI_COLOR_RESET);
				goto CHOOSE_PARAM1bis;
			}
		}
    }
    /*  At this stage we have handled the first parameter. The following loop will handle the rest, if any. */
	for (size_t p = 1; p < numberParameters; p++) {
CHOOSE_PARAMS:
		if (fgets(line, sizeof(line), stdin)) {
			if(line[0] == '\n') {
				userMapValues.parameters[p] = defaultMapValues.parameters[p];
				printf( ANSI_COLOR_GREEN "The default value will be used for variable %s.\n\n"
                        ANSI_COLOR_RESET, parametersNames[p]);
			}
			else if (sscanf(line, "%f", &userMapValues.parameters[p]) != 1) {
				printf(ANSI_COLOR_GREEN "Please enter a valid number.\n\n" ANSI_COLOR_RESET);
				goto CHOOSE_PARAMS;
			}
		}
	}

}
