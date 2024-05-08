#include "Chaos.h"

/*
 * The plotLyapunov function will hardcode a python file to be executed, which will produce a png file.
 *
 * The function is called from ChaosMain. 
 */

/*** !!! Only works for discrete maps, for flows there are a shit ton of exponents, syntax and plot ***/
void plotLyapunov() {

	char *pythonFile;
	char pythonFileContents[2500] = "import matplotlib.pyplot as plt\n";
	strcat(pythonFileContents, "import numpy as np\n");

	char command[35] = "python3 ";
	FILE *fp;
	int status;

	char title[150] = "Lyapunov exponents for the ";
	strcat(title, mapName);
	strcat(title, " ");
	strcat(title, LaTeXformula);
	
	pythonFile = "img/PlotLyapunov.py";
	fp = fopen(pythonFile, "w");
	
	strcat(pythonFileContents, "plt.style.use(\'dark_background\')\n");
	strcat(pythonFileContents, "plt.rcParams[\'text.usetex\'] = True\n");
	strcat(pythonFileContents, "plt.rcParams[\'axes.titlesize\'] = 22\n");
	strcat(pythonFileContents, "plt.rcParams[\'axes.labelsize\'] = 18\n");
	strcat(pythonFileContents, "plt.rcParams[\'xtick.labelsize\'] = 16\n");
	strcat(pythonFileContents, "plt.rcParams[\'ytick.labelsize\'] = 16\n");
	strcat(pythonFileContents, "plt.rcParams[\'grid.color\'] = \'gray\'\n");
	strcat(pythonFileContents, "plt.rcParams[\'text.color\'] = \'darkgray\'\n");
	strcat(pythonFileContents, "plt.rcParams[\'axes.labelcolor\'] = \'darkgray\'\n");
	strcat(pythonFileContents, "plt.rcParams[\'xtick.color\'] = \'tab:blue\'\n");
	strcat(pythonFileContents, "plt.rcParams[\'ytick.color\'] = \'tab:blue\'\n");
	strcat(pythonFileContents, "plt.rcParams[\'axes.edgecolor\'] = \'darkgray\'\n");

	strcat(pythonFileContents, "with open(\'");
	strcat(pythonFileContents, dataFile);
	strcat(pythonFileContents, "\') as f:\n");
	strcat(pythonFileContents, "\tlines = f.readlines()\n");
	strcat(pythonFileContents, "\tparam = [float(line.split()[0]) for line in lines]\n");
	for (size_t d = 1; d <= dimension; d++) {
		snprintf(pythonFileContents, sizeof(pythonFileContents), "%s\tlya%zu = [float(line.split()[%zu]) for line in lines]\n", pythonFileContents, d, d);
	}
	strcat(pythonFileContents, "lya = np.empty((2, len(param)))\nparam = np.array(param)\n");
	for (size_t d = 0; d < dimension; d++) {
	snprintf(pythonFileContents, sizeof(pythonFileContents), "%slya[%zu] = lya%zu\n", pythonFileContents, d, d + 1);
	
	}
	strcat(pythonFileContents, "ax = plt.figure(figsize = (16, 9), constrained_layout=True).add_subplot()\n");
	/* If only 1D, we don't print the variable name as there is only one exponent -> no ambiguity */
	if (dimension == 1) {
		strcat(pythonFileContents, "ax.plot(param[lya[0] < 0], lya[0][lya[0] < 0], \',\', color = (0, 0, 1), label = r\'Exponent when $<0$\')\n");
		strcat(pythonFileContents, "ax.plot(param[lya[0] > 0], lya[0][lya[0] > 0], \',\', color = (1, 0, 0), label = r\'Exponent when $>0$\')\n");
	}
	else {
		for (size_t d = 0; d < dimension; d++) {
			snprintf(pythonFileContents, sizeof(pythonFileContents), "%sax.plot(param[lya[%zu] < 0], lya[%zu][lya[%zu] < 0], \',\', color = (0, 0, (255 - 127 * %zu)/255), label = r\'Exponent along ", pythonFileContents, d, d, d, d);
			strcat(pythonFileContents, variablesNames[d]);
			strcat(pythonFileContents, ", when $<0$\')\n");
			snprintf(pythonFileContents, sizeof(pythonFileContents), "%sax.plot(param[lya[%zu] > 0], lya[%zu][lya[%zu] > 0], \',\', color = ((255 - 127 * %zu)/255, 0, 0), label = r\'Exponent along ", pythonFileContents, d, d, d, d);
			strcat(pythonFileContents, variablesNames[d]);
			strcat(pythonFileContents, ", when $>0$\')\n");
		}
	}
	strcat(pythonFileContents, "ax.set_xlabel(r\'Values of the parameter ");
	strcat(pythonFileContents, parametersNames[0]);
	strcat(pythonFileContents, "\')\n");
	switch (dimension) {
		case 1:
			strcat(pythonFileContents, "ax.set_ylabel(r\'Value of the Lyapunov exponent\')\n");
			break;
		default:
			strcat(pythonFileContents, "ax.set_ylabel(r\'Value of all Lyapunov exponents\')\n");
			break;
	}
	strcat(pythonFileContents, "ax.set_title(r\'");
	strcat(pythonFileContents, title);
	strcat(pythonFileContents, "\')\n");
	strcat(pythonFileContents, "leg = ax.legend([r\'");
	strcat(pythonFileContents, summaryPlotLegend);
	strcat(pythonFileContents,"\'], fontsize = \'x-large\', loc = \'best\', handlelength=0)\n");
	if (dimension > 1) {
		strcat(pythonFileContents,"ax.add_artist(leg)\nleg1 = ax.legend(loc = \'best\', fontsize = \'x-large\', handlelength=1)\n");
	}
	strcat(pythonFileContents, "for item in leg.legend_handles:\n\titem.set_visible(False)\n");
	strcat(pythonFileContents, "ax.axhline(0.0, color='darkgray', linewidth=0.5, alpha = 0.3, linestyle = \'--\')\n");

	strcat(pythonFileContents, "plt.savefig(\'");
	strcat(pythonFileContents, imageFile);
	strcat(pythonFileContents, "\', format = \'png\')\n");
	
	
	fprintf(fp, "%s", pythonFileContents);
	fclose(fp);

	strcat(command, pythonFile);
	if ((status = system(command)) != 0) {
		printf("Issue when executing python command to plot the Lyapunov exponents!\n");
		exit(1);
	}
	remove(pythonFile);

	printf("Image file created: '%s'.\n\n", imageFile);
}
