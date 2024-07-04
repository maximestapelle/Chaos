#include "Chaos.h"


/********************/
/* GOBAL VARIABLES  */
// Database
char databaseName[] = "database/Chaos";
sqlite3 * database;
//Map
int rowid;                      /*  The entry in the database if the request was already made in the past   */
int userMap;					/*  The user's choice of map  */
char mapName[25];               /*  The name of the map */
unsigned int dimension; 		/*  The actual dimension of domain of map */
unsigned int numberParameters; 	/*  Actual number of parameters for the map  */
char space[2]; 					/*  R (real) or C (complex)  */
char type[10]; 					/*  discrete or flow */
int isDiscrete = 1;             /*  bool version of the latter  */
char LaTeXformula[100];                     /*  The LaTeX formula for the chosen map */
char variablesNames[DIMENSION_MAX][15];     /*  Whatever the dimension, we will store the variable LaTeX names as a
                                                R^3 vector. Subsequent programs will ignore non-existing dimensions.  */
char parametersNames[NUMBER_PARAMETERS_MAX][15];/*  Whatever the number of parameters, we will store the parameters
                                                    LaTeX names as a NParamMax dimension vector. Subsequent programs
                                                    will ignore non-existing dimensions.  */
//Action
int userAction;                     /*  The user's choice of action */
char actionName[25]; 			    /*  The name of the requested action */
int requestCapacityDimension = 0;   /*  For Attractor: user may request an estimation of the capacity dimension;  */
float dCap;                         /*          default value is No.  */
//Default and actual values
struct mapValues userMapValues, defaultMapValues;
long unsigned int NDefault;     /*  Default number of iterations */
long unsigned int N;            /*  Effective number of iterations */
long unsigned int SDefault;     /*  Default number of divisions of the range of parameter(s)  */
long unsigned int S;            /*  Effective number of divisions of the range of parameter(s)  */

float h;                    /*  For flows: timestep */
unsigned int T;             /*  For flows and lyapunov: mutliplicative term */

//Parameters for physics
unsigned int maxPoints = 500;   /*  Number of points to keep after we consider we have converged
                                    (bifurcation). Changed by bifurcation2D */

//Text to be displayed on plots or in file names
char summaryStdout[500] = "";       /*  Those 3 are strings containing a summary of the request, for use in */
char summaryPlotLegend[350] = "";   /*  plot legend, stdout summary, and building the names of files. */
char summaryFileName[200] = "";
char dataFile[210] = "data/";
char imageFile[210] = "img/";
char commandOpenImageFile[250] = "";
