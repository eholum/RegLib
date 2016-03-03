
 /*_C_INSERT_SAO_COPYRIGHT_HERE_(2007)_*/
 /*_C_INSERT_GPL_LICENSE_HERE_*/

 /*
  * Coverage tests for coordinates conversion. Including
  * regConvertWorldRegion (and indirectly regConvertWorldShape,
  * since regConvertWorldRegion just converts the shapes one
  * by one) and regRegionToList.
  *
  *
  */

#include "cxcregion.h"
#include "region_priv.h"
#include "regtest_utils.c"

// Test conversion function being passed to regConvertWorldRegion,
// simply divides coordinates by 2 (if they are world_coord)
//void invertTestFunction(double*, double*);
void convert_test(void);
void verify_region(regRegion *);
void test_invert_function(double *, double *);
void print_list(regRegion*);
void test_invert(regRegion * region);


int main(int argc, char* argv[]) {
    regRegion *myRegion;
    double xx[5] = {0,0,0,0,0};
    double yy[5] = {0,0,0,0,0};
    double rr[2] = {0,0};

    xx[0] = 8; yy[0] = 8;
    rr[0] = 3; rr[1] = 3;

    /*
     * Simple box conversion
     */
    printf(">> Test coordinate conversion for simple box:\n");
    
    myRegion = regCreateRegion(NULL, NULL);
    regCreateShape(myRegion, regAND, regBOX, regInclude, xx, yy, 1, rr, NULL, 3, 3);
    regPrintRegion(myRegion);
    printMask(myRegion);

    regConvertWorldRegion(myRegion, 5, test_invert_function);
    regPrintRegion(myRegion);
    printMask(myRegion);
    regFree(myRegion);


    /*
     * Force conversion from logical coords
     */
    printf(">> Test coordinate conversion for simple box (double the radii):\n");
    
    myRegion = regCreateRegion(NULL, NULL);
    regCreateShape(myRegion, regAND, regBOX, regInclude, xx, yy, 1, rr, NULL, 2, 2);
    regPrintRegion(myRegion);
    printMask(myRegion);

    regConvertRegion(myRegion, .5, test_invert_function, 1);
    regPrintRegion(myRegion);
    printMask(myRegion);
    regFree(myRegion);

    /*
     * Multiple shapes with only 1 in world_coord
     */
    printf(">> Test coordinate conversion for a mixture of shapes in different coords:\n");
    myRegion = regParse("box(8,8,3,3)+ellipse(8,4,3,1,0)");
    xx[0] = 6; yy[0] = 4;
    rr[0] = 1;
    regCreateShape(myRegion, regOR, regCIRCLE, regInclude, xx, yy, 1, rr, NULL, 3, 3);
    
    regPrintRegion(myRegion);
    printMask(myRegion);   

    printf(">> (Only circle is in world coord)\n");
    regConvertWorldRegion(myRegion, 5, test_invert_function);
    regPrintRegion(myRegion);
    printMask(myRegion);
    regFree(myRegion);

    /*
     * Parsing into world for coords and radii
     */
    printf(">> Coords in world, radii in world\n");
    myRegion = regParse("ellipse(00:24:00,04:00:00,200\',300\',0)");
    regPrintRegion(myRegion);
    printMask(myRegion);   
    regConvertWorldRegion(myRegion, 5, test_invert_function);
    regPrintRegion(myRegion);
    printMask(myRegion);
    regFree(myRegion);

    /*
     * Parsing logical radii
     */
    printf(">> Coords in logical, radii in logical force conversion\n");
    myRegion = regParse("ellipse(4,6,3i,5i,0)");
    regPrintRegion(myRegion);
    printMask(myRegion);   
    regConvertRegion(myRegion, 5, test_invert_function, 1);
    regPrintRegion(myRegion);
    printMask(myRegion);
    regFree(myRegion);

    /*
     * World coords, physical radii, force convert
     */
    printf(">> Coords in world, radii in physical, force conversion\n");
    myRegion = regParse("ellipse(00:24:00,04:00:00,5p,5p,0)");
    regPrintRegion(myRegion);
    printMask(myRegion);   
    regConvertRegion(myRegion, 2, test_invert_function, 1);
    regPrintRegion(myRegion);
    printMask(myRegion);
    regFree(myRegion);
    
    /*
     * Polygon
     */
    printf(">> Convert a polygon\n");
    xx[0] = 2; yy[0] = 2;
    xx[1] = 2; yy[1] = 8;
    xx[2] = 8; yy[2] = 8;
    xx[3] = 8; yy[3] = 2;
    xx[4] = 2; yy[4] = 2;
   
    myRegion = regCreateRegion(NULL, NULL); 
    regCreateShape(myRegion, regAND, regPOLYGON, regInclude, xx, yy, 4, rr, NULL, 3, 3);

    regPrintRegion(myRegion);
    printMask(myRegion);   

    regConvertWorldRegion(myRegion, 5, test_invert_function);
    regPrintRegion(myRegion);
    printMask(myRegion);
    regFree(myRegion);
    printf("\n");


    /*
     * regRegionToList creates a list of coordinates that are defined
     * to be "inside" the region over a given x and y range. 
     */
   
    /*
     * Empty region 
     */

    myRegion = NULL;
    printf(">> Testing regRegionToList null entry\n");
    regPrintRegion(myRegion);
    printMask(myRegion);

    printf("Converting to list of pixels:\n");
    print_list(myRegion);

    /*
     * circle
     */
    printf(">> Testing regRegionToList w/shape\n");

    myRegion = regParse("circle(5,5,2)");
    regPrintRegion(myRegion);
    printMask(myRegion);

    printf("Converting to list of pixels:\n");
    print_list(myRegion);

    regFree(myRegion);


    convert_test();


    /**
     *
     * Testing a regResolveField - used to convert the first shape in a region
     * to a bounding rectangle IF the first shape is a field.
     */
    xx[0] = 3; yy[0] = 3;
    xx[1] = 7; yy[1] = 7;

    printf(">> Testing regResolveField\n");
    
    myRegion = regParse("field()-circle(5,5,2)");
    regPrintRegion(myRegion);
    printMask(myRegion);
    printf("> resolving field:\n");
    regResolveField(myRegion, xx, yy);
    regPrintRegion(myRegion);
    printMask(myRegion);

    myRegion = regParse("!circle(5,5,2)*field()");
    regPrintRegion(myRegion);
    printMask(myRegion);
    printf("> resolving field:\n");
    regResolveField(myRegion, xx, yy);
    regPrintRegion(myRegion);
    printMask(myRegion);
    
    return 0;
}



void convert_test() {

  regRegion * region;
  double xx[5] = {0,0,0,0,0};
  double yy[5] = {0,0,0,0,0};
  double rr[2] = {0,0};
  double aa[2] = {10,0};
  long npoints = 2;

  // Null region test                                                                                                                                          
  printf("\n>>> Null region:\n");
  region = NULL;
  verify_region(region);
  test_invert(region);
  printf("> mask:\n");
  printMask(region);
  printf("> list:\n");
  print_list(region);


  // Empty region test                                                                                                                                         
  printf("\n>>> Empty region:\n");
  region = regCreateEmptyRegion();
  verify_region(region);
  test_invert(region);
  print_list(region);


  // Box test                                                                                                                                                  
  printf("\n>>> Box:\n");
  xx[0] = 4; yy[0] = 6;
  rr[0] = 4; rr[1] = 5;
  regCreateShape(region, regOR, regBOX, regInclude, xx, yy, npoints, rr, aa, 1, 1);
  verify_region(region);
  test_invert(region);


  // Ellipse and box test                                                                                                                                      
  printf("\n>>> Elipse and box:\n");
  xx[0] = 9; yy[0] = 6;
  rr[0] = 6; rr[1] = 1.5;
  regCreateShape(region, regAND, regELLIPSE, regInclude, xx, yy, npoints, rr, aa, 0, 0);
  verify_region(region);
  test_invert(region);
  printf("> list:\n");
  print_list(region);


  // Adding a few more shapes for inverted test                                                                                                                
  printf("\n>>> Invert test:\n");
  xx[0] = 1; yy[0] = 1;
  rr[0] = 1; rr[1] = 1;
  regCreateShape(region, regOR, regCIRCLE, regInclude, xx, yy, npoints, rr, aa, 0, 0);
  regCreateShape(region, regAND, regANNULUS, regInclude, xx, yy, npoints, rr, aa, 0, 0);
  regCreateShape(region, regAND, regSECTOR, regInclude, xx, yy, npoints, aa, rr, 0, 0);
  regPrintRegion(region);

  // Expected = !Box&!Circle|!Box&!Annulus|!Box&!Sector|                                                                                                       
  //            !Ellipse&!Circle|!Ellipse&!Annulus|!Ellipse&!Sector                                                                                            
  test_invert(region);

  printf("> region mask:\n");
  printMask(region);
  printf("\n> inverted region mask:\n");
  printMask(regInvert(region));

  regFree(region);
}


void print_list( regRegion* region ) {
  double *xat, *yat;
  long nat, ii;

  regRegionToList(region, 0, 9, 0, 9, 1, &xat, &yat, &nat);
  printf("number of points inside region=%lu\n", nat);
  for (ii=0; ii<nat; ii++) {
    printf("(%.1f, %.1f) ", xat[ii], yat[ii]);
  }
  printf("\n");
}


void test_invert_function(double* a, double* b) {
    // Just divide the input by 2 and call it good.
    b[0] = a[0] / 2.0;
    b[1] = a[1] / 2.0;
}


void test_invert(regRegion * region) {
  regRegion * invertedRegion;

  invertedRegion = regInvert(region);
  printf("> invertedRegion:\n");
  regPrintRegion(invertedRegion);

  regFree(invertedRegion);
}


void verify_region(regRegion * region) {
  regRegion * convertedRegion;
  regPrintRegion(region);

  convertedRegion = regCopyRegion(region);
  regConvertWorldRegion(convertedRegion, 3.0, test_invert_function);
  printf("> converted region:\n");
  regPrintRegion(convertedRegion);
  regFree(convertedRegion);

  convertedRegion = regCopyRegion(region);
  regConvertRegion(convertedRegion, 3.0, test_invert_function, 1);
  printf("> converted region (forced):\n");
  regPrintRegion(convertedRegion);
  regFree(convertedRegion);
  
}

