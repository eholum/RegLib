/*_C_INSERT_SAO_COPYRIGHT_HERE_(2007)_*/
 /*_C_INSERT_GPL_LICENSE_HERE_*/

#include "cxcregion.h"
#include "region_priv.h"
#include "regtest_utils.c" 

void verifyShape(regShape*, int, int, long, long, long, long, long);

int main( int argc, char* argv[] )
{
    regRegion *myRegion;

    // regPOINT, regBOX, regROTBOX (is regBOX), regRECTANGLE, 
    // regROTRECTANGLE (is regRECTANGLE), regCIRCLE, regELLIPSE, regPIE,
    // regSECTOR, regPOLYGON, regANNULUS, regFIELD
   
    // Each shape is designed so that (5,5) is inside and (0,0) is outside. 
    // (Except for the field, of course)
    myRegion = regParse( "point(5,5)" );
    verifyShape( regGetShapeNo( myRegion, 0 ), 0, 1, 0 ,0, 1, 1, 0 );
    regFree( myRegion );
    
    myRegion = regParse( "box(5,5,3,2)" );
    verifyShape( regGetShapeNo( myRegion, 0 ), 0, 1, 1, 0, 1, 1, 2 );
    regFree( myRegion );

    myRegion = regParse( "rect(2,2,6,6)" );
    verifyShape( regGetShapeNo( myRegion, 0 ), 0, 1, 1, 0, 1, 2, 0 );
    regFree( myRegion );
    
    myRegion = regParse( "circle(4,4,2)" );
    verifyShape( regGetShapeNo( myRegion, 0 ), 0, 1, 0, 0, 1, 1, 1 );
    regFree( myRegion );
    
    myRegion = regParse( "ellipse(5,5,2,3,35)" );
    verifyShape( regGetShapeNo( myRegion, 0 ), 0, 1, 1, 0, 1, 1, 2 );
    regFree( myRegion );
        
    myRegion = regParse( "pie(1,1,3,6,25,75)" );
    verifyShape( regGetShapeNo( myRegion, 0 ), 0, 1, 2, 0, 1, 1, 2 );
    regFree( myRegion );

    myRegion = regParse( "sector(1,1,35,75)" );
    verifyShape( regGetShapeNo( myRegion, 0 ), 0, 1, 2, 0, 1, 1, 0 );
    regFree( myRegion );

    myRegion = regParse( "annulus(7,7,1,4)" );
    verifyShape( regGetShapeNo( myRegion, 0 ), 0, 1, 0, 0, 1, 1, 2 );
    regFree( myRegion );

    myRegion = regParse( "polygon(1,1,5,5,10,9,10,0)" );
    verifyShape( regGetShapeNo( myRegion, 0 ), 0, 1, 0, 0, 1, 5, 0 );
    regFree( myRegion );

    myRegion = regParse( "field()" );
    verifyShape( regGetShapeNo( myRegion, 0 ), 1, 1, 0, 0, 1, 0, 0 );

    /*
     * Including a regResolveField test for the field shape.
     *
     */
    double x[2], y[2];
    x[0] = 1; y[0] = 1;
    x[1] = 5; y[1] = 5;
    regResolveField( myRegion, x, y );
    regPrintRegion( myRegion );
    printMask( myRegion );
    printStatus( "region", "yes", regInsideRegion( myRegion, 2, 2 ));
    printStatus( "region", "no", regInsideRegion( myRegion, 0, 0 ));

    regFree( myRegion );
    
    return 0;
}

void verifyShape(regShape *myShape,
                 int in1, // (0,0) inside
                 int in2, // (5,5) inside
                 long angles, // how many angles defined
                 long angle, // value of first angle
                 long component, // included or excluded (this really shouldn't be a long)
                 long numPoints, // Number of points in the shape
                 long radii) // Number of radii in the shape
{
    regPrintShape( myShape );

    char name[12];
    regShapeGetName( myShape, &name[0], 12 );
    printf("regShapeGetName=%s\n", name);

    printf("regInsideShape=%d (expected=%d)\n", regInsideShape( myShape, 0.0, 0.0 ), in1);
    printf("regInsideShape=%d (expected=%d)\n", regInsideShape( myShape, 5.0, 5.0 ), in2);
    printf("regShapeAngles=%lu (expected=%lu)\n", reg_shape_angles( myShape ), angles);
    printf("regShapeGetAngles=%lu (expected=%lu)\n", regShapeGetAngles( myShape, 0), angle); 
    printf("regShapeGetComponent=%lu (expected=%lu)\n", regShapeGetComponent( myShape ), component);
    printf("regShapeGetNoPoints=%lu (expected=%lu)\n", regShapeGetNoPoints( myShape ), numPoints);
    printf("regShapeRadii=%lu (expected=%lu)\n\n", reg_shape_radii( myShape ), radii);
}
