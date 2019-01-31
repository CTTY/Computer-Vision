#include "VisXV4.h"           /* VisionX structure include file    */
#include "Vutil.h"            /* VisionX utility header files      */

Vfstruct (im);                      /* i/o image structure          */
Vfstruct (tm);                      /* temp image structure         */

VXparam_t par[] =             /* command line structure            */
{ /* prefix, value,   description                         */   
{    "if=",    0,   " input file  vtemp: local max filter "},
{    "of=",    0,   " output file "},
{     0,       0,   0}  /* list termination */
};
#define  IVAL   par[0].val
#define  OVAL   par[1].val
setlabel(x,y,L)
{
	im.u[y][x]=L;        //save label as integer elements
	if(tm.u[y+1][x]==tm.u[y][x]&&im.u[y+1][x]==0) setlabel(x,y+1,L);
	if(tm.u[y-1][x]==tm.u[y][x]&&im.u[y-1][x]==0) setlabel(x,y-1,L);
	if(tm.u[y][x+1]==tm.u[y][x]&&im.u[y][x+1]==0) setlabel(x+1,y,L);
	if(tm.u[y][x-1]==tm.u[y][x]&&im.u[y][x-1]==0) setlabel(x-1,y,L);
}
main(argc, argv)
int argc;
char *argv[];
{

int        y,x;                     /* index counters               */
  VXparse(&argc, &argv, par);       /* parse the command line       */

  Vfread(&im, IVAL);                /* read image file              */
  Vfembed(&tm, &im, 1,1,1,1);       /* image structure with border  */
  if ( im.type != VX_PBYTE ) {      /* check image format           */
     fprintf(stderr, "vtemp: no byte image data in input file\n");
     exit(-1);
  }

for (y = im.ylo ; y <= im.yhi ; y++) {  
     for (x = im.xlo; x <= im.xhi; x++)  {     //clear the output image, im, set all labels to 0
          im.u[y][x] = 0;
     }
   }
int L=1;        			 //set the initial label to 1

for (y = tm.ylo ; y <= tm.yhi ; y++) {  
     for (x = tm.xlo; x <= tm.xhi; x++)  {     //search tm for objects
          if(tm.u[y][x]!=0&&im.u[y][x]==0){    //is an object and not labeled
			setlabel(x,y,L);
			L++;
	      }						
     }
  }

   Vfwrite(&im, OVAL);             /* write image file                */
   exit(0);
}
