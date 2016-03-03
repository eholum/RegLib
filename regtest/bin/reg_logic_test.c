
/*_C_INSERT_SAO_COPYRIGHT_HERE_(2007)_*/
/*_C_INSERT_GPL_LICENSE_HERE_*/

 /*
  * Includes coverage tests for regUnionRegion, regCombineRegion
  * regInvert, regIntersectRegion, and regCompareRegion.
  */

#include "cxcregion.h"
#include "region_priv.h"
#include "regtest_utils.c"

void regUnionRegion1( regRegion *, regRegion * );
void verify(regRegion *, regRegion *);
void logic_test(void);
void eval_test(void);


int main( int argc, char* argv[] )
{
    
    regRegion *A, *B, *temp, *check;
    short equality;

    /*
     * Checking trivial/edge cases.
     *
     */

    // Both null
    printf(">> Verifying null regions\n");
    printf("Should end up with a null region:\n");
    check = regUnionRegion( NULL, NULL );
    regPrintRegion( check );
    regFree( check );
    printf("\n\n");

    // One null
    printf(">> Verifying one non-null, one null\n");
    
    printf("In region:\n");
    A = regParse( "box(3,3,2,2)*box(1,1,2,2)+box(4,4,2,2)" );
    regPrintRegion( A );
    
    check = regUnionRegion( NULL, A );
    printf("Out region:\n");
    regPrintRegion( A );
    
    printf("regions are equal=%d (should be 1)\n\n", regCompareRegion(check, A));
    regFree( check );
    
    /*
     * Perform simple Union check on A and B.
     */
    printf(">> Verifying overlapping, complex regions\n");
    A = regParse("box(3,3,2,2)*box(1,1,2,2)+box(4,4,2,2)");
    B = regParse("box(8,8,2,2)+box(6,2,2,2)");

    printf("region A:\n");
    regPrintRegion(A);
    printf("region B:\n");
    regPrintRegion(B);
    
    printf("regions are equal=%d (should be 0)\n\n", regCompareRegion(B, A));
    
    check = regUnionRegion( A, B );
    printf("A union B:\n");
    regPrintRegion(check);

    temp = regParse("box(3,3,2,2)*box(1,1,2,2)+box(4,4,2,2)+box(8,8,2,2)+box(6,2,2,2)" );
    equality = regCompareRegion( temp, check );
    printf("regions are equal=%d (should be 1)\n\n", equality);

    regFree(A); regFree(B); regFree(temp); regFree(check);

    /*
     * Inverting non-overlapping region
     */
    printf(">> Inverting a simple region verification\n");
    printf("In region:\n");
    A = regParse("box(4,4,6,6)");
    regPrintRegion(A);
    printMask(A);

    printf("Out region:\n");
    temp = regInvert(A);
    regPrintRegion(temp);
    printMask(temp);
    
    printf("Intersection of in/out should be null region:\n");
    check = regIntersectRegion(A, temp);
    regPrintRegion(check);
    printMask(check);

    regFree(A); regFree(temp); regFree(check);
    
    /*
     * Inverting a more complex region.
     */
    printf(">> Inverting a more complex region");
    printf("In region:\n");
    A = regParse("box(3,3,2,2)*box(1,1,2,2)+box(4,4,2,2)+box(8,8,2,2)+box(6,2,2,2)" );
    regPrintRegion(A);
    printMask(A);

    printf("Out region:\n");
    temp = regInvert(A);
    regPrintRegion(temp);
    printMask(temp);
    
    printf("Intersection of in/out should be null region:\n");
    check = regIntersectRegion(A, temp);
    regPrintRegion(check);
    printMask(check);
    
    regFree(A);  regFree(temp); regFree(check);

    /*
     * Intersecting two regions.
     */
    printf(">> Intersection of two non-trivial regions\n");
    printf("Region A:\n");
    A = regParse("box(4,4,3,3)+box(6,6,3,3)");
    regPrintRegion(A);
    printMask(A);
    
    printf("Region B:\n");
    B = regParse("circle(5,5,2)*!box(6,6,2,2)"); 
    regPrintRegion(B);
    printMask(B);

    printf("Intersection:\n");
    temp = regIntersectRegion(A, B);
    
    printf("NShapes: %li\n", regGetNoShapes(temp));

    regPrintRegion(temp);
    printMask(temp); 

    regFree(A); regFree(B);  regFree(temp); 


    eval_test();
    logic_test();

    return 0;
}


void eval_test() {

  regRegion * region;
  regRegion * cmpRegion;

  regShape * shape;
  double xx[5] = {0,0,0,0,0};
  double yy[5] = {0,0,0,0,0};
  double rr[2] = {0,0};
  double aa[2] = {0,0};
  long npoints;
  char buf[120] = "";

  // Empty region test                                                                                                                                         
  printf(">>> Empty regions test:\n");
  region = regCreateEmptyRegion();
  cmpRegion = regCreateEmptyRegion();

  printf("Regions are equal=%d (yes)\n", regCompareRegion(region, cmpRegion));
  printf("Max points=%lu (0)\n", regGetMaxPoints(region));
  printf("Number of shapes=%lu (0)\n", regGetNoShapes(region));
  printf("Inside region=%d (0)\n", regInsideRegion(region, 5, 5));

  // Simple box test                                                                                                                                           
  printf("\n>>> Simple inverted check:\n");
  xx[0] = 5; yy[0] = 5;
  rr[0] = 4; rr[1] = 6;
  npoints = 1;
  regCreateShape(region, regAND, regBOX, regExclude, xx, yy, npoints, rr, aa, 0, 0);
  regPrintRegion(region);

  printf("Regions are equal=%d (no)\n", regCompareRegion(region, cmpRegion));
  regCreateShape(cmpRegion, regAND, regBOX, regExclude, xx, yy, npoints, rr, aa, 0, 0);
  printf("Regions are equal=%d (yes)\n", regCompareRegion(region, cmpRegion));

  printf("Max points=%lu (1)\n", regGetMaxPoints(region));
  printf("Number of shapes=%lu (1)\n", regGetNoShapes(region));
  printf("Inside region=%d (0)\n", regInsideRegion(region, 5, 5));
  printf("Inside region=%d (1)\n", regInsideRegion(region, -1, -1));

  printf(">>> Print the box:\n");
  shape = regGetShapeNo(region, 1);
  shape->toString(shape, buf, 120);
  printf("%s\n", buf);


  // Add a polygon                                                                                                                                             
  printf("\n>>> Box and polygon check:\n");
  xx[0] = 5;  yy[0] = 5;
  xx[1] = 10; yy[1] = 5;
  xx[2] = 10; yy[2] = 10;
  xx[3] = 5;  yy[3] = 10;
  xx[4] = 5;  yy[4] = 5;
  npoints = 5;
  regCreateShape(region, regAND, regPOLYGON, regInclude, xx, yy, npoints, rr, aa, 0, 0);
  regPrintRegion(region);

  printf("Regions are equal=%d (no)\n", regCompareRegion(region, cmpRegion));
  regCreateShape(cmpRegion, regAND, regPOLYGON, regInclude, xx, yy, npoints, rr, aa, 0, 1);
  printf("Regions are equal=%d (yes)\n", regCompareRegion(region, cmpRegion));

  printf("Max points=%lu (5)\n", regGetMaxPoints(region));
  printf("Number of shapes=%lu (2)\n", regGetNoShapes(region));
  printf("Inside region=%d (0)\n", regInsideRegion(region, 4, 4));
  printf("Inside region=%d (1)\n", regInsideRegion(region, 9, 9));

  printf(">>> Print the polygon:\n");
  shape = regGetShapeNo(region, 2);
  shape->toString(shape, buf, 120);
  printf("%s\n", buf);

  printf("\n>>> Print nothing:\n");
  shape = regGetShapeNo(region, 3);
  printf("Shape is null=%d (1)\n", !shape);

  // Rectangles tests                                                                                                                                          
  printf("\n>>> Rectangles test:\n");
  region = regCreateEmptyRegion();
  cmpRegion = regCreateEmptyRegion();
  xx[0] = 10; yy[0] = 10;
  xx[1] = 14; yy[1] = 15;
  npoints = 2;
  regCreateShape(region, regOR, regRECTANGLE, regInclude, xx, yy, npoints, rr, aa, 0, 0);
  regPrintRegion(region);


  xx[0] = 11; yy[0] = 11;
  xx[1] = 13; yy[1] = 14;
  npoints = 2;
  regCreateShape(cmpRegion, regOR, regRECTANGLE, regInclude, xx, yy, npoints, rr, aa, 0, 0);
  regPrintRegion(cmpRegion);
  printf("Regions are equal=%d (no)\n", regCompareRegion(region, cmpRegion));
  printf("Rectangles are equal=%d (no)\n",
	 region->shape->isEqual(region->shape, cmpRegion->shape));

  regFree(region);
  regFree(cmpRegion);



}


void logic_test() {

  regRegion * region1;
  regRegion * region2;
  double xx[5] = {0,0,0,0,0};
  double yy[5] = {0,0,0,0,0};
  double rr[2] = {0,0};
  double aa[2] = {4,0};
  long npoints;


  // Empty region test                                                                                                                                         
  printf("\n>>> Empty test\n");
  region1 = regCreateEmptyRegion();
  region2 = regCreateEmptyRegion();
  verify(region1, region2);

  // One null region test                                                                                                                                      
  printf("\n>>> One null region test\n");
  xx[0] = 17; yy[0] = 17;
  rr[0] = 1; rr[1] = 3;
  npoints = 1;
  regCreateShape(region1, regOR, regANNULUS, regInclude, xx, yy, npoints, rr, aa, 0, 0);
  verify(region1, region2);

  // Two shapes in first region                                                                                                                                
  printf("\n>>> Multiple shapes in first region\n");
  xx[0] = 4; yy[0] = 4;
  rr[0] = 2; rr[1] = 3;
  npoints = 1;
  regCreateShape(region1, regOR, regBOX, regInclude, xx, yy, npoints, rr, aa, 0, 0);
  verify(region1, region2);

  // Overlapping box test                                                                                                                                      
  printf("\n>>> Overlapping boxes\n");
  xx[0] = 3; yy[0] = 3;
  rr[0] = 2; rr[1] = 3;
  npoints = 1;
  regCreateShape(region2, regAND, regBOX, regInclude, xx, yy, npoints, rr, aa, 0, 0);
  verify(region1, region2);

  // Special rectangle cases                                                                                                                                   
  region1 = regCreateEmptyRegion();
  region2 = regCreateEmptyRegion();
  printf("\n>>> Rectangles have special cases\n");
  xx[0] = 10; yy[0] = 10;
  xx[1] = 14; yy[1] = 15;
  npoints = 2;
  regCreateShape(region2, regOR, regRECTANGLE, regInclude, xx, yy, npoints, rr, aa, 0, 0);
  verify(region1, region2);

  printf("\n>>> Rectangles have additional special cases (containment)\n");
  xx[0] = 11; yy[0] = 11;
  xx[1] = 13; yy[1] = 14;
  npoints = 2;
  regCreateShape(region1, regOR, regRECTANGLE, regInclude, xx, yy, npoints, rr, aa, 0, 0);
  verify(region1, region2);

  // NOTE: There are some fuzzy things around these test cases. In particular, new code                                                                        
  // will correctly overlap rectangles into a merged single shape, whereas old code would                                                                      
  // not. In this test case the old code produces the following rectangles:                                                                                    
  //                                                                                                                                                           
  //    1   Rectangle(11.000000, 11.000000, 13.000000, 14.000000)                                                                                              
  //    1   Rectangle(10.000000, 10.000000, 14.000000, 15.000000)                                                                                              
  //    2   Rectangle(14.000000, 14.000000, 18.000000, 18.000000)                                                                                              
  //    2   Rectangle(10.000000, 10.000000, 14.000000, 15.000000)                                                                                              
  //                                                                                                                                                           
  // Where as new code wraps these up into two single rectangles:                                                                                              
  //    1   Rectangle(11.000000, 11.000000, 13.000000, 14.000000)                                                                                              
  //    2   Rectangle(14.000000, 14.000000, 14.000000, 15.000000)                                                                                              
  //                                                                                                                                                           
  // Which are equivalent regions.                                                                                                                             
  printf("\n>>> Rectangles have additional special cases (overlap)\n");
  xx[0] = 14; yy[0] = 14;
  xx[1] = 18; yy[1] = 18;
  npoints = 2;
  regCreateShape(region1, regOR, regRECTANGLE, regInclude, xx, yy, npoints, rr, aa, 0, 0);
  verify(region1, region2);


  regFree(region1);
  regFree(region2);

}

void verify(regRegion* region1, regRegion* region2) {
  regRegion * result;

  printf("> Region 1:\n");
  printf("%s \n", regToStringRegion(region1));
  printf("> Region 2:\n");
  printf("%s \n", regToStringRegion(region2));

  printf("> regCombineRegion:\n");
  result = regCombineRegion(region1, region2);
  printf("%s \n", regToStringRegion(result));
  regFree(result);

  printf("> regUnionRegion:\n");
  result = regUnionRegion(region1, region2);
  printf("%s \n", regToStringRegion(result));
  regFree(result);

  printf("> regIntersectRegion:\n");
  result = regIntersectRegion(region1, region2);
  printf("%s \n", regToStringRegion(result));
  regFree(result);

}
