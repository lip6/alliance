/*####==============================================================####*/
/*##                                                                  ##*/
/*##  This file is part of BOUNDARY SCAN TESTER for PC/Linux Systems  ##*/
/*##  (C) 1995, 1996 MASI, CAO-VLSI Team                              ##*/
/*##                                                                  ##*/
/*##  Authors            : Philippe OLEK, Olivier SIROL (1995)        ##*/
/*##  Authors            : Eric MECHIN (1996)                         ##*/
/*##                                                                  ##*/
/*##  E-mail support:    cao-vlsi@masi.ibp.fr                         ##*/
/*##                                                                  ##*/
/*####==============================================================####*/

/*####==============================================================####*/
/*##                                                                  ##*/
/*##  lpscan.c   : Driver pour port parallele architecture PC/Linux   ##*/
/*##               NOYAU - driver caractere	                      ##*/
/*##                                                                  ##*/
/*##  13/12/99  F.Dromard. Modification pour Linux 2.0 et plus.       ##*/
/*##            La routine init n'a plus de parametre et ne           ##*/
/*##            renvoie rien. (correction de 1996 non reportee)       ##*/
/*##  09/04/01  1/ F.Donnet Modification pour Linux 2.1 et plus.      ##*/
/*##               les fonction memcpy_{to,from}fs sont remplacees    ##*/
/*##               par copy_{to,from}_user                            ##*/
/*##            2/ segment.h n'existe plus et est remplace par        ##*/
/*##               uaccess.h pour Linux 2.1 et plus                   ##*/
/*##            3/ la structure file_operations a changee ainsi que   ##*/
/*##               les prototype des fonctions read et write          ##*/
/*##  15/02/02  1/ F. Donnet pour la version 2.4.9-21                 ##*/
/*##               la structure file_operations a changee             ##*/
/*####==============================================================####*/


#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/signal.h>
#include <linux/utsname.h>
/* #include <asm/segment.h> */
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/io.h>
#if 0
#include <sys/ioctl.h>
#endif
#include    "lpscan.h"

#define memcpy_tofs(a,b,c) copy_to_user((a),(b),(c))
#define memcpy_fromfs(a,b,c) copy_from_user((a),(b),(c))

/*========================================================================
function decls
*/

#if 0
static   int  lpscan_read(struct inode * inode,struct file * file,char * buf,int count);
static   int  lpscan_write(struct inode * inode,struct file * file,char * buf,int count);
static   void lpscan_release(struct inode * inode, struct file * file);
#endif

void          lpscan_init(void);
static   int  lpscan_open(struct inode * inode, struct file * file);
static   ssize_t lpscan_read(struct file * file,char * buf,size_t count,loff_t *off);
static   ssize_t lpscan_write(struct file * file,const char * buf,size_t count,loff_t *off);
static   int  lpscan_ioctl(struct inode * inode,struct file * file,unsigned int cmd,unsigned long arg);
static   int lpscan_release(struct inode * inode, struct file * file);



/*========================================================================
variables globales, gestion d'un seul port parallele
*/

typedef struct {  /* struct pour dev (choix personnel) */
char        bR[MAX_BUF];    /*  buffer en reception */
char        bW[MAX_BUF];    /*  buffer en emission  */
char        iocard;         /* 1 : carte paral presente  */
char        inUse;      /* mis a 0 par init, a IN_USE par open, a 0 par close   */
			/* empeche l'utilisation du port par plusieurs process  */
                        /* a la fois */
    } Vglob;

Vglob       lpscan;

typedef struct {  /* struct pour un arg ioctl (choix personnel) */
char   *   buf;    /*  buffer */
int        count;  /* sa taille */
           }  ioctlarg;

#if 0  /*noyau 2.0*/
static struct file_operations lpscan_fops = {  /* struct imposee */
	NULL,			/* seek 	*/
	lpscan_read,		/* read 	*/
	lpscan_write,		/* write 	*/
	NULL,			/* readdir 	*/
	NULL,			/* select 	*/
	lpscan_ioctl,		/* ioctl 	*/
	NULL,			/* mmap 	*/
	lpscan_open,		/* open 	*/
	lpscan_release         	/* release	*/
};
#endif

#if 0 /*noyau 2.2.10*/
static struct file_operations lpscan_fops = {  /* struct imposee */
        NULL,                   /* loff_t (*llseek) (struct file *, loff_t, int); */
        lpscan_read,		/* ssize_t (*read) (struct file *, char *, size_t, loff_t *); */
        lpscan_write,		/* ssize_t (*write) (struct file *, const char *, size_t, loff_t *); */
        NULL,			/* int (*readdir) (struct file *, void *, filldir_t); */
        NULL,			/* unsigned int (*poll) (struct file *, struct poll_table_struct *); */
        lpscan_ioctl,		/* int (*ioctl) (struct inode *, struct file *, unsigned int, unsigned long); */
        NULL,			/* int (*mmap) (struct file *, struct vm_area_struct *); */
        lpscan_open,		/* int (*open) (struct inode *, struct file *); */
        NULL,			/* int (*flush) (struct file *); */
        lpscan_release,		/* int (*release) (struct inode *, struct file *); */
        NULL,			/* int (*fsync) (struct file *, struct dentry *); */
        NULL,			/* int (*fasync) (int, struct file *, int); */
        NULL,			/* int (*check_media_change) (kdev_t dev); */
        NULL,			/* int (*revalidate) (kdev_t dev); */
        NULL,			/* int (*lock) (struct file *, int, struct file_lock *); */
};          
#endif

/*noyau 2.4.9-21*/
static struct file_operations lpscan_fops = {  /* struct imposee */
	NULL,                   /* struct module *owner; */
        NULL,                   /* loff_t (*llseek) (struct file *, loff_t, int); */
        lpscan_read,		/* ssize_t (*read) (struct file *, char *, size_t, loff_t *); */
        lpscan_write,		/* ssize_t (*write) (struct file *, const char *, size_t, loff_t *); */
        NULL,			/* int (*readdir) (struct file *, void *, filldir_t); */
        NULL,			/* unsigned int (*poll) (struct file *, struct poll_table_struct *); */
        lpscan_ioctl,		/* int (*ioctl) (struct inode *, struct file *, unsigned int, unsigned long); */
        NULL,			/* int (*mmap) (struct file *, struct vm_area_struct *); */
        lpscan_open,		/* int (*open) (struct inode *, struct file *); */
        NULL,			/* int (*flush) (struct file *); */
        lpscan_release,		/* int (*release) (struct inode *, struct file *); */
        NULL,			/* int (*fsync) (struct file *, struct dentry *); */
        NULL,			/* int (*fasync) (int, struct file *, int); */
        NULL,			/* int (*check_media_change) (kdev_t dev); */
        NULL,			/* int (*revalidate) (kdev_t dev); */
        NULL,			/* int (*lock) (struct file *, int, struct file_lock *); */
};          


/*========================================================================
Routines Driver
*/

/*========================================================================
INIT
*/
void lpscan_init(void)
    {
	lpscan.inUse = 0;     /* initialise a 0 */

        if (register_chrdev(MAJOR_LPSCAN,"lpscan",&lpscan_fops))
                {
		printk("lpscan_init: Unable to get major %d for Driver LPSCAN\n",MAJOR_LPSCAN);
		return ;
		}
        else    {
		printk("lpscan_init: Driver LPSCAN carte parallele OK ########################\n");
		}

        /* on ecrit 0 sur le port parallele (DATA reg) puis on le relit */
        /* pour verifier qu'il est present */
        outb(0x00,DATA);

	if (inb(DATA) == 0)
		{
		lpscan.iocard=1;
        	printk("lpscan_init: Carte parallele presente\n");
        	}
        else
        	{
        	lpscan.iocard=0;
        	printk("lpscan_init: Carte parallele absente\n");
		}
	return ;
    }



/*========================================================================
OPEN
*/
static int lpscan_open(struct inode * inode, struct file * file)
    {
int minor =MINOR(inode->i_rdev);

#ifdef DEBUG
    printk("Driver_LPSCAN : open lpscan%x\n",minor);
#endif

    if ((minor) != 0)
       {
#ifdef DEBUG
       printk("Driver_LPSCAN : mauvais minor\n");
#endif
       return -EINVAL;    /* invalid argument */
       }

    if (lpscan.inUse)
       {
#ifdef DEBUG
       printk("Driver_LPSCAN : port utilise par un autre process\n");
#endif
       return -EBUSY;	 /* device busy  */
       }

    lpscan.inUse = IN_USE;
    return 0;            /* no error */
    }


/*========================================================================
READ
*/
#if 0
static int lpscan_read(struct inode * inode,struct file * file,char * buf,int count)
  {
#ifdef DEBUG
int minor =MINOR(inode->i_rdev);

	printk("Driver_LPSCAN : read lpscan%x\n",minor);
#endif

  if (count != 1)
    return -EINVAL;    /* invalid argument (read de 1 octet seulement) */

  lpscan.bR[0] = inb(STATUS);  /*  Renvoie le contenu du registre STATUS */
  memcpy_tofs(buf,&lpscan.bR[0],count);    /* noyau -> utilisateur */

#ifdef DEBUG
  printk("Driver_LPSCAN : fin read\n");
#endif
  return 0;  /* pas d'erreur */
  }
#endif

static ssize_t lpscan_read(struct file * file,char * buf,size_t count, loff_t *off)
  {
  if (count != 1)
    return -EINVAL;    /* invalid argument (read de 1 octet seulement) */

  lpscan.bR[0] = inb(STATUS);  /*  Renvoie le contenu du registre STATUS */
  memcpy_tofs(buf,&lpscan.bR[0],count);    /* noyau -> utilisateur */

#ifdef DEBUG
  printk("Driver_LPSCAN : fin read\n");
#endif
  return count;  /* pas d'erreur */
  }

/*========================================================================
WRITE
*/
#if 0
static int lpscan_write(struct inode * inode,struct file * file,char * buf,int count)
  {
#ifdef DEBUG
int minor =MINOR(inode->i_rdev);

  printk("Driver_LPSCAN : write lpscan%x\n",minor);
#endif

  if (count != 1)
     return -EINVAL;    /* invalid argument (write de 1 octet seulement) */

  memcpy_fromfs(&lpscan.bW[0],buf,count); /* utilisateur -> noyau */
  outb(lpscan.bW[0],DATA); /* Ecrit l'octet dans le registre DATA */

#ifdef DEBUG
  printk("Driver_LPSCAN : fin write\n");
#endif
  return 0;  /* pas d'erreur */
  }
#endif

static ssize_t lpscan_write(struct file * file,const char * buf,size_t count, loff_t *off)
  {
  if (count != 1)
     return -EINVAL;    /* invalid argument (write de 1 octet seulement) */

  memcpy_fromfs(&lpscan.bW[0],buf,count); /* utilisateur -> noyau */
  outb(lpscan.bW[0],DATA); /* Ecrit l'octet dans le registre DATA */

#ifdef DEBUG
  printk("Driver_LPSCAN : fin write\n");
#endif
  return count;  /* pas d'erreur */
  }

/*========================================================================
IOCTL
*/
static int lpscan_ioctl(struct inode * inode,struct file * file,unsigned int cmd,unsigned long arg)
    {
int i;

#ifdef DEBUG
int minor =MINOR(inode->i_rdev); 

    printk("Driver_LPSCAN : ioctl lpscan%x, cmd %x\n",minor,cmd);
#endif
    switch (cmd)
     {

/*========================================================================
IOCTL CHECK_CARD_PRESENT
*/
     case CHECK_CARD_PRESENT :
        /* carte parallele est presente : 1 sinon 0 */
        return lpscan.iocard;  /* cf init */
        break;

/*========================================================================
IOCTL CHECK_BSCARD_PRESENT
*/
     case CHECK_BSCARD_PRESENT :
        /* STROBE au 5V de la carte BS
           AUTOFEED a la masse de la carte BS via 150 Ohms
           methode valide si carte controller rescente ...
           attention court-circuit sinon ?
           on ecrit 01 sur le port parallele (CONTROL reg) puis on le relit
                    01 : Carte Boundary Scan absente!
                    11 : Carte Boundary Scan mal alimentee !!
                    10 : Carte Boundary Scan OK                           */

        outb(0x01,CONTROL);
        return((int)inb(CONTROL) & 3);   /* on garde STROBE et AUTOFEED */
        break;

/*========================================================================
IOCTL GET_TDO_BUFFERED
*/
     case GET_TDO_BUFFERED :  /* non utilisee */
     /* implementation de la fonction utilisateur getTDO */

        if (!arg)
           return -EINVAL;    /* need pointer */

        if (((ioctlarg * )arg)->count > MAX_BUF)
           return -EINVAL;    /* invalid argument (MAX_BUF octets seulement) */

        memcpy_fromfs(&lpscan.bW[0],((ioctlarg * )arg)->buf,((ioctlarg * )arg)->count); /* utilisateur -> noyau */

        for (i=0;i < (((ioctlarg * )arg)->count) ; i++)
            {
            outb(lpscan.bW[i],DATA); /* Ecrit l'octet dans le registre DATA */
            lpscan.bR[i] = inb(STATUS);  /*  Renvoie le contenu du registre STATUS */
#ifdef DEBUG
  printk("Driver_LPSCAN : done one GET_TDO_BUFFERED\n");
#endif
            }

        memcpy_tofs(((ioctlarg * )arg)->buf,&lpscan.bR[0],((ioctlarg * )arg)->count);    /* noyau -> utilisateur */

        return 0;  /* pas d'erreur */
        break;

/*========================================================================
IOCTL default
*/
     default :
	return -EINVAL;     /* Invalid argument   */
	break;
     }

    return 0;
    }


/*========================================================================
RELEASE
*/
#if 0
static void lpscan_release(struct inode * inode, struct file * file)
{
#ifdef DEBUG
int minor =MINOR(inode->i_rdev);

    printk("Driver_LPSCAN : release lpscan%x\n",minor);
#endif
  lpscan.inUse = 0; /* libere le port */
}
#endif

static int lpscan_release(struct inode * inode, struct file * file)
{
#ifdef DEBUG
int minor =MINOR(inode->i_rdev);

    printk("Driver_LPSCAN : release lpscan%x\n",minor);
#endif
  lpscan.inUse = 0; /* libere le port */
 return 0;
}


/* End of lpscan.c ======================================================*/
