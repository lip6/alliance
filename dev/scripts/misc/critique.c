#define OUTIL "cheops"
#include <stdio.h>

main (argc, argv)
	int             argc;
	char          **argv;
{
	char	nom[15];
	char	cmd[80];

	printf ("Programme d'installation de l'outil ");
	printf ("%s en periode critique.\n\n", OUTIL);
	printf ("ATTENTION : lancer le batch sur la station MI.\n\n");
	printf ("Quel est votre nom ? ");
	scanf  ("%s", nom);
	setuid (30136);
	seteuid (30136);
	setruid (30136);
	setgid (30100);
	setegid (30100);
	setrgid (30100);
	sprintf (cmd, "/labo/sys/install %s %s", OUTIL, nom);
	execlp ("/bin/csh", "csh", "-c", cmd, (char *) NULL);
}
