/*******************************************************************************
*
* XNF driver from a lofig.
* Based on Nathtalie Dictus and Eudes Prado Lopez version for FPGA
* By Frederic Petrot
*
* 03/10/96	Traite TRANSCV, obuft doit etre explicite, le ibuf peut etre
*			explicie, sinon il est rajoute
* 07/10/96 ibuf et obuf peuvent etre explicites, sinon ils sont rajoutes 
* 03/11/2000 Traitement de zero_y, one_y
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mut.h>
#include <mlo.h>
#include <mlu.h>


void liste_model(lofig_list *ptfig);
void liste_figure(lofig_list *ptfig);
void liste_con_ext(lofig_list *ptfig);
void liste_sig_index(lofig_list *ptfig);
void liste_mod_inst_conn(lofig_list *ptfig);
static char *dot_to_slash(char *name,char *buffer);
static char *xnfvector(char *name,char *buffer);
int prim_add_inv(char *name);
static char *prim_map_pin_name(char *name,char *buffer);
static char *xnf_map_figname(char *name,char *buffer);
static char *signalname(ht *h,lofig_list *lof,losig_list *ls,char *buffer);
static char dir(locon_list *lc);
static char dirext(locon_list *lc);
void xnfsavelofig(lofig_list *lof);


static char *spec_ibuf[] = { "inff", "outfft","inlat", "bufgp", "bufgs", "bufg", 
			     "ibuf", "obuf" };
static int spec_ibuf_size = sizeof spec_ibuf / sizeof ( char * ) ;

static char *clkbuf = "bufgp";
static char *ibuf = "ibuf";
static char *obuf = "obuf";
static char *tbuf = "tbuf";
static char *obuft = "obuft";

static char buffer1[BUFSIZE]; /* large buffer for string manipulation */
static char buffer2[BUFSIZE]; /* large buffer for string manipulation */

static int prim_map ; /* mode to map pins o->O, i0->0, and add INVS */
static char prim_input_size ;
static char prim_bubble_count ;

/*#define DEBUG 1*/

/*-------------------------------------------------------------*/

void liste_model(lofig_list *ptfig)
{
chain_list *pch;

	pch = ptfig->MODELCHAIN ;
	while(pch) {
		if(pch->DATA) 
			fprintf(stdout,"modele : %s \n", pch->DATA);		
		pch = pch->NEXT;
	}
}
/*-------------------------------------------------------------*/
void liste_figure(lofig_list *ptfig)
{
lofig_list *pfg;

	pfg = ptfig ;
	while(pfg) {
		if(pfg->NAME) 
			fprintf(stderr,"figure : %s \n", pfg->NAME);		
		pfg = pfg->NEXT;
	}
}
/*-------------------------------------------------------------*/
void liste_con_ext(lofig_list *ptfig)
{
locon_list *plc;

	fprintf(stderr,"**liste des connecteurs externes\n");
	plc = ptfig->LOCON ;
	while(plc) {
		fprintf(stderr,"connecteur : %s ,sig index : %d ,type : %c,
				direction : %c \n",plc->NAME,plc->SIG->INDEX,plc->TYPE,
				plc->DIRECTION);
		plc = plc->NEXT;
	}
}
/*-------------------------------------------------------------*/
void liste_sig_index(lofig_list *ptfig)
{
losig_list *pls;

	fprintf(stderr,"**liste des signaux et index\n");
	pls = ptfig->LOSIG ;
	while(pls) {
		if(pls->NAMECHAIN) {
			if(pls->NAMECHAIN->DATA)
				fprintf(stderr,"signal : %s ,index : %d ,type : %c\n", 
						pls->NAMECHAIN->DATA,pls->INDEX,pls->TYPE);		
		}
		else
			fprintf(stderr,"index : %d ,type : %c\n",pls->INDEX,pls->TYPE);		
		pls = pls->NEXT;
	}
}
/*-------------------------------------------------------------*/
void liste_mod_inst_conn(lofig_list *ptfig)
{
loins_list *pi;
locon_list	*pc;

	fprintf(stderr,"**liste des instances et connecteurs associes\n");
	pi = ptfig->LOINS ;
	while(pi) {
		fprintf(stderr,"modele : %s ,instance : %s \n",pi->FIGNAME,pi->INSNAME);
		pc = pi->LOCON;
		while(pc) {
			fprintf(stderr,"connecteur : %s ,index : %d ,type : %c,
					dir : %c \n",pc->NAME,pc->SIG->INDEX,pc->TYPE,pc->DIRECTION);
			pc = pc->NEXT;
		}
		pi = pi->NEXT;
	}
}
/*-------------------------------------------------------------*/
/* forward slash denotes hier for sig and symbol names in a
   flattened XNF file
   REF Figure 2-5 of April 1994 XACT reference guide, Volume II
   XNFMerge section page 2-16
   "XNFMERGE places the instance name of the symbol referencing the
   logic and a / at the beginning of each signal and symbol name"
   Thus .'s are replaced with /'s via dot_to_slash
*/
static char *dot_to_slash(char *name,char *buffer)
{
   int i;
   for ( i=0 ; ; i++ ) {
      if( *(name+i) == '.' ) {
          *(buffer+i) =  '/' ;
      } else {
          *(buffer+i) =  *(name+i) ;
      }
      if ( *(name+i) == NULL ) break ;
   }
   return buffer ;
}
/*-------------------------------------------------------------*/
static char *xnfvector(char *name,char *buffer )
{
char *s;
int l ;
   dot_to_slash(name,buffer);  /* buffer == name with dots mapped to slash */
   l = strlen(buffer) ;
   if ((s = strchr(buffer, ' ')) == NULL)
      return buffer;

   buffer[s - buffer] = '<';
   buffer[l++] = '>';
   buffer[l] = '\0';
   return buffer;
}   
/*-------------------------------------------------------------*/
int prim_add_inv(char *name)
{
  char diff,pin;
  if ( (! prim_map) || (prim_bubble_count==0) ) return 0;
  diff = prim_input_size - prim_bubble_count ;
  if      ( ! strcmp(name,"o")  ) { return 0 ; }  
  else if ( ! strcmp(name,"i0") ) { pin=0 ; }
  else if ( ! strcmp(name,"i1") ) { pin=1 ; }
  else if ( ! strcmp(name,"i2") ) { pin=2 ; }
  else if ( ! strcmp(name,"i3") ) { pin=3 ; }
  else if ( ! strcmp(name,"i4") ) { pin=4 ; }
  else  { fprintf(stderr,"FORMAT_ERROR: pin name *%s* not in set o,i0,i1,
				  i2,i3,i4 \n",name) ; }
  if ( pin >= diff ) { return 1 ; }
  else { return 0 ; } 
}
/*-------------------------------------------------------------*/
static char *prim_map_pin_name(char *name,char *buffer)
{
  xnfvector( name, buffer ) ; /* buffer updated "name elt" to "name<elt>" */
  if ( ! prim_map ) return buffer ;
  if      ( ! strcmp(buffer,"o")  ) { return "O" ; }
  else if ( ! strcmp(buffer,"i0") ) { return "1" ; }
  else if ( ! strcmp(buffer,"i1") ) { return "2" ; }
  else if ( ! strcmp(buffer,"i2") ) { return "3" ; }
  else if ( ! strcmp(buffer,"i3") ) { return "4" ; }
  else if ( ! strcmp(buffer,"i4") ) { return "5" ; }
  else { fprintf(stderr,"FORMAT_ERROR: pin name *%s* not in set o,i0,i1,
				 i2,i3,i4 \n",buffer) ;
      return buffer ;}
}
/*-------------------------------------------------------------*/
static char *xnf_map_figname(char *name,char *buffer)
{
   int l = strlen(name);
   int stpos=3;
   char *sp;
   prim_map=0;
   prim_bubble_count=0;
   /* fd and dff primitives+ to dff, no prim_map */
   if ( ! ( strcmp(name,"fdce") && strcmp(name,"fdpe") && strcmp(name,"fd") 
		   && strcmp(name,"fdcer") ) ) {
        strcpy(buffer,"dff");
        return buffer ;
   }
   /* process plain primitives */
   else if ( strncmp(name,"and",3) == 0 ) {
        strcpy(buffer,"and");
   } else if ( strncmp(name,"nand",4) == 0 ) {
        strcpy(buffer,"nand");
        stpos=4;
   } else if ( strncmp(name,"xor",3) == 0 ) {
        strcpy(buffer,"xor");
   } else if ( strncmp(name,"nxor",4) == 0 ) {
        strcpy(buffer,"xnor");
        stpos=4;
   } else {
      return name;
   }
   sp = name + stpos ;
   if ( l == stpos + 1 ) {
      prim_input_size =  *sp - 0x30 ;
      if ( (prim_input_size < 2) || (prim_input_size > 5) ) { 
		  fprintf(stderr,"A:FORMAT_ERROR\n") ; }
   } else if ( l == stpos+2+1) {
      prim_input_size =  *sp - 0x30 ;
      if ( (prim_input_size < 2) || (prim_input_size > 5) ) { 
		  fprintf(stderr,"B:FORMAT_ERROR\n") ; }
      if ( strncmp(++sp,"b",1) != 0 ) {  
		  fprintf(stderr,"C:FORMAT_ERROR name=%s\n",name) ; }
      prim_bubble_count=*(++sp) - 0x30 ;
      if ( (prim_bubble_count < 0) || (prim_bubble_count > prim_input_size) ) { 
      	  fprintf(stderr,"D:FORMAT_ERROR\n") ;
      }
   } else { 
	   fprintf(stderr,"E:FORMAT_ERROR name=%s l=%d stpos=%d \n",name,l,stpos) ; }

   prim_map = 1;
   return buffer ;
}
/*-------------------------------------------------------------*/
static lofig_list *mkbuf()
{
lofig_list *lof = NULL;
locon_list *lc;
losig_list *ls;

   /* let us generate the buffers that are necessary for our IOB */
   lof = addlomodel(lof, ibuf);
   addlocon(lof, "vss",
            addlosig(lof, 0, addchain(NULL, "vss"), EXTERNAL), IN);
   addlocon(lof, "vdd",
            addlosig(lof, 1, addchain(NULL, "vdd"), EXTERNAL), IN);
   addlocon(lof, "o",
            addlosig(lof, 2, addchain(NULL, "o"), EXTERNAL), OUT);
   addlocon(lof, "i",
            addlosig(lof, 3, addchain(NULL, "i"), EXTERNAL), IN);
   ibuf = lof->NAME; /* avoid the double namealloc! */

   lof = addlomodel(lof, obuf);
   addlocon(lof, "vss",
            addlosig(lof, 0, addchain(NULL, "vss"), EXTERNAL), IN);
   addlocon(lof, "vdd",
            addlosig(lof, 1, addchain(NULL, "vdd"), EXTERNAL), IN);
   addlocon(lof, "o",
            addlosig(lof, 2, addchain(NULL, "o"), EXTERNAL), OUT);
   addlocon(lof, "i",
            addlosig(lof, 3, addchain(NULL, "i"), EXTERNAL), IN);
   obuf = lof->NAME; /* avoid the double namealloc! */

   lof = addlomodel(lof, clkbuf);
   addlocon(lof, "vss",
            addlosig(lof, 0, addchain(NULL, "vss"), EXTERNAL), IN);
   addlocon(lof, "vdd",
            addlosig(lof, 1, addchain(NULL, "vdd"), EXTERNAL), IN);
   addlocon(lof, "o",
            addlosig(lof, 2, addchain(NULL, "o"), EXTERNAL), OUT);
   addlocon(lof, "i",
            addlosig(lof, 3, addchain(NULL, "i"), EXTERNAL), IN);
   clkbuf = lof->NAME; /* avoid the double namealloc! */
   tbuf = namealloc(tbuf);

   return lof;

}
/*-------------------------------------------------------------*/
static char *signalname(ht *h,lofig_list *lof,losig_list *ls,char *buffer)
{
int  val;
char *s = getsigname(ls);

   if ((val = gethtitem(h, s)) == EMPTYHT) /* the name is unique up to now */
      addhtitem(h, s, (int)ls);
      /* that suck because I assume sizeof(int) == sizeof (void *),
         which insn't true on some beasts */
   else if (val != (int)ls) {
      /* that sucks: two signals have same name, so I return the
         concatenation of the signal name and its index. */
      sprintf(buffer, "%s%d", s, ls->INDEX);
      s = namealloc(buffer);
   }
   return xnfvector(s,buffer);
}
/*-------------------------------------------------------------*/
static char dir(locon_list *lc)
{
   switch(lc->DIRECTION) {
      case IN: return 'I';
      case TRISTATE: 
      case TRANSCV: 
      case INOUT:
      case OUT: return 'O';
      default: return 'X';
   }
}
/*-------------------------------------------------------------*/
static char dirext(locon_list *lc)
{
   switch(lc->DIRECTION) {
      case IN: return 'I';
      case TRANSCV: return 'B';
      case TRISTATE:
      case INOUT:
      case OUT: return 'O';
      default: return 'X';
   }
}
/*-------------------------------------------------------------*/
void xnfsavelofig(lofig_list *lof)
{
FILE *file;
loins_list *li;
locon_list *lc;
losig_list *ls;
long index, hsize = 0;
losig_list *sigvdd = NULL, *sigvss = NULL;
lofig_list *buflist;
ht *h; /* used to check signal name unicity no warrantied by mbk */
int i ;

   if ((file = mbkfopen(lof->NAME, "xnf", "w")) == NULL) {
      fflush(stdout);
      fprintf(stderr, "mbk error : unable to open file %s/%s.%s",
              WORK_LIB, lof->NAME, "xnf");
      EXIT(1);
   }

   VDD = namealloc(VDD);
   VSS = namealloc(VSS);
   buflist = mkbuf();
   guessextdir(lof);
   lofigchain(lof);

   fprintf(file, "LCANET,5\n");
   fprintf(file, "USER,xnf file %s generated for %s\n",
                  lof->NAME, getenv("USER"));
   fprintf(file, "PROG,mbk2xnf,1.0,\"Mapping for X4000 macros only!\"\n");
   fprintf(file, "PWR,0,%s,%s\n", VSS,"vssi");
   fprintf(file, "PWR,1,%s,%s\n", VDD,"vddi");

   /* This is a very simple and brute force xnf driver:
      detecting the IO is a pain, since the macro library may be
      enhanced by the user, and that IOB cannot be equivalent to CLB
      from a macro point of view.
      Therefore, all inputs are buffurized, all ouputs are bufferized,
      and tree state outputs are changed to iob for tree state. */
   /* waranty correct external signal detection, who knows... */

	/* parcours les connecteurs externes		*/
	for (lc = lof->LOCON; lc; lc = lc->NEXT) {
	   chain_list *c;
	   locon_list *externalcon;
	   int onexternalbuf,onebufgp;
#ifdef DEBUG
       /* lc->SIG points into a link list of losigs belonging to all the locons,
            only the one immediately pointed at belongs to the connector at hand */
       (void)fprintf(stdout,"locon: DIR *%c* TYPE *%c* NAME *%-15s* ",
                       lc->DIRECTION,lc->TYPE,lc->NAME );
       (void)fprintf(stdout,"sig: TYPE *%c* INDEX *%6ld* NAME *%-15s* \n",
                       lc->SIG->TYPE,lc->SIG->INDEX,getsigname(lc->SIG) ) ;
#endif

      lc->SIG->TYPE = EXTERNAL; /* probably not necessary */

      onexternalbuf = 0;
      onebufgp = 0;
      for (c = (chain_list *) ( getptype( lc->SIG->USER, (long) LOFIGCHAIN )->DATA ) ;
      	   c;
           c = c->NEXT) {
#ifdef DEBUG
           (void)fprintf(stdout,"chain: NAME *%s* ", ( (locon_list *) c->DATA)->NAME );
           (void)fprintf(stdout,"DIR *%c* TYPE *%c*",
 				( (locon_list *)c->DATA )->DIRECTION, ( (locon_list *)c->DATA )->TYPE );
           if (  (((locon_list *)c->DATA )->TYPE ) == INTERNAL ) {
              (void)fprintf(stdout,"FIGNAME %s\n",
                          ( (loins_list *) ((locon_list *)c->DATA )->ROOT )->FIGNAME );
           } else {
              (void)fprintf(stdout,"****\n");
           }
#     endif
         if (((locon_list *)c->DATA)->TYPE == EXTERNAL) {
            externalcon = (locon_list *)c->DATA;
            continue;
	  	 }
         if (((locon_list *)c->DATA)->TYPE != INTERNAL)
            continue;
         li = (loins_list *)((locon_list *)c->DATA)->ROOT;
         for( i=0 ; i < spec_ibuf_size ; i++ ) {
            if ( ! strcmp(li->FIGNAME, spec_ibuf[i] ) ) {
               onexternalbuf++;
               if(! strcmp(li->FIGNAME,clkbuf)) onebufgp++;
               break ;
            }
         }
      }
      if ((onexternalbuf && (strcmp(externalcon->NAME,"ck"))) || (onexternalbuf && onebufgp)) {
         externalcon->USER = (ptype_list *)-1;  /* ugly (but fast) */
#     ifdef DEBUG
		  fprintf(stdout,"Connecteur : %s, a ne pas bufferiser \n",externalcon->NAME);
#     endif
      }
   }

   for (index = 0, ls = lof->LOSIG; ls; ls = ls->NEXT) {

#     ifdef DEBUG
         (void)fprintf(stdout, "losig : TYPE *%c* index *%6ld* name *%s*\n",
                                          ls->TYPE, ls->INDEX, getsigname(ls) );
#     endif
      if (index < ls->INDEX)
         index = ls->INDEX;
      if (getsigname(ls) == VDD)
         sigvdd = ls;
      if (getsigname(ls) == VSS)
         sigvss = ls;
      hsize++;
 
   }
   index++;
   if (!sigvdd || !sigvss) {
      fflush(stdout);
      fprintf(stderr, "mbk2xnf error: no vdd or vss signals!\n");
      EXIT(1);
   }

   h = addht(hsize << 1); /* bigger means faster! */

   /* add IO buffers and rename the tristate ones connected to output
      ports. */
   for (lc = lof->LOCON; lc; lc = lc->NEXT) {
   chain_list *c;
   losig_list *pnewsig,*poldsig;
   char		  *pnewname,*poldname;

      if (isvdd(lc->NAME) || isvss(lc->NAME))
         continue;

      c = addchain(NULL, (void *)sigvss);
      c = addchain(c, (void *)sigvdd);
      switch(lc->DIRECTION) {
         case IN:

	  		if (lc->USER == (ptype_list *)-1)  {
				freechain(c);
		 		continue;
			}

            c = addchain(c, (void *)lc->SIG);
            lc->SIG->TYPE = INTERNAL;
            sprintf(buffer1, "$%s", lc->NAME);
            lc->SIG = addlosig(lof, index++,
                              addchain(NULL, (void *)buffer1),
                              EXTERNAL);
            c = addchain(c, (void *)lc->SIG);
            if( strcmp(lc->NAME,"ck") != 0)
				addloins(lof, buffer1, getlomodel(buflist, ibuf), c);
			else
				addloins(lof, buffer1, getlomodel(buflist, clkbuf), c);
            break;
            
         case INOUT:
            lc->DIRECTION = OUT;
         case OUT:
	  		if (lc->USER == (ptype_list *)-1)  {
				freechain(c);
		 		continue;
			}

            ls = lc->SIG;
            lc->SIG->TYPE = INTERNAL;
            sprintf(buffer1, "$%s", lc->NAME);
            lc->SIG = addlosig(lof, index++,
                              addchain(NULL, (void *)buffer1),
                              EXTERNAL);
            c = addchain(c, (void *)lc->SIG);
            c = addchain(c, (void *)ls);
            addloins(lof, buffer1, getlomodel(buflist, obuf), c); 
            break;

         case TRISTATE:
            for (c = (chain_list *)getptype(lc->SIG->USER, LOFIGCHAIN)->DATA;
                 c;
                 c = c->NEXT) {
               if (((locon_list *)c->DATA)->TYPE != INTERNAL)
                  continue;
               if (((loins_list *)((locon_list *)c->DATA)->ROOT)->FIGNAME == tbuf)
                  ((loins_list *)((locon_list *)c->DATA)->ROOT)->FIGNAME = obuft;
            }
            break;

        case TRANSCV:
	  		if (lc->USER == (ptype_list *)-1)  {
				freechain(c);
		 		continue;
			}
            sprintf(buffer1, "$%s", lc->NAME);
            pnewname = namealloc(buffer1);
            pnewsig = addlosig(lof, index++,
                              addchain(NULL, (void *)pnewname),
                              EXTERNAL);
            poldsig = lc->SIG;
			poldsig->TYPE = INTERNAL;
            poldname = lc->NAME;
     		c = addchain(NULL, (void *)sigvss);
      		c = addchain(c, (void *)sigvdd);
      		c = addchain(c, (void *)poldsig);
      		c = addchain(c, (void *)pnewsig);
			addloins(lof, poldname, getlomodel(buflist, ibuf), c);
            
            for (c = (chain_list *)getptype(lc->SIG->USER, LOFIGCHAIN)->DATA;
                 c;
                 c = c->NEXT) {
               if (((locon_list *)c->DATA)->DIRECTION == IN)
                  continue;
               if (((locon_list *)c->DATA)->DIRECTION == TRISTATE) {
                  ((locon_list *)c->DATA)->SIG = pnewsig;
                  continue;
               }
               if (((locon_list *)c->DATA)->DIRECTION == TRANSCV) {
                  ((locon_list *)c->DATA)->SIG = pnewsig;
                  ((locon_list *)c->DATA)->NAME = pnewname;
                  continue;               
               }
               fflush(stdout);
               fprintf(stderr, "mbk2xnf error: unsupported port direction");
               fprintf(stderr, " with transceiver on %s\n", lc->NAME);
           	   EXIT(1);
			}
			break;
 
        default:
            fflush(stdout);
            fprintf(stderr, "mbk2xnf error: unsupported external ");
            fprintf(stderr, "port direction on %s\n", lc->NAME);
            EXIT(1);
      }
   }

#     ifdef DEBUG
	liste_con_ext(lof);
	liste_sig_index(lof);
	liste_mod_inst_conn(lof);
#     endif
	for (li = lof->LOINS; li; li = li->NEXT) {
		if(!strcmp(li->FIGNAME,"one_y")) { /* traite one_y		*/
		 	for(lc = li->LOCON; lc; lc = lc->NEXT) {
		  		if(!strcmp(lc->NAME,"f")) {
			   		fprintf(file, "pwr,1, %s\n",(((lc->SIG)->NAMECHAIN)->DATA));
					break;
			   	}
	  		}
		continue;
		}
		else if(!strcmp(li->FIGNAME,"zero_y")) { /* traite zero_y		*/
			for(lc = li->LOCON; lc; lc = lc->NEXT) {
				if(!strcmp(lc->NAME,"f")) {
					fprintf(file, "pwr,0, %s\n",(((lc->SIG)->NAMECHAIN)->DATA));
					 break;
				}
	  		}
		continue;
		}
		fprintf(file, "SYM,%s,%s,\n", xnfvector( li->INSNAME, buffer1 ),
                                    xnf_map_figname( li->FIGNAME, buffer2 ) );
		for(lc = li->LOCON; lc; lc = lc->NEXT) {
			/* no vdd/vss required in this world, as far as I can tell */
     		if (!isvdd(lc->NAME) && !isvss(lc->NAME)) {
				fprintf(file, "PIN,%s,%c,%s,", prim_map_pin_name( lc->NAME, buffer1 ),
						dir(lc), signalname( h, lof, lc->SIG, buffer2 )    );
            	if ( prim_add_inv(lc->NAME) ) {
					fprintf(file,",INV\n");
            	} else {
       				fprintf(file,"\n"); 
            	}
			}
		}
   		fprintf(file, "END\n");
	}
   /* since we've played around with the external connectors, we must
      drive the correct external signal names... */
   for (lc = lof->LOCON; lc; lc = lc->NEXT)
      if (!isvdd(getsigname(lc->SIG)) && !isvss(getsigname(lc->SIG)))
         fprintf(file, "EXT,%s,%c,\n", signalname( h, lof, lc->SIG, buffer1 ), 
				 dirext(lc));

   fprintf(file, "EOF\n");
   delht(h);
   fclose(file);
}




/*-------------------------------------------------------------*/
main(argc, argv)
int argc;
char **argv;
{
lofig_list *ptfig;

   fprintf(stderr, "Conversion vst -> xnf \n\n");
   mbkenv();
   if (argc == 2)
      ptfig = getlofig(argv[1],'A');
   else if (argc == 3 && !strcmp(argv[1], "-r")) {
      ptfig = getlofig(argv[2],'A');
      rflattenlofig(ptfig, YES, YES);
   } else {
      fprintf(stderr, "Usage: %s [-r] file\n", argv[0]);
      exit(1);
   }
   xnfsavelofig(ptfig);
   exit(0);
}
