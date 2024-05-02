/***********************************************************************************************************
 **     CHAOS application                                                                                  *
 **                                                                                                        *
 ** Generates data and plots with Bifurcation figures, Lyapunov exponents, Attractors of chaotic maps      *
 **                                                                                                        *
 ** Author : Maxime Stapelle                                                                               *
 ** Created : 24th March - 10 April 2024                                                                   *
 **                                                                                                        *
 **********************************************************************************************************/


#include "Chaos.h"


int main() {

	int NMin; long int NMax; /* interval of allowed values for N */
	int SMin; long int SMax; /* interval of allowed values for S */
	bool choseDefaults;
 
	
	if (dbConnect()) exit(1); 	/* Connect to the database */

	if (inputMaps()) exit(1);	/* Display maps, fetch user input and store information for the chosen map */

	if (inputActions(&NMin, &NMax, &SMin, &SMax)) exit(1);	/* Display actions, fetch user input and store information for the chosen action. */

	if (inputDefaults()) exit(1); /* Store all default values */
	
	choseDefaults = inputChooseDefaults(); /* Ask if the user wants the default values */
	if (choseDefaults) goto INPUT_DONE;
	else printf(ANSI_COLOR_BLUE "\nFor each of the values asked below, we will consider the default values if you just press Enter.\n" ANSI_COLOR_RESET);
	
	inputIterations(NMin, NMax); /* Fetch the user's desired number of iterations */
	
	inputIC(); /* Fetch the user's initial conditions */
	
	if (userAction != 3) {
		inputParameterRange(); /* Fetch the user's desired range for the principal parameter */
		inputSweep(SMin, SMax); /* Fetch the user's desired division of the parameter for bifurcation/Lyapunov */
	}

	inputParameters(); /* Fetch the user's input parameters : all of them if attractor, and the remaining ones - if any - for other actions. */


/*** We're done with user input ! ***/
INPUT_DONE:
	consolidateUserRequest(); /* Create the summaries of the request */
	createFileName();		  /* Create the name of files */
	
	if (!choseDefaults)	printf(ANSI_COLOR_CYAN "Here below a summary of your choices:\n\n%s\n\n" ANSI_COLOR_RESET, summaryStdout); /* Print Summary */
	
	/* Check if the requested action already exists; if not, we're in business ! */
// 	rowid = dbCheckUseEntry();
	if (rowid == 0)	{
		/* AT LAST, do something :-) */
// 		dbCreateUseEntry();

		/* First assign value to variables that depend on the user's choices */
		minIterations = N - maxPoints;
		
		switch (userAction) {
			case 1:
				bifurcation();
	    		plotBifurcation();
				break;
	    	case 2:
	    		lyapunov();
	    		plotLyapunov();
	    		break;
			case 3:
				attractor();
				plotAttractor();
				break;
	    	case 4:
			{
	    		bifurcation2D();
// 	    		plotBifurcation2D();
	    		break;
			}
	//     	case 5:
	// 		{
	//     		lyapunov2D();
	//     		plotLyapunov2D();
	//     		break;
	// 		}
		}

		/* INSERT the data file and image into the database */
// 		dbInsertData();
// 		dbInsertImage();

	}
	else {
		printf(ANSI_COLOR_GREEN"The request you made has already been done in the past.\n"ANSI_COLOR_RESET);
		/* Check if the files exist and if not, create them */
		if (!checkDataFile()) dbCreateDataFile();
		if (!checkImageFile()) dbCreateImageFile();
		printf("Data file: %s\n", dataFile);
		printf("Image file: %s\n", imageFile);
	}
	
	/* Open the image file */
	strcat(commandOpenImageFile, imageFile);
	strcat(commandOpenImageFile, "\"");
	system(commandOpenImageFile);

	sqlite3_close(database);
	return 0;
}
