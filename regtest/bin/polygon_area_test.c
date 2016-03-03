 /*_C_INSERT_SAO_COPYRIGHT_HERE_(2007)_*/
 /*_C_INSERT_GPL_LICENSE_HERE_*/

 /*
  * Simple regression test for bug number 9575:
  * BUG (region): area of polygon is not correct for intersecting/non-simple polygons
  *
  * Tracked in:
  * https://github.com/eholum/RegionLibraryTasks/issues/3
  */
#include "cxcregion.h"
#include "region_priv.h"
#include "regtest_utils.c"

int main( int argc, char* argv[] )
{
    regRegion *myRegion;

    double area;
    double fieldx[2] = {0,0};
    double fieldy[2] = {0,0};
    double xpos[2] = {0,0};
    double ypos[2] = {0,0};  

    int within_field;
 
    fieldx[0] = 0; fieldy[0] = 0;
    fieldx[1] = 10; fieldy[1] = 10;

    
    myRegion = regParse( "poly(0,0,3,3,3,0,0,3,0,0)" );
    regPrintRegion( myRegion );
    printMask(myRegion);
    printf( "regInsideRegion = %d (1)\n", regInsideRegion(myRegion, 1, 1));
    printf( "regInsideRegion = %d (0)\n", regInsideRegion(myRegion, 5, 1));
    printf( "regInsideRegion = %d (0)\n", regInsideRegion(myRegion, 1, 0.5));
    printf( "regInsideRegion = %d (1)\n\n", regInsideRegion(myRegion, 1, 1));
    area = regArea( myRegion, fieldx, fieldy, 1 );
    printf( "Area = %.1f (expected 12.0)\n\n", area );
    regFree(myRegion); 

    /*
     * Standard overlap test of two triangles. The computed area should 
     * be 50
     */
    myRegion = regParse( "poly(0,0,10,10,10,0,0,10)" );
    regPrintRegion( myRegion );
    // regExtent determinds whether or not a shape is contained within
    // the defined field bounds. xpos and ypos will contain the point 
    // values for minimally containing fields
    within_field = regExtent( myRegion, fieldx, fieldy, xpos, ypos );
    printf( "shape in field=%d (expected 1)\n", within_field );
    printf( "bounded by (%.0f,%.0f) and (%.0f,%.0f) (expected (0,0) (10,10)\n", 
            xpos[0], ypos[0], xpos[1], ypos[1] );

    area = regArea( myRegion, fieldx, fieldy, 1 );
    printf( "Area = %.1f (expected 71.0)\n\n", area );

    regFree( myRegion );
    
    /*
     * Adding the point of intersection to make sure it doesn't make 
     * a difference in the calculation.
     */
    myRegion = regParse( "poly(0,0,5,5,10,10,10,0,5,5,0,10,0,0)" );
    regPrintRegion( myRegion );

    area = regArea( myRegion, fieldx, fieldy, 1 );
    printf( "Area = %.1f (expected 71.0)\n\n", area );

    regFree( myRegion );

    /*
     * Force a pixellated computation at a small bin size and we come up
     * with a more "correct" value.
     */
    myRegion = regParse( "poly(0,0,10,10,10,0,0,10)*rect(-10,-10,20,20)" );
    regPrintRegion( myRegion );

    printMask(myRegion);
    area = regArea( myRegion, fieldx, fieldy, .1 );
    printf( "Area = %.1f (expected 51.8)\n\n", area );

    regFree(myRegion);


    /*
     * Complex polygon with many intersections. Includes test for inside.
     */
    myRegion = regParse( "poly(0,0,5,0,5,6,2,6,2,4,9,4,9,9,0,9)" );
    regPrintRegion( myRegion );

    printf( "regInsideRegion = %d (1)\n", regInsideRegion(myRegion, 1, 1));
    printf( "regInsideRegion = %d (1)\n", regInsideRegion(myRegion, 4, 5));
    printf( "regInsideRegion = %d (1)\n", regInsideRegion(myRegion, 5, 8.99));
    printf( "regInsideRegion = %d (0)\n", regInsideRegion(myRegion, 10, 9));

    printMask(myRegion);
    area = regArea( myRegion, fieldx, fieldy, .1 );
    printf( "Area = %.1f (expected 84.0)\n\n", area );


    /*
     * Verify invert
     */
    myRegion = regInvert(myRegion);
    regPrintRegion(myRegion);
    printMask(myRegion);
    area = regArea( myRegion, fieldx, fieldy, .1 );
    printf( "Area = %.1f (expected 16.0)\n\n", area );
    
    regFree(myRegion);

    return 0;
}
