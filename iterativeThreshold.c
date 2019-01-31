/*********************************************************************/
/* vits.c     */
/*********************************************************************/

#include "VisXV4.h"          /* VisionX structure include file       */
#include "Vutil.h"           /* VisionX utility header files         */
#include <stdio.h>

VXparam_t par[] =            /* command line structure               */
{
{  "if=",   0,   " input file, vtpeak: threshold between hgram peaks"},
{  "of=",   0,   " output file "},
{  "d=",    0,   " min dist between hgram peaks (default 10)"},
{  "-v",    0,   "(verbose) print threshold information"},
{   0,      0,   0} /* list termination */
};
#define  IVAL   par[0].val
#define  OVAL   par[1].val
#define  DVAL   par[2].val
#define  VFLAG  par[3].val

main(argc, argv)
int argc;
char *argv[];
{
	

    Vfstruct (im);                 /* input image structure          */
    int y,x;                       /* index counters                 */
    int i;

    int hist[256];                 /* histogram bins                 */
    int thresh;                    /* threshold                      */

	int R1;                    /* average1                      */
	int R2;                    /* average2                      */


    int maxbin;                    /* maximum histogram bin          */
    int nxtbin;                    /* second maximum bin             */
    int minbin;                    /* minumim histogram bin          */
    int maxa, maxb;       /* second maximum bin above/below maxbin   */
    int dist;                   /* minimum distance between maxima   */

	   //average grey value of whole image 
			     
    VXparse(&argc, &argv, par);    /* parse the command line         */

    dist = 10;                    /* default dist */
    if (DVAL) dist = atoi(DVAL);  /* if d= was specified, get value */
    if (dist < 0 || dist > 255) {
	fprintf(stderr, "d= must be between 0 and 255\nUsing d=10\n");
        dist = 10;
    }

    while ( Vfread( &im, IVAL) ) {
        if ( im.type != VX_PBYTE ) {
              fprintf (stderr, "error: image not byte type\n");
              exit (1);
        }

        /* clear the histogram */
        for (i = 0; i < 256; i++) hist[i] = 0;
 
        /* compute the histogram */
        for (y = im.ylo; y <= im.yhi; y++)
            for (x = im.xlo; x <= im.xhi; x++)
                 hist[im.u[y][x]]++;

	/*computing function: the arverage gray value*/
	 int average(int begin,int end){
		int avr=0;
		int count=0;
		int x,y;
		for (y = im.ylo;y <= im.yhi;y++) {
		    for (x = im.xlo; x <= im.xhi; x++) {
		         if (im.u[y][x] >= begin&&im.u[y][x]<end){
				avr+=im.u[y][x];
				count++;
				}
		    }
		}
		if (count==0) return 0;
		avr/=count;
		return avr;
	}
	
       
	/*compute the threshold*/
	int temp1=0;
	int temp2=0;
	int whilecount=0;
	thresh=average(0,256);
	while(1){
	temp1=average(0,thresh);
	temp2=average(thresh,256);
	printf("R1 = %d, R2 = %d\n",temp1,temp2);    
	if(temp1!=R1||temp2!=R2){             //if R1 and R2 don't change anymore, break the loop
		R1=temp1;
		R2=temp2;
	}
	else if(temp1==R1||temp2==R2) break;
	if(R1==0||R2==0){                     //if R1 or R1 equals 0, break the loop
		thresh=R1+R2;
		break;
	
	}
	thresh=(R1+R2)/2;                     //compute a new threshold
	whilecount++;
	if(whilecount==100) break;            //ensure this while loop will not run endlessly
	}
	
	printf("thresh   =   %d\n",thresh);

  
        /* apply the threshold */
        for (y = im.ylo; y <= im.yhi; y++) {
            for (x = im.xlo; x <= im.xhi; x++) {
                 if (im.u[y][x] >= thresh) im.u[y][x] = 255;
                 else                      im.u[y][x] = 0;
            }
        }
  
        Vfwrite( &im, OVAL);
    } /* end of every frame section */
    exit(0);
}
