/*
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail support     : mailto:alliance-support@asim.lip6.fr
 * 
 * This progam is  free software; you can redistribute it  and/or modify it
 * under the  terms of the GNU  General Public License as  published by the
 * Free Software Foundation;  either version 2 of the License,  or (at your
 * option) any later version.
 * 
 * Alliance VLSI  CAD System  is distributed  in the hope  that it  will be
 * useful, but WITHOUT  ANY WARRANTY; without even the  implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 * 
 * You should have received a copy  of the GNU General Public License along
 * with the GNU C Library; see the  file COPYING. If not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/******************************************************************************/
/*                                                                            */
/*                      Chaine de CAO & VLSI   Alliance                       */
/*                                                                            */
/*    Produit : STANDAD CELLS ROUTER                                          */
/*    Fichier : SCR_Type.h                                                    */
/*                                                                            */
/*    (c) copyright 1991 Laboratoire MASI equipe CAO & VLSI                   */
/*    Tous droits reserves                                                    */
/*    Support : e-mail cao-vlsi@masi.ibp.fr                                   */
/*                                                                            */
/*    Auteur(s) : El housseine REJOUAN                  le : 26/10/1991       */
/*                                                                            */
/*    Modifie par :                                     le : ../../....       */
/*    Modifie par :                                     le : ../../....       */
/*    Modifie par :                                     le : ../../....       */
/*                                                                            */
/******************************************************************************/

extern char *ScrNameIndex();
extern int SXMODE;
  
# define TRICE		((char *) "_")
# define SIGNAME        ((char *) "Sig_")
# define NOTNET		((long 0) 0)

# define IMPAIRE(a)     (((a) & 1) ? TRUE : FALSE)
# define MAX(a,b)	((a>b) ? a:b)
# define MIN(a,b) 	((a<b) ? a:b)
# define INF(x,y,z)	(((x - y) <= (z - x)) ? y : z)
# define LABS(x,y)	((x > y) ? (x - y) : (y - x))
# define PATH		'P'
# define CELL		'C'
# define DOWN		'D'
# define UP		'U'
# define BOOLEAN	int
# define FALSE		((int ) 0)
# define TRUE		((int ) 1)
# define VDDOFFSET	((long ) (long)(MBK_WIDTH_VDD/2) * SCALE_X)
# define VSSOFFSET	((long ) (long)(MBK_WIDTH_VSS/2) * SCALE_X)
# define VDDWIDTH	((long ) MBK_WIDTH_VDD * SCALE_X)
# define VSSWIDTH	((long ) MBK_WIDTH_VSS * SCALE_X)
# define RIGHTSUPPLY	(PITCH_X / MBK_TRACK_WIDTH_ALU1)
# define LEFTSUPPLY	(PITCH_X / MBK_TRACK_WIDTH_ALU1)
# define MIDDLESUPPLY	((long )(MBK_WIDTH_VDD/2) * SCALE_X)
# define CELLAB	        ((long ) MBK_Y_SLICE * SCALE_X)

typedef struct SCR_Figure 
	{
	 struct SCR_Line	*LINE;	
	 struct SCR_Signal	*SIG;	
	 struct SCR_Connector	*CON;
         struct SCR_Segment	*ALLOW;
        } Figure;

typedef struct SCR_Line 
	{
	 struct SCR_Line	*NEXT;
         char		 	*NAME;	        
         char		 	TYPE;	        
         long		 	X,Y;          
         long		 	WIDTH;       
         long     	 	HEIGTH;     
	 long			INDEX;
         struct SCR_Instance 	*INS;     
         struct chain		*SIG;       	
         struct chain 		*CON;      
         struct SCR_Segment	*ALLOWUSED;   
         struct chain 		*ALLOW;      
         struct SCR_Channel	*CHANNEL;	
        } Line;

typedef struct SCR_Signal 
	{
	 struct SCR_Signal	*NEXT;
         char			*NAME;
         long			INDEX;     
         char			TYPE;     
         struct chain		*CON;	
         struct chain		*LINE;
         long 		 	MAXCON;        
        } Signal;	

typedef struct SCR_Instance 
	{
	 struct SCR_Instance	*NEXT;
         char			*NAME;
         char			*FIGNAME;
         long		 	X,Y;
         long		 	WIDTH;	
         long		 	HEIGTH; 
	 char		 	TRANSF; 
         struct SCR_Connector 	*CON;	
         struct SCR_Segment 	*ALLOW; 
        } Instance;

typedef struct SCR_Connector 
	{
	 struct SCR_Connector	*NEXT;
         char			*NAME;
         char		 	TYPE;     	
         char             	DIRECTION;     
         char             	ORIENT;    
	 long            	X,Y; 
	 long             	WIDTH;	   
	 long             	ORDER;	/* tenir compte de l'ordre des con. physiques */   
	 char             	LAYER;	
	 struct SCR_Signal	*SIG;
	 struct SCR_Line	*LINE;     
	 struct SCR_Instance	*INST;	
        } Connector;

typedef struct SCR_Segment 
	{
	 struct SCR_Segment	*NEXT;
	 char            	*NAME;
	 long             	X1,Y1,X2,Y2;
	 long             	WIDTH;	    
	 char             	TYPE;	
	 char             	LAYER;
        } Segment;


typedef struct SCR_Channel
        {
         struct SCR_ConnectorList	*NORTH_LIST;
         struct SCR_ConnectorList	*SOUTH_LIST;
         struct SCR_ConnectorList	*WEST_LIST;
         struct SCR_ConnectorList	*EAST_LIST;
	 struct SCR_SegmentList		*H_SEGMENT;
	 struct SCR_SegmentList		*V_SEGMENT;
	 struct SCR_ViasList		*VIA;
	 long				WIDTH;
	 long				HEIGTH;
        } Channel ; 

