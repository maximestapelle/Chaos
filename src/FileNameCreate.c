#include "Chaos.h"

void createFileName() {
	
	/* Based on summaryFileName, we create the data and image full names : */
	strcat(dataFile, actionName);
	strcat(dataFile, " ");
	strcat(dataFile, summaryFileName);
	strcat(dataFile, ".txt");

	strcat(imageFile, actionName);
	strcat(imageFile, " ");
	strcat(imageFile, summaryFileName);
	strcat(imageFile, ".png");

}
