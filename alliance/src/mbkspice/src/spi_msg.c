/* 
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail             : mailto:alliance-users@asim.lip6.fr
 * 
 * This library is free software; you  can redistribute it and/or modify it
 * under the terms  of the GNU Library General Public  License as published
 * by the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 * 
 * Alliance VLSI  CAD System  is distributed  in the hope  that it  will be
 * useful, but WITHOUT  ANY WARRANTY; without even the  implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 * 
 * You should have received a copy  of the GNU General Public License along
 * with the GNU C Library; see the  file COPYING. If not, write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */


/*******************************************************************************
*                                                                              *
*  Tool        : Spice parser / driver v 7.00                                  *
*  Author(s)   : Gregoire AVOT                                                 *
*  Updates     : March, 18th 1998                                              *
*                                                                              *
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "spi_msg.h"

#include <mut.h>

char *spimsg[NBLANG][NBMSG] =
            {
/* English */ { 
  /*  0 */      "*** Error in parser driver Spice",
  /*  1 */      "Syntax error",
  /*  2 */      "Can't use .INCLUDE inside a subckt",
  /*  3 */      "Incorrect value for variable MBK_SPI_LANG : Using English",
  /*  4 */      "Can't find *interf in file",
  /*  5 */      "Internal error [1]",
  /*  6 */      "Identical nodes on subckt",
  /*  7 */      "Variable MBK_SPI_SEPAR must be a single character",
  /*  8 */      "Can't read lofig with this mode",
  /*  9 */      "Can't open file",
  /* 10 */      "Data structure not empty when delete",
  /* 11 */      "File",
  /* 12 */      "Line",
  /* 13 */      "Incomplete line",
  /* 14 */      "Too many element on line",
  /* 15 */      "Can't use subckt in another subckt",
  /* 16 */      "No name for subckt",
  /* 17 */      "Node for *ASIMNET must be number",
  /* 18 */      "No symbol '=' after node in *ASIMNET",
  /* 19 */      "No node in *ASIMNET",
  /* 20 */      "No name for node in *ASIMNET",
  /* 21 */      "Only one name supported in *ASIMNET",
  /* 22 */      ".ENDS without .SUBCKT",
  /* 23 */      "No name for subckt",
  /* 24 */      "Name for .ENDS differs than name of .SUBCKT",
  /* 25 */      "No name for transistor",
  /* 26 */      "Transistor is already defined",
  /* 27 */      "The transistor has no DRAIN node",
  /* 28 */      "The transistor has no GRID node",
  /* 29 */      "The transistor has no SOURCE node",
  /* 30 */      "The transistor has no BULK node",
  /* 31 */      "The transistor has no type",
  /* 32 */      "The transistor has a bad type",
  /* 33 */      "Parameter incomplete for transistor",
  /* 34 */      "Bad value",
  /* 35 */      "Parameter not supported",
  /* 36 */      "No name for resistor",
  /* 37 */      "Resistor already defined in subckt",
  /* 38 */      "No name for capacitance",
  /* 39 */      "Capacitance already defined in subckt",
  /* 40 */      "No name for subckt",
  /* 41 */      "Subckt already defined",
  /* 42 */      "Unreconized element ignored",
  /* 43 */      "No model found",
  /* 44 */      "Number of node differs between model and instance",
  /* 45 */      "Signal is both VDD and VSS",
  /* 46 */      "Many signals are VSS",
  /* 47 */      "Many signals are VDD",
  /* 48 */      "Line too long",
  /* 49 */      "Incorrect value for node",
  /* 50 */      "Incorrect name for node",
  /* 51 */      "The node has already a name",
  /* 52 */      "Illegal signal name",
  /* 53 */      "File open",
  /* 54 */      "Circuit",
  /* 55 */      "Instance",
  /* 56 */      "Model",
  /* 57 */      "Too many model of transistors",
  /* 58 */      "Model name too long"
  
              },
/* French  */ {
  /*  0 */      "*** Erreur dans le parser driver Spice",
  /*  1 */      "Erreur de syntaxe",
  /*  2 */      "Un .INCLUDE n'est pas utilisable dans un subckt",
  /*  3 */      "Valeur incorrecte pour MBK_SPI_LANG : Utilisation du Francais",
  /*  4 */      "Impossible de trouver le *interf le fichier",
  /*  5 */      "Erreur interne [1] : Pas de locon correspondant au nom dans la liste de l'ordre des locons.",
  /*  6 */      "Connecteur identiques sur le subckt",
  /*  7 */      "La variable MBK_SPI_SEPAR doit etre un caractere seul",
  /*  8 */      "Mode de lecture de la lofig non supporte",
  /*  9 */      "Impossible d'acceder au fichier",
  /* 10 */      "Structure de donnee non nettoyee avant liberation",
  /* 11 */      "Fichier",
  /* 12 */      "Ligne",
  /* 13 */      "Ligne incomplete",
  /* 14 */      "Trop d'elements sur la ligne",
  /* 15 */      "Les subckt imbriques ne sont pas supportes",
  /* 16 */      "Pas de nom pour le subckt",
  /* 17 */      "Les noeuds dans les *ASIMNET doivent etre des nombres",
  /* 18 */      "Caractere '=' absent apres les noeuds de '*ASIMNET'",
  /* 19 */      "Noeud absent dans *ASIMNET",
  /* 20 */      "Pas de nom pour les noeuds dans *ASIMNET",
  /* 21 */      "Un seul nom doit etre donne dans *ASIMNET",
  /* 22 */      ".ENDS rencontre sans avoir le .SUBCKT correspondant",
  /* 23 */      "Le nom du subckt est absent",
  /* 24 */      "Le nom specifie dans le .ENDS n'est pas le meme que pour le .SUBCKT",
  /* 25 */      "Le nom du transistor n'est pas specifie",
  /* 26 */      "Le transistor existe deja dans ce subckt",
  /* 27 */      "Le transistor ne possede pas de DRAIN",
  /* 28 */      "Le transistor ne possede pas de GRILLE",
  /* 29 */      "Le transistor ne possede pas de SOURCE",
  /* 30 */      "Le transistor ne possede pas de SUBSTRAT",
  /* 31 */      "Type du transistor absent",
  /* 32 */      "Type du transistor incorrecte",
  /* 33 */      "Parametre du transistor incomplet",
  /* 34 */      "Valeur incorrect",
  /* 35 */      "Parametre non supporte",
  /* 36 */      "Le nom de la resistance n'est pas specifie",
  /* 37 */      "La resistance existe deja dans le subckt",
  /* 38 */      "Le nom de la capacite n'est pas specifie",
  /* 39 */      "La capacite existe deja dans le subckt",
  /* 40 */      "Pas de nom pour le subckt",
  /* 41 */      "Le subckt existe deja",
  /* 42 */      "Element non reconnu par le parser ignore",
  /* 43 */      "Modele non trouve",
  /* 44 */      "Le mombre de connecteurs differe entre le modele et l'instance",
  /* 45 */      "Un signal est a la fois VDD et VSS",
  /* 46 */      "Plusieurs equipotentielles sont VSS",
  /* 47 */      "Plusieurs equipotentielles sont VDD",
  /* 48 */      "Ligne trop longue",
  /* 49 */      "Valeur du noeud incorrect",
  /* 50 */      "Nom du noeud incorrect",
  /* 51 */      "Renommage d'un noeud qui possede un nom",
  /* 52 */      "Nom de signal vecteur incorrect",
  /* 53 */      "Ouverture du fichier",
  /* 54 */      "Circuit",
  /* 55 */      "Instance",
  /* 56 */      "Model",
  /* 57 */      "Trops de modeles de transistors",
  /* 58 */      "Nom de modele trop long"
              }
            };
            
int SPI_LANG = DEFAULT_SPI_LANG;

void spi_init_lang( void )
{
  char *env;

  env = mbkgetenv( "MBK_SPI_LANG" );

  if( env )
  {
    if( strcmp( env, "Francais" ) == 0 )
      SPI_LANG = 1;
    else
    if( strcmp( env, "English" ) == 0 )
      SPI_LANG = 0;
    else
    {
      SPI_LANG = DEFAULT_SPI_LANG;
      fflush( stdout );
      fprintf( stderr,"%s.\n", SPIMSG(3) );
    }
  }
}
