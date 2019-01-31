/*********************************************************************/
/* vsmed   Compute local 1x1xn median using the buffer method        */
/*********************************************************************/

#include "VisXV4.h"          /* VisionX structure include file       */
#include "Vutil.h"           /* VisionX utility header files         */

VXparam_t par[] =            /* command line structure               */
{
{    "if=",    0,   " input file vssum: compute temporal mean"},
{    "of=",    0,   " output file "},
{     0,       0,    0}
};
#define  IVAL   par[0].val
#define  OVAL   par[1].val

void sort(int arr[]) {         //sort function
	int i, j, temp;
	for (i = 0; i < 3; i++)
		for (j = 0; j < 2 - i; j++)
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
int sorted[3];
int        x,y,z;           /* index counters                 */

VXparse(&argc, &argv, par); /* parse the command line    */

    while (Vbfread( &im, IVAL, 3)) {
	if ( im.type != VX_PBYTE || im.chan != 1) { /* check format  */
           fprintf (stderr, "image not byte type\n");
           exit (1);
        }
        for (y = im.ylo; y <= im.yhi; y++) {
           for (x = im.xlo; x <= im.xhi; x++) {
			for (z = im.zlo; z <= im.zhi; z++) {
                   sorted[z]=im.u[z][y][x];        //read pixel values from 3 frames into an array
				
              	}
		sort(sorted);                //sort 3 frames
		im.u[0][y][x]=sorted[1];     //out put the median frame
            }
        }
        V3fwrite (&im, OVAL); /* write the oldest frame */
    }
    exit(0);
}
