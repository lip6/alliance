#include <stdio.h>

#ifndef LIB	
#define LIB "cns"
#endif

main (argc, argv)
	int             argc;
	char          **argv;
{
	char	cmd[80];
	char	nom[15];

	printf ("Programme d'installation de la librairie ");
	printf ("%s.\n\n", LIB);
        if (strcmp (getenv("HOST"), "heavy") != 0) {
                printf ("ERREUR : a lancer sur la station 'heavy'.\n\n");
                exit (1);
        };

	printf ("Quel est votre nom ? ");
	scanf  ("%s", nom);
	setuid (30136);
	seteuid (30136);
	setruid (30136);
	setgid (30100);
	setegid (30100);
	setrgid (30100);
	sprintf (cmd, "/users/outil1/generic/install %s %s", LIB, nom);
	execlp ("/bin/csh", "csh", "-c", cmd, (char *) NULL);
}
