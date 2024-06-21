/**********************************************************************************************************
 **     CHAOS application                                                                                 *
 **                                                                                                       *
 ** Generates data and plots with Bifurcation figures, Lyapunov exponents, Attractors of chaotic maps     *
 **                                                                                                       *
 ** AUTHOR   : Maxime Stapelle                                                                            *
 ** CREATED  : 24 March - 10 April 2024                                                                   *
 **                                                                                                       *
 ** Modified : 2 May 2024 Changed matrix definition and allocation in utilities.						  *
 **						  Improved comments.															  *
 ** 					  Added OS identification for image opening										  *
 **																										  *
 **			   5 May 2024 Fixed the crash when data/ or img/ directories don't exist					  *
 **						  Added an "active flag" set to false in (map/action)'s where not ready. This is  *
 **						  more for development purposes where I can have the entry in the database,       *
 **						  not usable in working version but testable when I'm implementing it.			  *
 **						  Added the same flag at action level (Lyapunov2D not ready)					  *
 **			  10 May 2024 Improved bifurcation graphs													  *
 **			  27 May 2024 -> 31 May 2024																  *
 **						  Specific header for dynamics.													  *
 **						  float to double when applicable.												  *
 **						  Implementation of Lyapunov for flows											  *
 **						  Small change in the handling of LaTeX variable names in plots					  *
 **						  Small bug fix : selecting unavailable action was possible						  *
 **						  History: even if the insertion of usage history in the database is deactivated, *
 **								   check if data or img files exist for the request.					  *
 **			  20 Jun 2024 Added hardcoding of stateDimension in RK4Full in Dynamics.c, since this version *
 **						  of Chaos uses a lot of hardcoding that improves performance. Also added		  *
 **						  explicit loop unrolling and safer optimization level -O2						  *
 **						  Actually, full hardcoding of all dimensions to improve performance.			  *
 **			  21 Jun 2024 Implement use of NMin from the database for minimum iterations				  *
 **						  Implement use of h, T from database for RK4									  *
 **						  Aesthetic changes																  *
 **						  Fix small bug in bifurcation (check on maxPoints)								  *
 **********************************************************************************************************/

// TODO :
// - Improve error handling which is very bad for the moment.
// - I've deactivated the historical data in the database, as the problematic of uploading data files into
//	 a database is non trivial. Sort it out. If reactivated, maintenance from the user has to be done once
//	 in a while as the size of the database can become huge if the user gets carried away.
// - Plots for Bifurcation2D.
// - Bifurcations : more parameters to be put in the table (eg dt/h for flows, minimum iterations).
// - Lyapunov exponents to improve.
// - The user should choose if he wants to display the image file or not. But it will be better with a UI.

#include "Chaos.h"

int main() {

	unsigned int NMin; long unsigned int NMax; 		/*	Interval of allowed values for N */
	unsigned int SMin; long unsigned int SMax; 		/*	Interval of allowed values for S */
	bool choseDefaults;								/*	Does the user choose the default values for his request ? */


	if (dbConnect()) exit(1); 						/*	Connect to the database */

	if (inputMaps()) exit(1);						/*	Display maps, fetch user input and store information
														for the chosen map */
	if (inputActions(&NMin, &NMax, &SMin, &SMax)) {	/*	Display actions, fetch user input and store information */
		exit(1);									/*	for the chosen action. */
	}

	if (inputDefaults()) exit(1); 					/*	Store all default values */
	choseDefaults = inputChooseDefaults(); 			/*	Ask if the user wants the default values */
	if (choseDefaults) goto INPUT_DONE;
	else {
		printf(ANSI_COLOR_BLUE	"\nFor each of the values asked below, we will consider "
								"the default values if you just press Enter.\n" ANSI_COLOR_RESET);
	}

	inputIterations(NMin, NMax); 			/*	Fetch the user's desired number of iterations */

	inputIC(); 								/*	Fetch the user's initial conditions */

	if (userAction != 3) {					/*	Only if the user wants a bifurcation figure or the Lyapunov exponents */
		inputParameterRange(); 					/*	Fetch the user's desired range for the principal parameter */
		inputSweep(SMin, SMax); 				/*	Fetch the user's desired division of the parameter */
	}

	inputParameters(); 						/*	Fetch the user's input parameters : all of them if attractor,
												and the remaining ones - if any - for other actions. */

/*** We're done with user input ! ***/
INPUT_DONE:
	consolidateUserRequest(); 						/*	Create the summaries of the request */
	if (createFileName()) exit(1);					/*	Check directories and create the name of the files */

	if (!choseDefaults)	{							/*	Print Summary of the user's request on the console */
		printf(	ANSI_COLOR_CYAN "Here below a summary of your choices:\n\n%s\n\n"
				ANSI_COLOR_RESET, summaryStdout);
	}

/* Check if the requested action already exists; if not, we're in business ! DEACTIVATED on database level */
// 	rowid = dbCheckUseEntry();
// 	if (rowid == 0)	{
	bool dataExists = checkDataFile();
	bool  imgExists = checkImageFile();

	if (dataExists || imgExists) {
		printf(	ANSI_COLOR_CYAN "The request you made has already been done in the past.\n"
				ANSI_COLOR_RESET);

		if (dataExists)	{
			printf("Data file found: %s\n", dataFile);
		}
		else printf("Data file not found. Recreating from scratch.\n");

		if (imgExists)	{
			printf("Image file found: %s\n", imageFile);
		}
		else printf("Image file not found. Recreating from data file.\n");
	}

	if (!dataExists || !imgExists) {
// 		dbCreateUseEntry();					/* No entry exists -> create one // DEACTIVATED */

		switch (userAction) {				/* Now we decide what to call based on the requested action */
			case 1:
				if (!dataExists) bifurcation(NMin);
	    		if (!imgExists ) plotBifurcation();
				break;
	    	case 2:
	    		if (!dataExists) lyapunov();
	    		if (!imgExists ) plotLyapunov();
	    		break;
			case 3:
				if (!dataExists) attractor();
				if (!imgExists ) plotAttractor();
				break;
// 	    	case 4:
// 	    		bifurcation2D(NMin);
// 	    		plotBifurcation2D();
// 	    		break;
// 	    	case 5:
// 	    		lyapunov2D();
// 	    		plotLyapunov2D();
// 	    		break;
		}

												/* DEACTIVATED */
// 		dbInsertData();							/* INSERT the data file into the database */
// 		dbInsertImage();						/* INSERT the image file and image into the database */

	}
// 	else {
// 		printf(ANSI_COLOR_GREEN"The request you made has already been done in the past.\n"ANSI_COLOR_RESET);
// 		/* Check if the files exist and if not, create them from the database -> DEACTIVATED */
// 		if (!checkDataFile()) dbCreateDataFile();
// 		if (!checkImageFile()) dbCreateImageFile();
// 		printf("Data file: %s\n", dataFile);
// 		printf("Image file: %s\n", imageFile);
// 	}

	/* Open the image file */
	if (strcmp(PLATFORM_NAME, "linux") == 0) strcat(commandOpenImageFile,"xdg-open \"");
	else if (strcmp(PLATFORM_NAME, "osx") == 0) strcat(commandOpenImageFile,"open \"");
	else {
		printf("Unknown OS. Don't know how to open image file.\n");
		goto END;
	}
	strcat(commandOpenImageFile, imageFile);
	strcat(commandOpenImageFile, "\"");
	system(commandOpenImageFile);

END:
	sqlite3_close(database);
	return 0;
}
