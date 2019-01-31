#include "VisXV4.h"          /* VisionX structure include file       */
#include "Vutil.h"           /* VisionX utility header files         */

VXparam_t par[] =            /* command line structure               */
{
{    "if=",    0,   " input file  v3dmean: compute local mean"},
{    "of=",    0,   " output file "},
{    "-v",     0,   " visible flag"},
{     0,       0,    0}
};
#define  IVAL   par[0].val
#define  OVAL   par[1].val
#define  VFLAG  par[2].val

void sort(int arr[]) {         //sort function
	int i, j, temp;
	for (i = 0; i < 27; i++)
		for (j = 0; j < 26 - i; j++)
			if (arr[j] > arr[j + 1]) {
				temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
}

int
main(argc, argv)
int argc;
char *argv[];
{
V3fstruct (im);
V3fstruct (tm);
int        x,y,z;               /* index counters                 */
int        xx,yy,zz;            /* window index counters          */
int        sum;
int        temp[27];
int	   count;
    VXparse(&argc, &argv, par); /* parse the command line         */

    V3fread( &im, IVAL);        /* read 3D image                  */
    if ( im.type != VX_PBYTE || im.chan != 1) { /* check  format  */
       fprintf (stderr, "image not byte type or single channel\n");
       exit (1);
    }   
   
    V3fembed(&tm, &im, 1,1,1,1,1,1); /* temp image copy with border */
    if(VFLAG){
       fprintf(stderr,"bbx is %f %f %f %f %f %f\n", im.bbx[0],
                 im.bbx[1],im.bbx[2],im.bbx[3],im.bbx[4],im.bbx[5]);
    }

    for (z = im.zlo; z <= im.zhi; z++) {/* for all pixels */
      for (y = im.ylo; y <= im.yhi; y++) {
        for (x = im.xlo; x <= im.xhi; x++) {
             count = 0;
             for (zz = -1; zz <= 1; zz++) {/* compute the function */
               for (yy = -1; yy <= 1; yy++) {
                 for (xx = -1; xx <= 1; xx++) {
			  		temp[count]=tm.u[z + zz][y + yy][x + xx];
		    		count++;
                 }   
               }   
             } 
  			sort(temp);
             im.u[z][y][x] = temp[13];
        }
      }
   }
   V3fwrite (&im, OVAL);
   exit(0);
}
