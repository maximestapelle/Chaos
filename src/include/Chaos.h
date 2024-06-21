#ifndef CHAOS_H
#define CHAOS_H

/*
    This header is used by all functions of the program. It specifies data structures and function prototypes.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#if defined(__linux__)
    #define PLATFORM_NAME "linux" 				// Debian, Ubuntu, Gentoo, Fedora, openSUSE, RedHat, Centos and others
#elif defined(__APPLE__)  && defined(__MACH__)
    #include <TargetConditionals.h>
    #if TARGET_OS_MAC == 1
        #define PLATFORM_NAME "osx"             // Apple OSX
	#endif
#endif

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_GRAY    "\x1b[90m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define DIMENSION_MAX 3
#define NUMBER_PARAMETERS_MAX 5

/****************/
/*  Structures  */
struct mapValues {
	float IC[DIMENSION_MAX];
	float parameters[NUMBER_PARAMETERS_MAX];
	float parameterRange[2];
};

/********************/
/* GOBAL VARIABLES  */
// Database
extern char databaseName[];
extern sqlite3 * database;
//Map
extern int rowid;                       /*  The entry in the database if the request was already made in the past   */
extern int userMap;					 	/*  The user's choice of map  */
extern char mapName[25];				/*  The name of the map */
extern unsigned int dimension; 			/*  The actual dimension of domain of map */
extern unsigned int numberParameters; 	/*  Actual number of parameters for the map  */
extern char space[2]; 					/*  R (real) or C (complex)  */
extern char type[10]; 					/*  discrete or flow */
extern char LaTeXformula[100]; 			/*  The LaTeX formula for the chosen map  */
extern char variablesNames[][15]; 		/*  Whatever the dimension, we will store the variable LaTeX names as a
                                            R^3 vector. Subsequent programs will ignore non-existing dimensions.    */
extern char parametersNames[][15];		/*  Whatever the number of parameters, we will store the parameters LaTeX names
                                            as a NParamMax dimension vector. Subsequent programs will ignore
                                            non-existing dimensions.    */
//Action
extern int userAction;                  /*  The user's choice of action */
extern char actionName[25]; 			/*  The name of the requested action */
//Default and actual values
extern struct mapValues userMapValues, defaultMapValues;

extern long unsigned int NDefault;			/*  Default number of iterations   */
extern long unsigned int N;					/*  Effective number of iterations */
extern long unsigned int SDefault;			/*  Default number of divisions of the range of parameter(s)   */
extern long unsigned int S; 		  		/*  Effective number of divisions of the range of parameter(s)    */

//Parameters for physics
extern unsigned int maxPoints;				/*  Number of points to keep after we consider we have converged
                                                (bifurcation). Changed by bifurcation2D */
//Text to be displayed on plots or in file names
extern char summaryStdout[];        /*  Those 3 are strings containing a summary of the request, for use in */
extern char summaryPlotLegend[];    /*	plot legend, stdout summary, and building the names of files. */
extern char summaryFileName[];
extern char dataFile[];
extern char imageFile[];
extern char commandOpenImageFile[];



/*************/
/* FUNCTIONS */
/* User Input functions */
int inputMaps();
int inputActions(
        unsigned int *NMin,
        long unsigned int *NMax,
        unsigned int *SMin,
        long unsigned int *SMax);
int inputDefaults();
bool inputChooseDefaults();
void inputIterations(
        const unsigned int NMin,
        const long unsigned int NMax);
void inputIC();
void inputParameterRange();
void inputSweep(
        const unsigned int SMin,
        const long unsigned int SMax);
void inputParameters();
void consolidateUserRequest();
/* Action functions */
void attractor();
void bifurcation(unsigned int NMin);
void lyapunov();
void bifurcation2D(unsigned int NMin);
/* Plots */
void plotAttractor();
void plotBifurcation();
void plotLyapunov();

/* File utilities */
int createFileName();

/* Database functions */
int dbConnect();
int dbCheckUseEntry();
void dbCreateUseEntry();
void dbInsertData();
void dbInsertImage();
void dbCreateDataFile();
void dbCreateImageFile();

/* File Utilities */
bool checkDataFile();
bool checkImageFile();

#endif
