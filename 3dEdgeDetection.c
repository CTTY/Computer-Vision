#include "VisXV4.h"          /* VisionX structure include file       */
#include "Vutil.h"           /* VisionX utility header files         */
#include "math.h" 

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


int
main(argc, argv)
int argc;
char *argv[];
{
V3fstruct (im);
V3fstruct (tm);

int        x,y,z;               /* index counters                 */
int        xx,yy,zz;            /* window index counters          */
int	i,j,k;
int        sum;
int	   count=0;
int	   countx;
int	   county;
int	   countz;



int	sobel3dkerx[3][3][3]; 					//bulid the sobel kernel
int	sobel3dkery[3][3][3];
int	sobel3dkerz[3][3][3];

int	sobel[3]={1,2,1};
int	sobelp[3]={1,0,-1};

for(i=0;i<=2;i++){
	for(j=0;j<=2;j++){
		for(k=0;k<=2;k++){
			sobel3dkerx[i][j][k]=sobelp[i]*sobel[j]*sobel[k];
			sobel3dkery[i][j][k]=sobel[i]*sobelp[j]*sobel[k];
			sobel3dkerz[i][j][k]=sobel[i]*sobel[j]*sobelp[k];
		}
	}
}

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
		countx=0;
		county=0;
		countz=0;
             for (zz = -1; zz <= 1; zz++) {/* compute the function */
               for (yy = -1; yy <= 1; yy++) {
                 for (xx = -1; xx <= 1; xx++) {
			countx+=sobel3dkerx[zz+1][yy+1][xx+1]*tm.u[z-zz][y-yy][x-xx]; //convolve sobel operator with the image
			county+=sobel3dkery[zz+1][yy+1][xx+1]*tm.u[z-zz][y-yy][x-xx];
			countz+=sobel3dkerz[zz+1][yy+1][xx+1]*tm.u[z-zz][y-yy][x-xx];
			
		     
                 }   
               }   
             } 
  		countx/=10;
		county/=10;
		countz/=10;
		count=sqrt(countx*countx+county*county+countz*countz);
		if(count>70) im.u[z][y][x]=255;
		else im.u[z][y][x]=0;
        }
      }
   }
   V3fwrite (&im, OVAL);
   exit(0);
}
