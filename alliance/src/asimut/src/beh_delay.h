
/* ###--------------------------------------------------------------### */
/* file		: beh_delay.h						*/
/* date		: Oct 30 1995						*/
/* version	: v109							*/
/* author	: Pirouz BAZARGAN SABET	& Julien DUNOYER		*/
/* contents	: defines delay definitions used in BEH library		*/
/* ###--------------------------------------------------------------### */

	/* ###------------------------------------------------------### */
	/*    defines							*/
	/* ###------------------------------------------------------### */

#define BEH_DELAY_FIXED          0x01	/* Fixed delay behavior		*/
#define BEH_DELAY_RANDOM         0x02	/* Random delay behavior	*/
#define BEH_DELAY_BACKANOTATED   0x04	/* Backanotated delay behavior	*/
#define BEH_DELAY_BUFREG         0x08	/* Add Buffer delay after reg	*/
#define BEH_DELAY_DEFAULT        0x10	/* Default delay behavior	*/
#define BEH_DELAY_MIN            0x20	/* Minimum delay mode		*/
#define BEH_DELAY_MAX            0x40	/* Maximum delay mode		*/
#define BEH_DELAY_TYP            0x80	/* Typical delay mode		*/

#define BEH_DELAY_FIELD             7	/* delay dico field		*/

