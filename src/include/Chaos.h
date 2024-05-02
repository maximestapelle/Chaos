#ifndef CHAOS_H
#define CHAOS_H


/*
 * This header is used by all functions of the program. It specifies data structures and function prototypes.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_GRAY    "\x1b[90m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define dimensionMax 3
#define numberParametersMax 5

/* SOLUTION WITH STRUCTURES ; CHANGES NOTHING TO PERFORMANCE. More elegant for the default/actual values ; not for the map as you havbe map.everything everywhere.

struct mapAttributes {
	int id;
	char name[25];
	char LaTeXformula[100];
	unsigned int dimension;
	char variablesNames[dimensionMax][15];
	unsigned int numberParameters;
	char parametersNames[numberParametersMax][15];
	char space[2];
	char type[10];
};
struct map_values {
	float userMapValues.IC[dimensionMax];
	float parameters[numberParametersMax];
	float parameterRange[2];
};

extern struct mapAttributes map;
extern struct map_values mapValues, mapDefaults;

*/
/********************/
/* Structures       */
struct mapValues {
	float IC[dimensionMax];
	float parameters[numberParametersMax];
	float parameterRange[2];
};

/********************/
/* GOBAL VARIABLES  */
// Database
extern char databaseName[];
extern sqlite3 * database;
//Map
extern int rowid;										 /* The entry in the database if the request was already made in the past */
extern int userMap;					 				 /* The user's choice of map */
extern char mapName[25];				 				 /* The name of the map */
extern unsigned int dimension; 							 /* The actual dimension of domain of map */
extern unsigned int numberParameters; 	 				 /* Actual number of parameters for the map */
extern char space[2]; 									 /* R (real) or C (complex) */
extern char type[10]; 									 /* discrete or flow */
extern char LaTeXformula[100]; 							 /* The LaTeX formula for the chosen map */
extern char variablesNames[][15]; 			 			 /* Whatever the dimension, we will store the variable LaTeX names as a R^3 vector. Subsequent
															programs will ignore non-existing dimensions.*/
extern char parametersNames[][15];					     /* Whatever the number of parameters, we will store the parameters LaTeX names as a NParamMax
												    		dimension vector. Subsequent programs will ignore non-existing dimensions.*/
//Action
extern int userAction;   	 				/* The user's choice of action */
char actionName[25]; 						/* The name of the requested action */
//Default and actual values
extern struct mapValues userMapValues, defaultMapValues;

extern long unsigned int NDefault;					  /* Default number of iterations */
extern long unsigned int N;							  /* Effective number of iterations */
extern long unsigned int SDefault;					  /* Default number of divisions of the range of parameter(s) for Bifurcation and Lyapunov */
extern long unsigned int S; 		  				  /* Effective number of divisions of the range of parameter(s) for Bifurcation and Lyapunov */

//Text to be displayed on plots or in file names
extern char summaryStdout[];     /* Those 3 are strings containing a summary of the request, for use in plot legend, stdout summary, */
extern char summaryPlotLegend[]; /*	and building the names of files. */
extern char summaryFileName[];
extern char dataFile[];
extern char imageFile[];
extern char commandOpenImageFile[];
//Parameters for physics
extern unsigned int maxPoints;					/* Number of points to keep after we consider we have converged (bifurcation) */
extern unsigned int minIterations;				/* Minimum of iterations we have to do to consider we have converged (bifurcation) */



/*************/
/* FUNCTIONS */
/* User Input functions */
int inputMaps();
int inputActions(int * NMin, long int * NMax, int * SMin, long int * SMax);
int inputDefaults();
bool inputChooseDefaults();
void inputIterations(unsigned int NMin, long unsigned int NMax);
void inputIC();
void inputParameterRange();
void inputSweep(unsigned int SMin, long unsigned int SMax);
void inputParameters();
void consolidateUserRequest();
/* Action functions */
void attractor();
void bifurcation();
void lyapunov();
void bifurcation2D();
/* Dynamics functions */
void logistic(float trajectory[]);
float logisticDerivative(float trajectory[]);
float logisticLyapunov(float trajectory[]);
void logisticExp(float trajectory[]);
float logisticExpDerivative(float trajectory[]);
float logisticExpLyapunov(float trajectory[]);
void gauss(float trajectory[]);
float gaussDerivative(float trajectory[]);
float gaussLyapunov(float trajectory[]);
void tent(float trajectory[]);
void circle(float trajectory[]);
float circleDerivative(float trajectory[]);
float circleLyapunov(float trajectory[]);
void tinkerbell(float trajectory[]);
void tinkerbellJacobian(float trajectory[], float jacobian[][dimension]);
void hénon(float trajectory[]);
void hénonJacobian(float trajectory[], float jacobian[][dimension]);
void lorenzEvolution(float input[], float fgh[]);
void lorenz(float trajectory[], float dt);
void lorenzRK4(float trajectory[], float h);
void rösslerEvolution(float input[], float fgh[]);
void rössler(float trajectory[], float dt);
void rösslerRK4(float trajectory[], float h);

/* Plots */
void plotAttractor();
void plotBifurcation();
void plotLyapunov();

/* File utilities */
void createFileName(); /* This procedure also creates a legend for the plots, containing the same data as the file names */

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
