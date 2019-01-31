/*******************************************************************/
/* vgrow.c*/
/*******************************************************************/

#include "VisXV4.h"           /* VisionX structure include file    */
#include "Vutil.h"            /* VisionX utility header files      */
#include "stdio.h"

Vfstruct (im);                      /* i/o image structure          */
Vfstruct (tm);                      /* temp image structure         */


VXparam_t par[] =             /* command line structure            */
{ /* prefix, value,   description                         */   
{    "if=",    0,   " input file  vtemp: local max filter "},
{    "of=",    0,   " output file "},
{     "-p",    0,   "pflag"},
{     "r=",    0,   "range"},
{     0,       0,   0}  /* list termination */
};
#define  IVAL   par[0].val
#define  OVAL   par[1].val
#define  PFLAG   par[2].val
#define  RFLAG   par[3].val

int range=10;          /*Range for region growing, default value is 10*/
int first=0;   /*set the first for labeling*/

void setlabel(int x,int y,int l)
{
	
	im.u[y][x]=l;        //label this pixel
	if(y<im.yhi&&abs(tm.u[y+1][x]-first)<range&&      
				im.u[y+1][x]==0&&
				tm.u[y+1][x]!=0) setlabel(x,y+1,l);
	if(y>im.ylo&&abs(tm.u[y-1][x]-first)<range&&
				im.u[y-1][x]==0&&
				tm.u[y-1][x]!=0) setlabel(x,y-1,l);
	if(x<im.xhi&&abs(tm.u[y][x+1]-first)<range&&
				im.u[y][x+1]==0&&
				tm.u[y][x+1]!=0) setlabel(x+1,y,l);
	if(x>im.xlo&&abs(tm.u[y][x-1]-first)<range&&
				im.u[y][x-1]==0&&
				tm.u[y][x-1]!=0) setlabel(x-1,y,l);

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


	if(RFLAG) range=atoi(RFLAG);        /* Read range value*/



  for (y = im.ylo ; y <= im.yhi ; y++) {  
     for (x = im.xlo; x <= im.xhi; x++)  {     //clear the output image, im, set all labels to 0
          im.u[y][x] = 0;
     }
   }


int L=1;        			 //set the initial label to 1

 for (y = tm.ylo ; y <= tm.yhi ; y++) {  
     for (x = tm.xlo; x <= tm.xhi; x++)  {     //search tm for objects
          if(tm.u[y][x]!=0&&im.u[y][x]==0){    //is a object and not labeled
		first=tm.u[y][x];
		if(PFLAG){                          //if pflag is true, use the first value as label
			first=tm.u[y][x];
			setlabel(x,y,first);
		}
		else{
			setlabel(x,y,L);
			L++;
			if(L==256) L=1;
		}
	  }						
     }
   }


   Vfwrite(&im, OVAL);             /* write image file                */
   exit(0);
}
