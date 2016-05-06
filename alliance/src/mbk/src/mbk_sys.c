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

/* 
 * Purpose : system dependent functions
 * Date    : 06/03/92
 * Author  : Frederic Petrot <Frederic.Petrot@lip6.fr>
 * Modified by Czo <Olivier.Sirol@lip6.fr> 1997,98
 */

#ident "$Id: mbk_sys.c,v 1.5 2012/05/14 14:20:23 alliance Exp $"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

#include "mut.h"
#include "mbk_sys.h"

static char filename[BUFSIZ];
char MBKFOPEN_NAME[BUFSIZ];
char MBKFOPEN_FILTER;
endchld_list *HEAD_ENDCHLD=NULL;
endchld_list *HEAD_ENDCHLD_FREE=NULL;
int STAT_ENDCHLD;
autoackchld_list *HEAD_AUTOACKCHLD=NULL;
autoackchld_list *HEAD_AUTOACKCHLD_FREE=NULL;
int STAT_AUTOACKCHLD;

long mbkalloc_stat = 0; /* statistics on maximun allocated memory */

void *mbkalloc(size_t nbytes)
{
void  *pt;

   mbkalloc_stat += nbytes;
   if (!(pt = malloc(nbytes))) {
      (void)fflush(stdout);
      (void)fprintf(stderr,"*** mbk error ***\n");
      (void)fprintf(stderr,"fatal mbkalloc error : not enough memory\n");
      EXIT(1);
   }
   return pt;
}

void *mbkrealloc(void  *pt, size_t nbytes)
{

   mbkalloc_stat += nbytes;
   if (!(pt = realloc(pt, nbytes))) {
      (void)fflush(stdout);
      (void)fprintf(stderr,"*** mbk error ***\n");
      (void)fprintf(stderr,"fatal mbkrealloc error : not enough memory\n");
      EXIT(1);
   }
   return pt;
}

void mbkfree(void *ptr)
{
   (void)free(ptr);
}

char *mbkgetenv(const char *name)
{
char *value;

   if (name == NULL)
      fprintf(stdout, "--- mbk --- ERROR : mbkgetenv(NULL)\n");

   value = getenv(name);

   if (TRACE_GETENV) {
      fprintf(stdout, "--- mbk --- mbkgetenv %s : %s\n",
                      name ? name : "NULL", value ? value : "NULL");
   }

   return value;
}


/* file opening :
   defines the strategy used for searching and opening file in the
   mbk environment. */
FILE* mbkfopen(const char *name, const char *extension, const char *mode)
{
  FILE *ptf;

  ptf = mbkfopentrace(name, extension, mode);
  if(( TRACE_FILE == 1 ) || (( TRACE_FILE == 3 ) && (ptf != NULL )))
  {
    fflush( stdout );
    fprintf( stderr,
             "*** mbkfopen( \"%s\", \"%s\", \"%s\" ) -> %s\n",
             name,
             extension ? extension : "",
             mode,
             ptf ? MBKFOPEN_NAME : "FAILED"
           );
  }
  return ptf;
}

void mbkfopen_infos(const FILE *in, const char *filename, const char *name, const char *extension, const char *mode, const char  filter)
{
  if( in )
  {
    strcpy( MBKFOPEN_NAME, filename );
    MBKFOPEN_FILTER = filter;
  }

  if( TRACE_FILE == 2 )
  {
    /* La methode de Czo : y parait qu'on peut debugger avec ca... */
    fprintf( stdout,
             "--- mbk --- mbkfopen %s : %s, (%s,%s,%s)\n",
             in ? "ok" : "FAILED",
             filename,
             name,
             extension ? extension : "",
             mode
           );
  }
}

FILE *mbkfopentrace(const char *name, const char *extension, const char *mode)
{
FILE *in=NULL;
FILE *insav;
int i;


if (!CATA_LIB || !WORK_LIB)
    mbkenv(); /* not done yet */


  if( *mode == 'w' )
  {
    if( OUT_FILTER && FILTER_SFX )
    {
      if (extension)
        (void)sprintf( filename,
                       "%s/%s.%s%s",
                       WORK_LIB,
                       name,
                       extension,
                       FILTER_SFX
                     );
      else
        (void)sprintf(filename, "%s/%s%s", WORK_LIB, name, FILTER_SFX );

      strcpy( MBKFOPEN_NAME, filename );
      in = mbkpopen( filename, OUT_FILTER, 'w' );
      mbkfopen_infos( in, filename, name, extension, mode, YES );
      if (extension)
        (void)sprintf( filename,
                       "%s/%s.%s",
                       WORK_LIB,
                       name,
                       extension
                     );
      else
        (void)sprintf(filename, "%s/%s", WORK_LIB, name );
       if((insav = fopen(filename,READ_TEXT)) != NULL)
         {
          fflush( stdout );
          fprintf( stderr,
             "*** mbk warning : file %s already exist in the drive directory\n", filename) ;
          fprintf( stderr, "it will be deleted\n" ) ;
          fflush( stderr );
          unlink(filename) ;
          fclose(insav) ;
         }
      return ( in );
    }
    else
    {
      if (extension)
        (void)sprintf( filename,
                       "%s/%s.%s",
                       WORK_LIB,
                       name,
                       extension
                     );
      else
        (void)sprintf(filename, "%s/%s", WORK_LIB, name );
      
      strcpy( MBKFOPEN_NAME, filename );
      in = fopen( filename, WRITE_TEXT ) ;
      mbkfopen_infos( in, filename, name, extension, mode, NO );
      return ( in );
    }
  }
  else
  if( *mode == 'r' )
  {
    if (extension) /* if extension is null, no dot is required */
      (void)sprintf( filename,
                     "%s/%s.%s",
                     WORK_LIB,
                     name,
                     extension
                   );
    else
      (void)sprintf(filename, "%s/%s", WORK_LIB, name);
  
    in = fopen( filename, READ_TEXT );
    mbkfopen_infos( in, filename, name, extension, mode, NO );
    if( in )
    {
      if( FILTER_SFX && IN_FILTER )
      {
        if (extension) /* if extension is null, no dot is required */
          (void)sprintf( filename,
                         "%s/%s.%s%s",
                         WORK_LIB,
                         name,
                         extension,
                         FILTER_SFX
                       );
        else
          (void)sprintf(filename, "%s/%s%s", WORK_LIB, name , FILTER_SFX);
        if((insav = fopen(filename,READ_TEXT)) != NULL)
         {
            fflush( stdout );
            fprintf( stderr,
               "*** mbk warning : file %s also exist it will be ignored\n", filename) ;
            fflush( stderr );
            fclose(insav) ;
         }
      }
      return in;
    }

    if( FILTER_SFX && IN_FILTER )
    {
      if (extension) /* if extension is null, no dot is required */
        (void)sprintf( filename,
                       "%s/%s.%s%s",
                       WORK_LIB,
                       name,
                       extension,
                       FILTER_SFX
                     );
      else
        (void)sprintf(filename, "%s/%s%s", WORK_LIB, name, FILTER_SFX );
  
      in = mbkpopen( filename, IN_FILTER, 'r' );
      mbkfopen_infos( in, filename, name, extension, mode, YES );
      if( in )
        return in ;
    }
  
    i=0;

    while (CATA_LIB[i])
    {
      if (extension)
        (void)sprintf( filename,
                       "%s/%s.%s",
                       CATA_LIB[i],
                       name,
                       extension
                     );
      else
        (void)sprintf( filename,
                       "%s/%s",
                       CATA_LIB[i],
                       name
                     );
  
      in = fopen( filename, READ_TEXT );
      mbkfopen_infos( in, filename, name, extension, mode, NO );
      if( in )
      {
        if( FILTER_SFX && IN_FILTER )
        {
          if (extension) /* if extension is null, no dot is required */
            (void)sprintf( filename,
                           "%s/%s.%s%s",
                           CATA_LIB[i],
                           name,
                           extension ,
                           FILTER_SFX
                         );
          else
            (void)sprintf(filename, "%s/%s%s", CATA_LIB[i], name ,FILTER_SFX);
          if((insav = fopen(filename,READ_TEXT)) != NULL)
           {
              fflush( stdout );
              fprintf( stderr,
               "*** mbk warning : file %s also exist it will be ignored\n", filename) ;
              fflush( stderr );
              fclose(insav) ;
             }
        }
        return( in );
      }

      if( FILTER_SFX && IN_FILTER )
      {
        if (extension) /* if extension is null, no dot is required */
          (void)sprintf( filename,
                         "%s/%s.%s%s",
                         CATA_LIB[i],
                         name,
                         extension,
                         FILTER_SFX
                       );
        else
          (void)sprintf(filename, "%s/%s%s", CATA_LIB[i], name, FILTER_SFX );
    
        in = mbkpopen( filename, IN_FILTER, 'r' );
        mbkfopen_infos( in, filename, name, extension, mode, YES );
        if( in )
         {
          return in ;
         }
      }

      i++;
    } 
    return( NULL );
  }
  else
  {
    (void)fflush(stdout);
    (void)fprintf(stderr, "*** mbk error ***\n");
    (void)fprintf(stderr, "unknown file opening mode %s\n", mode);
    EXIT(1);
  }

  return NULL; /* never reached */
}

/* unlink :
   ensures that only files in the working library may be erased. */
int mbkunlink(const char *name, const char *extension)
{
   if (!CATA_LIB || !WORK_LIB)
      mbkenv(); /* not done yet */

   if (extension) /* if extension is null, no dot is required */
      (void)sprintf(filename, "%s/%s.%s", WORK_LIB, name, extension);
   else
      (void)sprintf(filename, "%s/%s", WORK_LIB, name);

   return unlink(filename);
}
   
/* filepath :
   find the complete path of file from mbkfopen point of view. */
char *filepath(const char *name, const char *extension)
{
FILE *in;
int i ;

  if (!CATA_LIB || !WORK_LIB)
    mbkenv(); /* not done yet */

  if (extension) /* if extension is null, no dot is required */
    (void)sprintf(filename, "%s/%s.%s", WORK_LIB, name, extension );
  else
    (void)sprintf(filename, "%s/%s", WORK_LIB, name );
  
  if ((in = fopen(filename, READ_TEXT))) {
    (void)fclose(in);
    return filename;
  }

  if( FILTER_SFX && IN_FILTER )
  {
    if (extension) /* if extension is null, no dot is required */
      (void)sprintf(filename, "%s/%s.%s%s", WORK_LIB, name, extension, FILTER_SFX );
    else
      (void)sprintf(filename, "%s/%s%s", WORK_LIB, name, FILTER_SFX );
    
    if ((in = fopen(filename, READ_TEXT))) {
      (void)fclose(in);
      return filename;
    }
  }


  i = 0;
  while (CATA_LIB[i]) {
    if (extension) /* if extension is null, no dot is required */
      (void)sprintf(filename, "%s/%s.%s", CATA_LIB[i], name, extension );
    else
      (void)sprintf(filename, "%s/%s", CATA_LIB[i], name );
    
    if ((in = fopen(filename, READ_TEXT))) {
      (void)fclose(in);
      return filename;
    }
  
    if( FILTER_SFX && IN_FILTER )
    {
      if (extension) /* if extension is null, no dot is required */
        (void)sprintf(filename, "%s/%s.%s%s", CATA_LIB[i], name, extension, FILTER_SFX );
      else
        (void)sprintf(filename, "%s/%s%s", CATA_LIB[i], name, FILTER_SFX );
      
      if ((in = fopen(filename, READ_TEXT))) {
        (void)fclose(in);
        return filename;
      }
    }
    i++;
  }
  return NULL;
}

/* mbkpopen : open a pipe with a filter, return a FILE*
 * Gregoire.Avot
 */

FILE* mbkpopen(const char *nom, const char *filter, char mode)
{
  int ptf[2];
  static int fic;
  int pid;
  FILE *file;
  char **argv;

  if( mode == 'w' )
    fic = open( nom,
                O_WRONLY|O_CREAT|O_TRUNC,
                S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH
              );
  else
    fic = open( nom, O_RDONLY );

  if( fic == -1 )
    return( NULL );

  if( pipe( ptf ) == -1 )
  {
    fflush( stdout );
    fprintf( stderr, "*** Fatal error in mbkpopen( \"%s\" )\n", nom );
    perror( "pipe() ");
    EXIT(1);
  }

#ifndef AUTO_HAS_VFORK
    pid = fork();
#else
    pid = vfork();
#endif

  if( pid == -1 )
  {
    fflush( stdout );
    fprintf( stderr, "*** Fatal error in mbkpopen( \"%s\" )\n", nom );
    perror( "(v)fork() ");
    EXIT(1);
  }
  
  if( pid == 0 )
  {
    /* fils */
    fflush( stdout );

    argv = decompfilter( filter );
    if( argv == NULL )
    {
      fprintf( stderr, "*** Fatal error in mbkpopen( \"%s\" ) child\n", nom );
      fprintf( stderr, "Bad filter.\n" );
      EXIT(1);
    }

    if( mode == 'w' )
    {
      if( dup2( ptf[0], 0 ) == -1 )
      {
        fprintf( stderr, "*** Fatal error in mbkpopen( \"%s\" ) child\n", nom );
        perror( "dup2( ptf[0], 0 ) ");
        EXIT(1);
      }
          
      if( dup2( fic, 1 ) == -1 )
      {
        fprintf( stderr, "*** Fatal error in mbkpopen( \"%s\" ) child\n", nom );
        perror( "dup2( fic, 1 ) ");
        EXIT(1);
      }
    }
    else
    {
      if( dup2( ptf[1], 1 ) == -1 )
      {
        fprintf( stderr, "*** Fatal error in mbkpopen( \"%s\" ) child\n", nom );
        perror( "dup2( ptf[1], 1 ) ");
        EXIT(1);
      }
          
      if( dup2( fic, 0 ) == -1 )
      {
        fprintf( stderr, "*** Fatal error in mbkpopen( \"%s\" ) child\n", nom );
        perror( "dup2( fic, 0 ) ");
        EXIT(1);
      }
    }

    if( close( fic ) == -1 )
    {
      fprintf( stderr, "*** Fatal error in mbkpopen( \"%s\" ) child\n", nom );
      perror( "close( fic ) ");
      EXIT(1);
    }
    
    if( close( ptf[1] ) == -1 )
    {
      fprintf( stderr, "*** Fatal error in mbkpopen( \"%s\" ) child\n", nom );
      perror( "close( ptf[1] ) ");
      EXIT(1);
    }

    if( close( ptf[0] ) == -1 )
    {
      fprintf( stderr, "*** Fatal error in mbkpopen( \"%s\" ) child\n", nom );
      perror( "close( ptf[0] ) ");
      EXIT(1);
    }
    
    if( execvp( argv[0], &(argv[1]) ) == -1 )
    {
      fprintf( stderr, "*** Fatal error in mbkpopen( \"%s\" ) child\n", nom );
      perror( "execvp " );
      EXIT(1);
    }
  }

  mbksetautoackchld( pid );
  close( fic );

  if( mode == 'w' )
  {  
    if( close( ptf[0] ) == -1 )
    {
      fflush( stdout );
      fprintf( stderr, "*** Fatal error in mbkpopen( \"%s\" ) parent\n", nom );
      perror( "close( ptf[0] ) ");
      EXIT(1);
    }
    
    file = fdopen( ptf[1], "w" );
    if( file == NULL )
    {
      fflush( stdout );
      fprintf( stderr, "*** Fatal error in mbkpopen( \"%s\" ) parent\n", nom );
      perror( "fdopen( ptf[1], \"w\" ) ");
      EXIT(1);
    }
  }
  else
  {  
    if( close( ptf[1] ) == -1 )
    {
      fflush( stdout );
      fprintf( stderr, "*** Fatal error in mbkpopen( \"%s\" ) parent\n", nom );
      perror( "close( ptf[1] ) ");
      EXIT(1);
    }
    
    file = fdopen( ptf[0], "r" );
    if( file == NULL )
    {
      fflush( stdout );
      fprintf( stderr, "*** Fatal error in mbkpopen( \"%s\" ) parent\n", nom );
      perror( "fdopen( ptf[0], \"r\" ) ");
      EXIT(1);
    }
  }

  return( file );
}

char **decompfilter( const char *filter )
{
  int         pos;
  int         i;
  char       *pt;
  char       *fullpath;
  char       *argv0;
  char       *argvn;
  chain_list *head;
  chain_list *scan;
  int         nbarg;
  char      **ret;

  /* remove trailing space */
  for( pos = 0 ; filter[pos] == ' ' && filter[pos] != 0  ; pos++ );
  if( filter[pos] == 0 )
    return( NULL );

  /* The full path to binairie */
  for( i = pos ; filter[i] != ' ' && filter[i] != 0 ; i++ );
  fullpath = (char*)mbkalloc( sizeof(char) * ( i-pos+1 ) );
  strncpy( fullpath, filter+pos, i-pos );
  fullpath[i-pos] = 0;
  pos = i;

  /* The binairie himself */
  pt = strrchr( fullpath, '/' );
  argv0 = (char*)mbkalloc( sizeof( char ) * (strlen( fullpath ) + 1) );
  if( pt == NULL )
    strcpy( argv0, fullpath );
  else
    strcpy( argv0, pt+sizeof(char) );

  /* The arguments */
  head = NULL;
  nbarg=2;

  do
  {
    for( ; filter[pos] == ' ' && filter[pos] != 0  ; pos++ );
    if( filter[pos] == 0 )
      break;

    for( i = pos ; filter[i] != ' ' && filter[i] != 0 ; i++ );
    argvn = (char*)mbkalloc( sizeof( char ) * ( i-pos+1 ) );
    strncpy( argvn, filter+pos, i-pos );
    argvn[i-pos] = 0;
    pos = i;
    head = addchain( head, argvn );
    nbarg++;
  }
  while( 1 );

  head = reverse( head );

  ret = (char**)mbkalloc( sizeof(char*) * (nbarg+1) );

  ret[0] = fullpath;
  ret[1] = argv0;

  for( scan = head, pos=2 ; scan ; scan = scan->NEXT, pos++ )
    ret[pos] = scan->DATA;
  ret[pos] = NULL;

  freechain( head );

  return( ret );
}

void mbkackchld( int sig )
{
  int               pid;
  int               status;
  endchld_list     *new;
  int               err;
  autoackchld_list *scanauto, *prevauto;
  
  /* Le handlert d'interruption SIGCHLD. Il doit avoir été configuré pour ne
   * pas être interrompu par lui même. */
  
  if( sig != SIGCHLD )
  {
    fflush( stdout );
    fprintf( stderr,
             "*** mbk warning : call of mbkackchld with signal %d\n",
             (int)sig
           );
    return;
  }
 
  do
  {
    errno = 0;
    pid = waitpid( -1, &status, WNOHANG );
    err = errno ;
    if( pid > 0 )
    {
      for( scanauto = HEAD_AUTOACKCHLD, prevauto=NULL ;
           scanauto ;
           scanauto = scanauto->NEXT
         )
      {
        if( scanauto->PID == pid )
          break;
        prevauto = scanauto ;
      }

      if( scanauto )
      {
        /* Process was previously put in the list auto acknowledge. */
        if( prevauto )
          prevauto->NEXT = scanauto->NEXT;
        else
          HEAD_AUTOACKCHLD = scanauto->NEXT;
        
        mbkfreeautoackchld( scanauto );
      }
      else
      {
        /* Put process in the waiting queue to be read by user */
        new           = mbkgetendchld();
        new->NEXT     = HEAD_ENDCHLD;
        HEAD_ENDCHLD = new;
        new->PID      = pid;
        new->STATUS   = status;
      }
    }
  }
  while( pid != 0 && err != ECHILD );

}

/* Sur les architectures multipro, on peut avoir un malloc() interrompu par
 * un signal. Si ou alloue une structure endchld_list avec un malloc dans le
 * handler, on aura une réentrance du malloc. Cette caractéristiques n'est pour
 * l'instant pas supportée. */

endchld_list* mbkgetendchld( void )
{
  endchld_list *new;

  /* Cette fonction ne doit être appellée qu'à partir du handler du signal
   * SIGCHLD. Si cela devait changer, il faudrait la protéger contre la
   * réception de ce signal */
  
  if( ! HEAD_ENDCHLD_FREE )
  {
    fflush( stdout );
    fprintf( stderr, 
             "\n*** mbk ERROR : Not enought free slot in mbkgetfreeendchld().\n"
           );
    exit(1);
  }

  new = HEAD_ENDCHLD_FREE ;
  HEAD_ENDCHLD_FREE = HEAD_ENDCHLD_FREE->NEXT ;

  STAT_ENDCHLD++;
  return( new );
}

void mbkfreeendchld( endchld_list *bloc )
{
  sigset_t  set;
  
  /* Cette fonction est appellée à partir de mbkwaitpid(). Il peut y avoir une
   * corruption de la liste si un signal SIGCHLD intervient entre les deux
   * lignes puisque le handler appellera mbkgetendchld... */
  
  /* On masque temporairement le signal embétant */
  sigemptyset( &set );
  sigaddset( &set, SIGCHLD );
  sigprocmask( SIG_BLOCK, &set, NULL );

  
  bloc->NEXT = HEAD_ENDCHLD_FREE ;
  HEAD_ENDCHLD_FREE = bloc ;

  STAT_ENDCHLD--;
  /* On le demasque. Si il y en a un qui est arrivé entre temps, il sera pris
   * en compte maintenant. */
  /* sigrelse(SIGCHLD); (non défini sous SunOs) */
  sigprocmask( SIG_UNBLOCK, &set, NULL );
}

void mbkinitendchld( )
{
  endchld_list *bloc;
  int n;

  /* Là, pas de problèmes : le handler n'est pas encore positionné quand cette
   * fonction est appellée. */
  bloc = (endchld_list*)mbkalloc( sizeof( endchld_list ) * ENDCHLD_MAX );
  n = ENDCHLD_MAX - 1;
  bloc[n].NEXT = NULL ;
  while( n > 0 )
  {
    n-- ;
    bloc[n].NEXT = &( bloc[n+1] );
  }
  HEAD_ENDCHLD_FREE = &(bloc[0]);
  STAT_ENDCHLD=0;
}

autoackchld_list* mbkgetautoackchld( void )
{
  autoackchld_list *new;

  if( ! HEAD_AUTOACKCHLD_FREE )
  {
    fflush( stdout );
    fprintf( stderr, 
             "\n*** mbk ERROR : Not enought free slot in mbkgetautoackchld().\n"
           );
    exit(1);
  }

  new = HEAD_AUTOACKCHLD_FREE ;
  HEAD_AUTOACKCHLD_FREE = HEAD_AUTOACKCHLD_FREE->NEXT ;

  STAT_AUTOACKCHLD++;
  return( new );
}

void mbkfreeautoackchld( autoackchld_list *bloc )
{
  bloc->NEXT = HEAD_AUTOACKCHLD_FREE ;
  HEAD_AUTOACKCHLD_FREE = bloc ;
  STAT_AUTOACKCHLD--;
}

void mbkinitautoackchld( void )
{
  autoackchld_list *bloc;
  int n;

  bloc=(autoackchld_list*)mbkalloc( sizeof( autoackchld_list ) * ENDCHLD_MAX );
  n = AUTOACKCHLD_MAX - 1;
  bloc[n].NEXT = NULL ;
  while( n > 0 )
  {
    n-- ;
    bloc[n].NEXT = &( bloc[n+1] );
  }
  HEAD_AUTOACKCHLD_FREE = &(bloc[0]);
  STAT_AUTOACKCHLD=0;
}

void mbksetautoackchld( int pid )
{
  endchld_list     *scan, *prev ;
  autoackchld_list *new;
  sigset_t  set;

  /* Fonction utilisateur : on met dans la liste HEAD_AUTOACKCHLD un numéro de
   * pid. Dès qu'un processus fils se termine ave ce pid, il sera 
   * automatiquement supprimé. */

  /* Dans le déroulement normal du programme, on touche ici à des structures
   * qui sont modifiées par le handler SIGCHLD. On évite de mettre le bazard en
   * blindant le morceau de code. */

  sigemptyset( &set );
  sigaddset( &set, SIGCHLD );
  sigprocmask( SIG_BLOCK, &set, NULL );

  prev = NULL;
  for( scan = HEAD_ENDCHLD ; scan ; scan = scan->NEXT )
  {
    if( scan->PID == pid )
      break;
    prev = scan;
  }

  if( scan )
  {
    /* Cas où le processus que l'on demande d'acquiter automatiquement est déjà
     * terminé. Dans ce cas on le supprime de la file d'attente des processus
     * terminés */

    if( prev )
      prev->NEXT = scan->NEXT ;
    else
      HEAD_ENDCHLD = scan->NEXT ;

    mbkfreeendchld( scan );
  }
  else
  {
    /* On ne voudra jamais savoir ce qu'est devenu le processus fils pid. On le
     * met dans la liste des processus à acquiter automatiquement */
    new         = mbkgetautoackchld();
    new->PID    = pid;
    new->NEXT   = HEAD_AUTOACKCHLD ;
    HEAD_AUTOACKCHLD = new ;
  }
  sigprocmask( SIG_UNBLOCK, &set, NULL );
}
