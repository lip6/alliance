#include <dirent.h>
#include <iostream>
#include <popt.h>
#include "mut.h"
#include "A2Sdf.h"
using namespace std;

#define EXTENSION ".vbe"

static void version(void)
{
    cout << "a2sdf(" PACKAGE ") " VERSION << endl;
    cout << "The " PACKAGE " package comes with NO WARRANTY" << endl;
}

int
main(int argc, char** argv)
{
    int showUsage = 0;
    int showVersion = 0;
    const char* cellsDir = NULL;
    const char* sdfFileName  = NULL; 
    poptOption options[] = {
	{ "help", 'h', POPT_ARG_NONE, &showUsage, 0, "print this message", 0},
        { "version", 'v', POPT_ARG_NONE, &showVersion, 0, "print the version information", 0},
        { 0, 0, 0, 0, 0, 0, 0 }
    };

    poptContext context = poptGetContext("a2sdf", argc, (const char**)argv, options, 0);
    poptSetOtherOptionHelp(context, "<cellsdir> <sdffilename> ");

    if (int rc = poptGetNextOpt(context) < -1)
    {
	cerr << "simann: bad argument " << poptBadOption(context, POPT_BADOPTION_NOALIAS) << ": " << poptStrerror(rc) << endl;
        cerr << "Try `"  << argv[0] << " --help' for more information"         << endl;
        return 1;
    }

    if (showVersion)
    {
	version();
	return 0;
    }

    if (showUsage)
    {
	version();
	cout << endl;
	poptPrintHelp(context, stdout, 0);
	return 0;
    }
        
    if ((cellsDir = poptGetArg(context)) == 0 || poptPeekArg(context) == 0)
    {
	poptPrintUsage(context, stderr, 0);
	return 1;
    }

    if ((sdfFileName = poptGetArg(context)) == 0 || poptPeekArg(context) != 0) 
    {
	poptPrintUsage(context, stderr, 0);
	return 1;
    }

    poptFreeContext(context);

    alliancebanner ("A2Sdf", VERSION, "Alliance to SDF format translator", "2002", ALLIANCE_VERSION);


    DIR* dir = opendir (cellsDir);
    if (!dir)
    {
	cerr << "No directory " << cellsDir  << endl;
	return 1;
    }
    
    A2Sdf a2sdf(sdfFileName);

    int extensionlength = strlen(EXTENSION);

    struct dirent* entry = NULL;

    while ((entry = readdir (dir)) != NULL)
    {
	char* filename = entry->d_name;
	int filenamelength = strlen (filename);

	if (filenamelength <= extensionlength) continue;
	/* is extension of filename accepted */
	if (strncmp (filename + filenamelength - extensionlength,
		    EXTENSION, extensionlength)) continue;
	string cellName(filename);
	cellName.erase( cellName.find(EXTENSION));
	a2sdf.CreateTimingCell(cellName.c_str());
    }

    a2sdf.Dump();

    return 0;
}
