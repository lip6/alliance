

# include  <mut.h>
# include  <mlo.h>
# include  <mlu.h>
# include  <mph.h>
# include  <mpu.h>
# include  <mgn.h>
# include  <abl.h>
# include  <aut.h>
# include  <beh.h>
# include  <stdio.h>
# include  <stdlib.h>


# define  _ALU2_     0
# define  _ALU3_     1
# define  _ALU4_     2
# define  _ALU5_     3
# define  _ALU6_     4
# define  _ALU7_     5
# define  _ALU8_     6
# define  _MAX_ALU_  7



int main (int argc, char *argv[])
{
  phfig_list *pFig;
  phseg_list *pSeg;
  phvia_list *pVIA;

        long  length[_MAX_ALU_];
        long  number[_MAX_ALU_];
        long  total_length;
        long  total_number;
        long  total_VIA;
         int  i;


  mbkenv ();
  alliancebanner ( "PdV"
                 , "0.1"
                 , "Pot de Vin - Routing Evaluator"
                 , "2002"
                 , "5.0"
                 );


  if (argc < 2) {
    fprintf (stderr, "Usage : pdv <layout>\n");

    exit (1);
  }


  pFig = getphfig (argv[1], 'A');

  for (i = 0; i < _MAX_ALU_; i++) {
    length[i] = 0;
    number[i] = 0;
  }

  for (pSeg = pFig->PHSEG; pSeg != NULL; pSeg = pSeg->NEXT) {
    switch (pSeg->LAYER) {
      case ALU2:
      case CALU2: i = _ALU2_; break;
      case ALU3:
      case CALU3: i = _ALU3_; break;
      case ALU4:
      case CALU4: i = _ALU4_; break;
      case ALU5:
      case CALU5: i = _ALU5_; break;
      case ALU6:
      case CALU6: i = _ALU6_; break;
      case ALU7:
      case CALU7: i = _ALU7_; break;
      case ALU8:
      case CALU8: i = _ALU8_; break;
      default:    continue;
    }

    number[i]++;

    if (pSeg->X1 == pSeg->X2) {
      /* Horizontal segments. */
      length[i] += (pSeg->Y2 - pSeg->Y1) / SCALE_X;
    } else {
      /* Vertical segments. */
      length[i] += (pSeg->X2 - pSeg->X1) / SCALE_X;
    }
  }

  for (i = 0; i < _MAX_ALU_; i++) { if (number[i] == 0) number[i]++; }


  total_VIA = 0;
  for (pVIA = pFig->PHVIA; pVIA != NULL; pVIA = pVIA->NEXT) {
    total_VIA++;
  }

  

  printf ("\n\n");
  printf ("  Routing statistics :\n\n");

  total_length = 0;
  total_number = 0;
  for (i = 0; i < _MAX_ALU_; i++) {
    printf ("  - ALU%d length  := %10ld", i+2, length[i]);
    printf ("  (average length := %ld)\n", length[i] / number[i]);

    total_length += length[i];
    total_number += number[i];
  }

  printf ("\n");
  printf ("  - Total length := %10ld" , total_length);
  printf ("  (average length := %ld)\n", total_length / total_number);

  printf ("\n");
  printf ("  - Total VIA    := %10ld\n" , total_VIA);
  printf ("\n\n");
  

  exit (0);
}
