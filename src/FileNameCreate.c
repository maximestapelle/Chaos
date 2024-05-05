#include "Chaos.h"
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

/*
 * The createFileName function creates the full filenames for data and image.
 * It also checks if the data/ and img/ directories exist, and creates them otherwise.
 * It is called from ChaosMain. 
 */

int createFileName() {
	
	int rc;
	
	/* Check if the directories exist */
	if (opendir(dataFile) == NULL) {
		if ((rc = mkdir(dataFile, 0777)) != 0) {
			printf("Error : unable to create directory %s.\n", dataFile);
			return 1;
		}
	}
	if (opendir(imageFile) == NULL) {
		if ((rc = mkdir(imageFile, 0777)) != 0) {
			printf("Error : unable to create directory %s.\n", imageFile);
			return 1;
		}
	}

	/* Based on summaryFileName, we create the data and image full names : */
	strcat(dataFile, actionName);
	strcat(dataFile, " ");
	strcat(dataFile, summaryFileName);
	strcat(dataFile, ".txt");

	strcat(imageFile, actionName);
	strcat(imageFile, " ");
	strcat(imageFile, summaryFileName);
	strcat(imageFile, ".png");

	return 0;
}
