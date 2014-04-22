/*------------------------------------------------------------\
|                                                             |
| Tool    :                    RDSCIF                         |
|                                                             |
| File    :                 cif_error.c                       |
|                                                             |
| Authors :       Petrot Frederic & Jacomme Ludovic           |
|                   ---------                                 |
| Date    :    The king ^     04/07/93                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>
# include <mut.h>
# include "cif_error.h"

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
|                          ciferror                           |
|                                                             |
\------------------------------------------------------------*/

void ciferror( Error, Text, Value )

    char  Error;
    char *Text;
    long  Value;
{
    switch( Error )
    {
      case CIF_ERROR_BAD_TRANS      :

       (void)fprintf( stderr, 
                      "cif109: Unknown instance transformation on `%s'\n", 
                      Text );
      break;

      case CIF_ERROR_CONNECTOR_NAME :

         (void)fprintf( stderr, 
                        "cif109: Rds connector (%ld, %ld) has no name !\n", 
                        (long)Text, Value );
      break;

      case CIF_ERROR_FILE_OPEN      :

       (void)fprintf( stderr, 
                      "cif109: Could not open file `%s'\n", 
                      Text );
      break;

      case CIF_ERROR_FILE_CLOSE     :

       (void)fprintf( stderr, 
                      "cif109: Could not close file `%s'\n", 
                      Text );
      break;
     
      case CIF_ERROR_MODEL          :

       (void)fprintf( stderr, 
                      "cif109: Could not find model `%s'\n", 
                      Text );
      break;

      case CIF_ERROR_EXPECTED_SEMI  :

       (void)fprintf( stderr,
                      "cif109: Missing ';' line %ld\n",
                      Value );
      break;

      case CIF_ERROR_EXPECTED_SPACE :

       (void)fprintf( stderr,
                      "cif109: Missing space, line %ld\n",
                      Value );
      break;

      case CIF_ERROR_EXPECTED_NAME :

       (void)fprintf( stderr,
                      "cif109: Missing name, line %ld\n",
                      Value );
      break;

      case CIF_ERROR_EXPECTED_UNSIGNED :

       (void)fprintf( stderr,
                      "cif109: Missing unsigned integer, line %ld\n",
                      Value );
      break;

      case CIF_ERROR_EXPECTED_SIGNED :

       (void)fprintf( stderr,
                      "cif109: Missing signed integer, line %ld\n",
                      Value );
      break;

      case CIF_ERROR_EXPECTED_LAYER : 

       (void)fprintf( stderr,
                      "cif109: Missing layer name, line %ld\n",
                      Value );
      break;

      case CIF_ERROR_EXPECTED_MANHATTAN :

       (void)fprintf( stderr,
                      "cif109: Expected manhattan distance, line %ld\n",
                      Value );
      break;

      case CIF_ERROR_EXPECTED_EOF :

       (void)fprintf( stderr,
                      "cif109: End of file expected, line %ld\n",
                      Value );
      break;

      case CIF_ERROR_UNEXPECTED_EOF :

       (void)fprintf( stderr,
                      "cif109: Unexpected end of file, line %ld\n",
                      Value );
      break;

      case CIF_ERROR_UNSIGNED_OVERFLOW :

       (void)fprintf( stderr,
                      "cif109: Unsigned integer overflow, line %ld\n",
                      Value );
      break;

      case CIF_ERROR_SIGNED_OVERFLOW :

       (void)fprintf( stderr,
                      "cif109: Signed integer overflow, line %ld\n",
                      Value );
      break;

      case CIF_ERROR_LAYER_UNKNOWN :

       (void)fprintf( stderr,
                      "cif109: Layer unknown, line %ld\n",
                      Value );
      break;

      case CIF_ERROR_POLYGON_EMPTY :

       (void)fprintf( stderr,
                      "cif109: Polygon empty, line %ld\n",
                      Value );
      break;

      case CIF_ERROR_POLYGON_OVERFLOW :

       (void)fprintf( stderr,
                      "cif109: Polygon overflow, line %ld\n",
                      Value );
      break;

      case CIF_ERROR_POLYGON_NOT_REC :

       (void)fprintf( stderr,
                      "cif109: Polygon not rectangular, line %ld\n",
                      Value );
      break;

      case CIF_ERROR_SYMBOL_STARTED :

       (void)fprintf( stderr,
                      "cif109: Symbol has already started, line %ld\n",
                      Value );
      break;

      case CIF_ERROR_SYMBOL_NOT_STARTED :

       (void)fprintf( stderr,
                      "cif109: Symbol hasn't  started, line %ld\n",
                      Value );
      break;

      case CIF_ERROR_BAD_DEFINE :

       (void)fprintf( stderr,
                      "cif109: Bad define, line %ld\n",
                      Value );
      break;

      case CIF_ERROR_BAD_COMMAND :

       (void)fprintf( stderr,
                      "cif109: Bad command, line %ld\n",
                      Value );
      break;

      case CIF_ERROR_BAD_COMMENT :

       (void)fprintf( stderr,
                      "cif109: Bad comment, line %ld\n",
                      Value );
      break;

      case CIF_ERROR_BAD_CONNECTOR :

       (void)fprintf( stderr,
                      "cif109: Bad connector specification, line %ld\n",
                      Value );
      break;
     
      case CIF_ERROR_BAD_SYMBOL :

       (void)fprintf( stderr,
                      "cif109: Bad symbol specification, line %ld\n",
                      Value );
      break;

      case CIF_ERROR_SYMBOL_NOT_DEFINE :

       (void)fprintf( stderr,
                      "cif109: Symbol number %ld not define\n",
                      Value );
      break;

      case CIF_ERROR_BAD_MIRROR :

       (void)fprintf( stderr,
                      "cif109: Bad mirror specification, line %ld\n",
                      Value );
      break;
     
      case CIF_ERROR_NO_TOP_MODEL :

       (void)fprintf( stderr,
                      "cif109: there is no top model !\n"
                    );
      break;
    }

    EXIT( 1 );
}

/*------------------------------------------------------------\
|                                                             |
|                          cifwarning                         |
|                                                             |
\------------------------------------------------------------*/

void cifwarning( Warning, Text, Value )

    char  Warning;
    char *Text;
    long  Value;
{
    switch( Warning )
    {
      case CIF_WARNING_PHYSICAL_GRID :

       (void)fprintf( stderr,
                      "cif109: Rectangle not aligned on the physical grid !\n"
                    );
      break;

      case CIF_WARNING_UNCONNECTED_NODE :

       (void)fprintf( stderr,
                      "cif109: Node %s is not connected, in model %s !\n",
                      Text, (char *)Value 
                    );
      break;

      case CIF_WARNING_ROUND_FLASH :

       (void)fprintf( stderr,
                      "cif109: Command round flash ignored, line %ld !\n",
                      Value 
                    );
      break;

      case CIF_WARNING_USER_COMMAND :

       (void)fprintf( stderr,
                      "cif109: User command ignored, line %ld !\n",
                      Value 
                    );
      break;
    }
}
