/*------------------------------------------------------------\
|                                                             |
| Tool    :                     RDS                           |
|                                                             |
| File    :                   rdsdel.c                        |
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

# include "rdsdel.h"
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
|                        Rds Del Functions                    |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        Rds Del Figure                       |
|                                                             |
\------------------------------------------------------------*/

int delrdsfig( Name )

   char *Name;
{
  rdsfig_list  *Figure;
  rdsfig_list **Previous;
  rdsins_list  *Instance;
  rdsrec_list  *Rectangle;
  void         *Save;
  int           Layer;

  Name     = namealloc( Name );
  Previous = &HEAD_RDSFIG;

  for ( Figure  = HEAD_RDSFIG;
        Figure != (rdsfig_list *)NULL;
        Figure  = Figure->NEXT )
  {
    if ( Figure->NAME == Name ) break;

    Previous = &Figure->NEXT;
  }

  if ( Figure == (rdsfig_list *)NULL ) 
  
    return( 0 );

  *Previous = Figure->NEXT;

  for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
  {
    Rectangle  = Figure->LAYERTAB[ Layer ];

    while ( Rectangle != (rdsrec_list *)NULL )
    {
      Save = (void *)(Rectangle->NEXT);

      freerdsrec( Rectangle, Figure->SIZE );

      Rectangle = (rdsrec_list *)Save;
    }
  }

  Instance = Figure->INSTANCE;

  while ( Instance != (rdsins_list *)NULL )
  {
    for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
    {
      Rectangle  = Instance->LAYERTAB[ Layer ];
  
      while ( Rectangle != (rdsrec_list *)NULL )
      {
        Save = (void *)(Rectangle->NEXT);
  
        freerdsrec( Rectangle, Figure->SIZE );
  
        Rectangle = (rdsrec_list *)Save;
      }
    }

    Save = (void *)(Instance->NEXT);

    freerdsins( Instance );

    Instance = (rdsins_list *)Save;
  }
   
  freerdsfig( Figure );

  return( 1 );
}

/*------------------------------------------------------------\
|                                                             |
|                        Rds Del Instance                     |
|                                                             |
\------------------------------------------------------------*/

int delrdsins( Figure, Name )

   rdsfig_list *Figure;
   char        *Name;
{
  rdsins_list **Previous;
  rdsins_list  *Instance;
  rdsrec_list  *Rectangle;
  void         *Save;
  int           Layer;

  Name     = namealloc( Name );
  Previous = &Figure->INSTANCE;

  for ( Instance  = Figure->INSTANCE;
        Instance != (rdsins_list *)NULL;
        Instance  = Instance->NEXT )
  {
    if ( Instance->INSNAME == Name ) break;

    Previous = &Instance->NEXT;
  }

  if ( Instance == (rdsins_list *)NULL ) 
  
    return( 0 );

  *Previous = Instance->NEXT;

  for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
  {
    Rectangle  = Instance->LAYERTAB[ Layer ];

    while ( Rectangle != (rdsrec_list *)NULL )
    {
      Save = (void *)(Rectangle->NEXT);

      freerdsrec( Rectangle, Figure->SIZE );

      Rectangle = (rdsrec_list *)Save;
    }
  }

  freerdsins( Instance );

  return( 1 );
}
 
/*------------------------------------------------------------\
|                                                             |
|                   Rds Del Rectangle In Instance             |
|                                                             |
\------------------------------------------------------------*/

int delrdsinsrec( Instance, Rectangle )

   rdsins_list *Instance;
   rdsrec_list *Rectangle;
{
  rdsrec_list **Previous;
  rdsrec_list  *Scan;

  Previous = &Instance->LAYERTAB[ GetRdsLayer( Rectangle ) ];

  for ( Scan  = Instance->LAYERTAB[ GetRdsLayer( Rectangle ) ];
        Scan != (rdsrec_list *)NULL;
        Scan  = Scan->NEXT )
  {
    if ( Scan == Rectangle ) break;

    Previous = &Scan->NEXT;
  }

  if ( Scan == (rdsrec_list *)NULL ) 
  
    return( 0 );

  *Previous = Rectangle->NEXT;

  freerdsrec( Rectangle, Instance->SIZE );

  return( 1 );
}
 
/*------------------------------------------------------------\
|                                                             |
|                   Rds Del Rectangle In Figure               |
|                                                             |
\------------------------------------------------------------*/

int delrdsfigrec( Figure, Rectangle )

   rdsfig_list *Figure;
   rdsrec_list *Rectangle;
{
  rdsrec_list **Previous;
  rdsrec_list  *Scan;

  Previous = &Figure->LAYERTAB[ GetRdsLayer( Rectangle ) ];

  for ( Scan  = Figure->LAYERTAB[ GetRdsLayer( Rectangle ) ];
        Scan != (rdsrec_list *)NULL;
        Scan  = Scan->NEXT )
  {
    if ( Scan == Rectangle ) break;

    Previous = &Scan->NEXT;
  }

  if ( Scan == (rdsrec_list *)NULL ) 
  
    return( 0 );

  *Previous = Rectangle->NEXT;

  freerdsrec( Rectangle, Figure->SIZE );

  return( 1 );
}

