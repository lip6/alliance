/*------------------------------------------------------------\
|                                                             |
| Tool    :                     RDS                           |
|                                                             |
| File    :                   rdsadd.c                        |
|                                                             |
| Authors :                Jacomme Ludovic                    |
|                                                             |
| Date    :                   27.06.95                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>
# include <string.h>

# include <mut.h>
# include "rds.h"

# include "rdsadd.h"
# include "rdserror.h"

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                            Types                            |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      Rds Add Functions                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        Rds Add Figure                       |
|                                                             |
\------------------------------------------------------------*/

rdsfig_list *addrdsfig( Name, Size )

   char           *Name;
   unsigned short  Size; 
{
  rdsfig_list *Figure;

  Figure = allocrdsfig();

  Figure->NAME  = namealloc( Name );
  Figure->SIZE  = Size;
  Figure->MODE  = 'A';
  Figure->NEXT  = HEAD_RDSFIG;

  HEAD_RDSFIG   = Figure;

  return( Figure );
}

/*------------------------------------------------------------\
|                                                             |
|                        Rds Add Instance                     |
|                                                             |
\------------------------------------------------------------*/

rdsins_list *addrdsins( Figure, Model, Name, Sym, X, Y )

   rdsfig_list *Figure;
   char        *Model;
   char        *Name;
   char         Sym;
   long         X; 
   long         Y; 
{
  rdsins_list *Instance;

  Instance = allocrdsins();

  Instance->FIGNAME = namealloc( Model );
  Instance->INSNAME = namealloc( Name  );
  Instance->X       = X;
  Instance->Y       = Y;
  Instance->TRANSF  = Sym;
  Instance->SIZE    = Figure->SIZE;
  Instance->NEXT    = Figure->INSTANCE;

  Figure->INSTANCE  = Instance;

  return( Instance );
}

/*------------------------------------------------------------\
|                                                             |
|                  Rds Add Rectangle in Instance              |
|                                                             |
\------------------------------------------------------------*/

rdsrec_list *addrdsinsrec( Instance, Name, Layer, X, Y, Dx, Dy )

   rdsins_list *Instance;
   char        *Name;
   char         Layer;
   long         X;
   long         Y;
   long         Dx;
   long         Dy;
{
  rdsrec_list *Rectangle;
 
  Rectangle = allocrdsrec( Instance->SIZE );
 
  Rectangle->NAME  = namealloc( Name );
  Rectangle->X     = X;
  Rectangle->Y     = Y;
  Rectangle->DX    = Dx;
  Rectangle->DY    = Dy;
  Rectangle->FLAGS = (unsigned char)(Layer);
  Rectangle->NEXT  = Instance->LAYERTAB[ (unsigned int)Layer ];
  
  Instance->LAYERTAB[ (unsigned int)Layer ] = Rectangle;

  ClearRdsFigRec( Rectangle );

  return( Rectangle );
}

/*------------------------------------------------------------\
|                                                             |
|                  Rds Add Rectangle in Figure                |
|                                                             |
\------------------------------------------------------------*/

rdsrec_list *addrdsfigrec( Figure, Name, Layer, X, Y, Dx, Dy )

   rdsfig_list *Figure;
   char        *Name;
   char         Layer;
   long         X;
   long         Y;
   long         Dx;
   long         Dy;
{
  rdsrec_list *Rectangle;
 
  Rectangle = allocrdsrec( Figure->SIZE );
 
  Rectangle->NAME  = namealloc( Name );
  Rectangle->X     = X;
  Rectangle->Y     = Y;
  Rectangle->DX    = Dx;
  Rectangle->DY    = Dy;
  Rectangle->FLAGS = (unsigned char)(Layer);
  Rectangle->NEXT  = Figure->LAYERTAB[ (unsigned int)Layer ];
  
  Figure->LAYERTAB[ (unsigned int)Layer ] = Rectangle;

  SetRdsFigRec( Rectangle );

  return( Rectangle );
}

