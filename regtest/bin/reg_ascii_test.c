/*_C_INSERT_SAO_COPYRIGHT_HERE_(1997,2007)_*/
/*_C_INSERT_GPL_LICENSE_HERE_*/

#include "cxcregion.h"
#include"region_priv.h"
#include <stdlib.h>
#include <stdio.h>

void verify_region(regRegion *, char ** names, long nn);
void repr_test(void);

int main(int argc, char **argv)
{

  if (argc != 3) {
    printf( "Usage: asciitest infile outfile\n");
    return 1;
  }

  regRegion *myRegion;
  char* infile = argv[1];
  char* outfile = argv[2]; 

  // read in input region file
  myRegion = regReadAsciiRegion( infile, 0);

  // write out ascii region file
  int status = regWriteAsciiRegion( outfile, myRegion, NULL, 0); 
  if (status != 1)
    return 1;

  regFree(myRegion);

  repr_test();

  return 0;
}


void repr_test() {

  regRegion * region;
  double xx[5] = {0,0,0,0,0};
  double yy[5] = {0,0,0,0,0};
  double rr[2] = {0,0};
  double aa[2] = {0,0};
  long npoints;
  char ** names = malloc(20 * sizeof(char*));
  long nn = 0;


  // Null region test                                                                                                                                          
  printf("\n>>> Null region:\n");
  region = NULL;
  verify_region(region, names, nn);

  // Empty region test                                                                                                                                         
  printf("\n>>> Empty region:\n");
  region = regCreateEmptyRegion();
  verify_region(region, names, nn);

  // Add a point                                                                                                                                               
  printf("\n>>> Point test:\n");
  xx[0] = 5000; yy[0] = 50000;
  npoints = 1;
  names[nn] = (char*)"point";
  regCreateShape(region, regAND, regPOINT, regInclude, xx, yy, npoints, rr, aa, 3, 3);
  verify_region(region, names, nn++);

  // Exclude or a box                                                                                                                                          
  printf("\n>>> Box test:\n");
  xx[0] = 5;
  rr[0] = 10; rr[1] = 10;
  aa[0] = 390;
  names[nn] = (char*)"box";
  npoints = 2;
  regCreateShape(region, regOR, regBOX, regExclude, xx, yy, npoints, rr, aa, 3, 3);
  verify_region(region, names, nn++);

  // Add a polygon                                                                                                                                             
  printf("\n>>> Polygon test:\n");
  xx[0] = 5;  yy[0] = 5;
  xx[1] = 10; yy[1] = 5;
  xx[2] = 10; yy[2] = 10;
  xx[3] = 5;  yy[3] = 10;
  xx[4] = 5;  yy[4] = 5;
  npoints = 5;
  names[nn] = (char*)"big ol polygon";
  regCreateShape(region, regAND, regPOLYGON, regInclude, xx, yy, npoints, rr, aa, 1, 1);
  verify_region(region, names, nn++);

  // Annulus (This was broken in previous code)                                                                                                                
  printf("\n>>> Annulus test:\n");
  xx[0] = 17; yy[0] = 17;
  rr[0] = 1; rr[1] = 3;
  npoints = 1;
  names[nn] = (char*)"annulus...";
  regCreateShape(region, regOR, regANNULUS, regExclude, xx, yy, npoints, rr, aa, 2, 2);
  verify_region(region, names, nn++);

  // CIRCLE

  regFree(region);
  region = regCreateEmptyRegion();
  printf("\n>>> Circle test:\n");
  xx[0] = 17.1111111; yy[0] = 17.1111111;
  rr[0] = 690;
  npoints = 1;
  names[nn] = (char*)"circle";
  regCreateShape(region, regOR, regCIRCLE, regExclude, xx, yy, npoints, rr, aa, 2, 2);
  verify_region(region, names, nn++);
  
  // ELLIPSE

  regFree(region);
  region = regCreateEmptyRegion();
  printf("\n>>> Ellipse test:\n");
  xx[0] = 17.1111111; yy[0] = 17.1111111;
  rr[0] = 1; rr[1] = 2;
  aa[0] = -60;
  npoints = 1;
  names[nn] = (char*)"ellipse";
  regCreateShape(region, regOR, regELLIPSE, regExclude, xx, yy, npoints, rr, aa, 3, 3);
  verify_region(region, names, nn++);
  
  // PIE

  regFree(region);
  region = regCreateEmptyRegion();
  printf("\n>>> Pie test:\n");
  xx[0] = 17.11; yy[0] = 17.111;
  rr[0] = 1; rr[1] = 2;
  aa[0] = -60; aa[1] = 3;
  npoints = 2;
  names[nn] = (char*)"pie";
  regCreateShape(region, regOR, regPIE, regExclude, xx, yy, npoints, rr, aa, 1, 1);
  verify_region(region, names, nn++);
  
  // RECTANGLE

  regFree(region);
  region = regCreateEmptyRegion();
  printf("\n>>> Rectangle test:\n");
  xx[0] = 17.11; yy[0] = 17.111;
  xx[1] = 20; yy[1] = 20;
  npoints = 2;
  names[nn] = (char*)"rectangle";
  regCreateShape(region, regOR, regRECTANGLE, regExclude, xx, yy, npoints, rr, aa, 1, 1);
  verify_region(region, names, nn++);
  
  // Sector

  regFree(region);
  region = regCreateEmptyRegion();
  printf("\n>>> Sector test:\n");
  xx[0] = 17.11; yy[0] = 17.111;
  aa[0] = 20; aa[1] = 90;
  npoints = 1;
  names[nn] = (char*)"sector";
  regCreateShape(region, regOR, regSECTOR, regExclude, xx, yy, npoints, rr, aa, 3, 3);
  verify_region(region, names, nn++);
  regFree(region);

}


void verify_region(regRegion * region, char **buf, long nn) {
  long maxlen = 512;
  char *names = malloc(maxlen * sizeof(char));
  char *rstring;

  printf("\n> regPrintRegion: \n");
  regPrintRegion(region);

  rstring = regToStringRegion(region);
  printf("\n> regToStringRegion: \n %s", rstring);

  regComposeRegion(region, names, maxlen);
  printf("\n> regComposeRegion:\n %s", names);
  printf("\nregComposeRegion == regToStringRegion ? %d (0)\n", strcmp(rstring, names));

  printf("\n> regWriteAsciiRegion:\n");
  regWriteAsciiRegion("stdout", region, buf, nn);

  free(rstring);
  free(names);
}


