#include <stdio.h>
#include <time.h>

char *pstime(Time)
time_t Time;
{
#define TIMESIZE 19
static char FormatedTime[TIMESIZE]; /* mmm dd yyyy HH:MM */

#ifdef sparc
        strftime(FormatedTime, TIMESIZE, "%d %h %Y %R", localtime(&Time));
        return FormatedTime;
#else
        return "No time today";
#endif
}

main (argc, argv)
	int             argc;
	char          **argv;
{
	long	Time;
	char	nom[15];
	char	outil[15];
	char	cmd[80];
	char	date[30];
	int	heure;

	printf ("Programme d'installation d'outil ");
	printf ("en periode critique.\n\n");
	if (strcmp (getenv("HOST"), "heavy") != 0) {
		printf ("ERREUR : a lancer sur la station 'heavy'.\n\n");
		exit (1);
	};
	strcpy (date, pstime (time (&Time)));
	heure = atoi (date+12);
	printf ("Nous sommes le %s, il est %d heures environ.\n", date, heure);
	if ((heure > 8) && (heure < 18)) {
		printf ("ERREUR : Recommencer apres 17h59.\n");
		exit (1);
	};
	
	printf ("Quel est le compte a installer ? ");
	scanf  ("%s", outil);
	strcpy (nom, getenv("USER"));
	setuid (30136);
	seteuid (30136);
	setruid (30136);
	setgid (30100);
	setegid (30100);
	setrgid (30100);
	sprintf (cmd, "/labo/sys/install %s %s", outil, nom);
	execlp ("/bin/csh", "csh", "-c", cmd, (char *) NULL);
}
