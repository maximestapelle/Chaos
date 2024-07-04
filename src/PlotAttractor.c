#include "Chaos.h"

/*
	The plotAttractor function will hardcode a python file to be executed, which will produce a png file.

	The function is called from ChaosMain.
 */

void plotAttractor() {

	char *pythonFile = "img/PlotAttractor.py";
	FILE *fp = fopen(pythonFile, "w");
	char command[30] = "python3 ";
	strcat(command, pythonFile);

	char pythonFileContents[2500] = "import matplotlib.pyplot as plt\n";

	char buffer[200];
	char title[200] = "Attractor for the ";
	strcat(title, mapName);
	strcat(title, " ");
	strcat(title, LaTeXformula);


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
	strcat(pythonFileContents, "\tx = [float(line.split()[0]) for line in lines]\n");
	strcat(pythonFileContents, "\ty = [float(line.split()[1]) for line in lines]\n");
	if (dimension == 3) {
		strcat(pythonFileContents, "\tz = [float(line.split()[2]) for line in lines]\n");
	}

	strcat(pythonFileContents, "ax = plt.figure(figsize = (16, 9), constrained_layout=True).add_subplot(");
	if (dimension == 3) {
		strcat(pythonFileContents,  "projection = \'3d\')\n");
		strcat(pythonFileContents,  "ax.xaxis.pane.fill = False\n"
									"ax.yaxis.pane.fill = False\n"
									"ax.zaxis.pane.fill = False\n");
	}
	else strcat(pythonFileContents, ")\n");
	strcat(pythonFileContents, "ax.plot(x, y, ");
	if (dimension == 3) {
		strcat(pythonFileContents, "z, ");
	}
	strcat(pythonFileContents, "\',\', color = \'cornflowerblue\')\n");
	strcat(pythonFileContents, "ax.set_xlabel(r\'Value of $");
	strcat(pythonFileContents, variablesNames[0]);
	strcat(pythonFileContents, "$\')\n");
	strcat(pythonFileContents, "ax.set_ylabel(r\'Value of $");
	strcat(pythonFileContents, variablesNames[1]);
	strcat(pythonFileContents, "$\')\n");
	if (dimension == 3) {
		strcat(pythonFileContents, "ax.set_zlabel(r\'Value of $");
		strcat(pythonFileContents, variablesNames[2]);
		strcat(pythonFileContents, "$\')\n");
	}
	strcat(pythonFileContents, "ax.set_title(r\'");
	strcat(pythonFileContents, title);
	strcat(pythonFileContents, "\')\n");
	strcat(pythonFileContents, "leg = ax.legend([r\'");
	strcat(pythonFileContents, summaryPlotLegend);
	if (requestCapacityDimension) {
		snprintf(buffer, sizeof (buffer), "\'+\'\\n\\n\'+r\'Estimation of the Capacity dimension: %.2f.", dCap);
		strcat(pythonFileContents, buffer);
	}
	strcat(pythonFileContents,"\'], fontsize = \'x-large\', loc = \'best\', handlelength=0)\n");
	strcat(pythonFileContents, "for item in leg.legend_handles:\n\titem.set_visible(False)\n");

	strcat(pythonFileContents, "plt.savefig(\'");
	strcat(pythonFileContents, imageFile);
	strcat(pythonFileContents, "\', format = \'png\')\n");

	fprintf(fp, "%s", pythonFileContents);
	fclose(fp);

	int status;
	if ((status = system(command)) != 0) {
		printf("Issue when executing python command to plot the Attractor!\n");
		exit(1);
	}
	remove(pythonFile);

	printf("Image file created: '%s'.\n\n", imageFile);
}
