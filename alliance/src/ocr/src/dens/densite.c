#include <stdlib.h>
#include MUT_H
#include MLO_H
#include MLU_H
#include MPH_H
#include MPU_H
#include <string.h>

//#define SITE_SIZE		300.0
#define SITE_SIZE		100.0
#define SCALE_X		100.0
#define PITCH			5.0
#define FEN				(SITE_SIZE / PITCH)
#define FEN2			(FEN * FEN)

phfig_list *l_pPhFig;

#define DHOR(x,y)	   l_tDensHor[x+(y)*(l_nXMax-0)]
#define DVER(x,y)	   l_tDensVert[x+(y)*(l_nXMax-0)]

long
getX (long i_nX)
{
  return (i_nX - l_pPhFig->XAB1) / SCALE_X / SITE_SIZE;
}

long
getY (long i_nY)
{
  return (i_nY - l_pPhFig->YAB1) / SCALE_X / SITE_SIZE;
}

long
getMin (int i_nXY)
{
  return i_nXY * SITE_SIZE * SCALE_X;
}

long
getMax (int i_nXY)
{
  return (i_nXY + 1) * SITE_SIZE * SCALE_X - 1;
}


int
main (int argc, char **argv)
{
  FILE *l_pDataFile;
  phseg_list *l_pPhSeg;
  long i = 0;
  long j = 0;
  double *l_tDensHor;
  double *l_tDensVert;
  char l_sName[50];
  long l_nXMax;
  long l_nYMax;


  if (argc != 3)
    {
	 fprintf (stderr, "usage : densite src dest\n");
	 fprintf (stderr, "src  : fichier ap\n");
	 fprintf (stderr, "dest : fichier gp\n");

	 exit (1);
    }

  // initialisation de MBK
  mbkenv ();

  // Lecture de la vue physique
  l_pPhFig = getphfig (argv[1], 'A');

  for (l_pPhSeg = l_pPhFig->PHSEG;
	  l_pPhSeg;
	  l_pPhSeg = l_pPhSeg->NEXT)
    i++;

  printf ("%ld segments\n", i);

  printf ("(%ld,%ld) (%ld,%ld)\n", l_pPhFig->XAB1, l_pPhFig->YAB1,
		l_pPhFig->XAB2, l_pPhFig->YAB2);

  l_nXMax = getX (l_pPhFig->XAB2) + 1;
  l_nYMax = getY (l_pPhFig->YAB2) + 1;

  printf ("XMax = %ld\n", l_nXMax);
  printf ("YMax = %ld\n", l_nYMax);

  l_tDensHor = (double *) mbkalloc (sizeof (double) * l_nXMax * l_nYMax);
  l_tDensVert = (double *) mbkalloc (sizeof (double) * l_nXMax * l_nYMax);

  for (i = 0; i < l_nXMax; i++)
    for (j = 0; j < l_nYMax; j++)
	 {
	   DHOR (i, j) = 0.0;
	   DVER (i, j) = 0.0;
	 }

  for (l_pPhSeg = l_pPhFig->PHSEG;
	  l_pPhSeg;
	  l_pPhSeg = l_pPhSeg->NEXT)
    {
	 switch (l_pPhSeg->LAYER)
	   {
	   case ALU2:
	   case ALU4:
	   case ALU6:
/*
   viewphseg(l_pPhSeg);
   printf("f1 : (%ld,%ld) f2 : (%ld,%ld)\n",
   getX (l_pPhSeg->X1),getX (l_pPhSeg->Y1),
   getX (l_pPhSeg->X2),getX (l_pPhSeg->Y2));
 */
		for (i = getX (l_pPhSeg->X1); i <= getX (l_pPhSeg->X2); i++)
		  {
		    double res;

		    if (i == getX (l_pPhSeg->X1))
			 {
			   if (i == getX (l_pPhSeg->X2))
				{
				  res = ((double) (l_pPhSeg->X2 - l_pPhSeg->X1)
					    / SCALE_X / PITCH + 1) / FEN2;
				}
			   else
				{
				  res = ((double) (getMax (i) - l_pPhSeg->X1)
					    / SCALE_X / PITCH + 1) / FEN2;
				}
			 }
		    else
			 {
			   if (i == getX (l_pPhSeg->X2))
				{
				  res = ((double) (l_pPhSeg->X2 - getMin (i))
					    / SCALE_X / PITCH + 1) / FEN2;
				}
			   else
				{
				  res = 1.0 / FEN;
				}
			 }
		    DHOR (i, getY (l_pPhSeg->Y1)) += res;
		  }
		break;

	   case ALU3:
	   case ALU5:
	   case ALU7:
/*
   viewphseg(l_pPhSeg);
   printf("f1 : (%ld,%ld) f2 : (%ld,%ld)\n",
   getX (l_pPhSeg->X1),getX (l_pPhSeg->Y1),
   getX (l_pPhSeg->X2),getX (l_pPhSeg->Y2));
 */
		for (i = getY (l_pPhSeg->Y1); i <= getY (l_pPhSeg->Y2); i++)
		  {
		    double res;

		    if (i == getY (l_pPhSeg->Y1))
			 {
			   if (i == getY (l_pPhSeg->Y2))
				{
				  res = ((double) (l_pPhSeg->Y2 - l_pPhSeg->Y1)
					    / SCALE_X / PITCH + 1) / FEN2;
				}
			   else
				{
				  res = ((double) (getMax (i) - l_pPhSeg->Y1)
					    / SCALE_X / PITCH + 1) / FEN2;
				}
			 }
		    else
			 {
			   if (i == getY (l_pPhSeg->Y2))
				{
				  res = ((double) (l_pPhSeg->Y2 - getMin (i))
					    / SCALE_X / PITCH + 1) / FEN2;
				}
			   else
				{
				  res = 1.0 / FEN;
				}
			 }

		    DVER (getX (l_pPhSeg->X1), i) += res;
		  }
	   }
    }

  strcpy (l_sName, argv[2]);
  strcat (l_sName, ".gp");

  l_pDataFile = fopen (l_sName, "w");
  if (!l_pDataFile)
    {
	 printf ("Impossible d'écrire le fichier %s\n", argv[2]);
	 exit (1);
    }

  fprintf (l_pDataFile, "# densite de %s.ap\n\n", argv[1]);

  for (i = 0; i < l_nXMax; i++)
    {
	 for (j = 0; j < l_nYMax; j++)
	   {
		fprintf (l_pDataFile, "%ld\t%ld\t%f\t%f\n",
			    i, j, DHOR (i, j), DVER (i, j));
	   }
	 fprintf (l_pDataFile, "\n");
    }
  fclose (l_pDataFile);

  mbkfree (l_tDensHor);
  mbkfree (l_tDensVert);

  delphfig (*argv);
  exit (0);
}
