 /*_C_INSERT_SAO_COPYRIGHT_HERE_(2007)_*/
 /*_C_INSERT_GPL_LICENSE_HERE_*/

 /* 
  * Creates a region and appends three shapes - circle, polygon, rectangle. 
  * After each shape append, it queries the region for regGetMaxPoints, 
  * regGetNoShapes, regGetShapeNo.  This also tests regAppendShape, 
  * regPrintRegion, regCreateEmptyRegion, regCompareShape, regCopyShape, 
  * regInvert.
  */

#include "cxcregion.h"
#include "region_priv.h"
#include "regtest_utils.c"

void printStatus(char*, char*, short);
void printMask(regRegion*);
int verifyCreateShape(void) ;
void regVerifyShape( regShape * );


int main( int argc, char* argv[] )
{

    regRegion *myRegion;
    regRegion *invRegion;
    regShape* myShape ;
    regShape* myShape2 ;


    int MAXLEN = 10;
    char name[MAXLEN];

    double xx[4]; // x coordinates
    double yy[4]; // y coordinates
    
    double rr[2]; // radii
    double aa[2]; // rotation

    short status;
    long maxpts;
    long nshapes;

    // create region
    printf(">> Create an empty region\n");
    myRegion = regCreateEmptyRegion();

    xx[0] = 5; yy[0] = 5;
    rr[0] = 2;

    regPrintRegion( myRegion);
    printMask( myRegion);

    maxpts = regGetMaxPoints( myRegion );
    printf("Max Points:  %li\n", maxpts);

    nshapes = regGetNoShapes( myRegion );
    printf("Number of Shapes:  %li\n", nshapes);


    printf("\n\n>> Append a CIRCLE to region\n");

    regAppendShape( myRegion, "CIRCLE", regInclude, regAND, xx, yy, 1, rr, aa, 0, 0);

    regPrintRegion( myRegion);
    printMask( myRegion);

    
    maxpts = regGetMaxPoints( myRegion );
    printf("Max Points:  %li\n", maxpts);

    nshapes = regGetNoShapes( myRegion );
    printf("Number of Shapes:  %li\n", nshapes);

    xx[0] = 6; yy[0] = 7;
    xx[1] = 9; yy[1] = 10;
    xx[2] = 9; yy[2] = 5;
    xx[3] = 6; yy[3] = 7;
    rr[0] = 0; rr[1] = 0;
    aa[0] = 0; aa[1] = 0;


    printf("\n\n>> Append a POLYGON with one vertex:\n");
    regAppendShape( myRegion, "POLYGON", regInclude, regOR, xx, yy, 1, rr, aa, 0, 0 );


    printf("\n\n>> Append a POLYGON with four vertices:\n");
    regAppendShape( myRegion, "POLYGON", regInclude, regOR, xx, yy, 4, rr, aa, 0, 0 );

    regPrintRegion( myRegion);
    printMask( myRegion);

    
    maxpts = regGetMaxPoints( myRegion );
    printf("Max Points:  %li\n", maxpts);

    nshapes = regGetNoShapes( myRegion );
    printf("Number of Shapes:  %li\n", nshapes);


    xx[0] = 1; yy[0] = 1;
    xx[1] = 7; yy[1] = 3;
    rr[0] = 0; rr[1] = 0;
    aa[0] = 0;

    printf("\n\n>> Append a RECTANGLE\n");
    regAppendShape( myRegion, "RECTANGLE", regInclude, regOR, xx, yy, 2, rr, aa, 0, 0 );

    regPrintRegion( myRegion);
    printMask( myRegion);

    
    maxpts = regGetMaxPoints( myRegion );
    printf("Max Points:  %li\n", maxpts);

    nshapes = regGetNoShapes( myRegion );
    printf("Number of Shapes:  %li\n", nshapes);

    
    printf("\n>> Get shape 5 in region\n");
    myShape = regGetShapeNo( myRegion, 5 );  // Needs to throw error message
    if (!myShape)
      printf("Error: Shape 5 does not exist \n");


    printf("\n>> Get shape 2 in region\n");
    myShape = regGetShapeNo( myRegion, 2 );
    regPrintShape(myShape);

    regShapeGetName( myShape, name, MAXLEN);

    status = regInsideShape( myShape, 9, 8 );
    printStatus( name, "yes", status );
    
    status = regInsideShape( myShape, 2, 2 );
    printStatus( name, "no", status );


    printf("\n\n>> Copy shape 3 in region\n");
    myShape2 = regCopyShape( regGetShapeNo(myRegion, 3) );
    regPrintShape(myShape2);

    
    status = regCompareShape( myShape, myShape2, 0 );
    printf("\n>> Are shape 2 and shape 3 equivalent?  %s\n", status ? "yes" : "no");

    myShape2 = regCopyShape( regGetShapeNo(myRegion, 2) );
    status = regCompareShape( myShape, myShape2, 0 );
    printf("\n>> Are shape 2 and shape 2 equivalent?  %s\n", status ? "yes" : "no");


    printf("\n>> Invert region\n");
    invRegion = regInvert( myRegion );
    printMask(invRegion);


    printf("\nFree region \n");
    regFree( myRegion );
    regFree( invRegion );

    printf("\nVerify CreateShape functions \nDone!\n");

    verifyCreateShape();


    return 0;
}


int verifyCreateShape() 
{
    regShape* shape;
    double xpos[5];
    double ypos[5];
    double angle[2];
    double radius[2];

    printf(">>> Null input checks:\n");
    shape = regCreateSector(regInclude, NULL, ypos, angle, 1, 1); 
    regFreeShape(NULL, shape);
    shape = regCreateAnnulus(regInclude, xpos, NULL, radius, 1, 1); 
    regFreeShape(NULL, shape);
    shape = regCreatePolygon(regInclude, NULL, ypos, 4, 1, 1);
    regFreeShape(NULL, shape);
    printf("\n\n");

    // POINT
    printf(">>> Point:\n");

    xpos[0] = 7;
    ypos[0] = 7;
    shape = regCreatePoint(regInclude, xpos, ypos, 1, 1); 
    regVerifyShape( shape );
    regFreeShape(NULL, shape);

    // BOX
    printf(">>> Box:\n");
    
    xpos[0] = 7;
    ypos[0] = 7;
    radius[0] = 1;
    radius[1] = 1;
    angle[0] = 0;
    shape = regCreateBox(regInclude, xpos, ypos, radius, angle, 1, 1); 
    regVerifyShape( shape );
    regFreeShape(NULL, shape);

    // CIRCLE
    printf(">>> Circle:\n");
    
    xpos[0] = 7;
    ypos[0] = 7;
    radius[0] = 1;
    shape = regCreateCircle(regInclude, xpos, ypos, radius, 1, 1); 
    regVerifyShape( shape );
    regFreeShape(NULL, shape);

    // ELLIPSE
    printf(">>> Ellipse:\n");

    xpos[0] = 7;
    ypos[0] = 7;
    radius[0] = 2;
    radius[1] = 2;  
    angle[0] = 0;
    shape = regCreateEllipse(regInclude, xpos, ypos, radius, angle, 1, 1); 
    regVerifyShape( shape );
    regFreeShape(NULL, shape);

    // PIE
    printf(">>> Pie:\n");

    xpos[0] = 6;
    ypos[0] = 6;
    radius[0] = 2;
    radius[1] = 3;
    angle[0] = 0;
    angle[1] = 90;
    shape = regCreatePie(regInclude, xpos, ypos, radius, angle, 1, 1); 
    regVerifyShape( shape );
    regFreeShape(NULL, shape);

    // SECTOR
    printf(">>> Sector:\n");
    
    xpos[0] = 5;
    ypos[0] = 5;
    angle[0] = 0;
    angle[1] = 90;    
    shape = regCreateSector(regInclude, xpos, ypos, angle, 1, 1); 
    regVerifyShape( shape );
    regFreeShape(NULL, shape);


    // ANNULUS
    printf(">>> Annulus Test:\n");
    
    xpos[0] = 5;
    ypos[0] = 5;
    radius[0] = 1;
    radius[1] = 3;
    shape = regCreateAnnulus(regInclude, xpos, ypos, radius, 1, 1); 
    regVerifyShape( shape );
    regFreeShape(NULL, shape);
   

    // POLYGON
    printf(">>> Polygon Test:\n");

    xpos[0] = 5; ypos[0] = 5;
    xpos[1] = 10; ypos[1] = 5;
    xpos[2] = 10; ypos[2] = 10;
    xpos[3] = 5; ypos[3] = 8;
    shape = regCreatePolygon(regInclude, xpos, ypos, 4, 1, 1);
    regVerifyShape( shape );
    regFreeShape(NULL, shape);

    // Weird POLYGON
    printf(">>> Irregular Polygon Test:\n");

    xpos[0] = 5; ypos[0] = 5;
    xpos[1] = 10; ypos[1] = 10;
    xpos[2] = 10; ypos[2] = 5;
    xpos[3] = 5; ypos[3] = 10;
    xpos[4] = 5; ypos[4] = 5;
    shape = regCreatePolygon(regInclude, xpos, ypos, 4, 1, 1);
    regVerifyShape( shape );
    regFreeShape(NULL, shape);
    
    // FIELD
    printf(">>> Field Test:\n");
    shape = regCreateField(regInclude, 1, 1);
    regVerifyShape( shape );
    regFreeShape(NULL, shape);


    // MASK
    printf(">>> Mask Test:\n");
    shape = regCreateMask(regInclude, 1, 1);
    regFreeShape(NULL, shape);

    return 0;
}


void regVerifyShape( regShape * shape ) {
    char buf[1000] = {};
    double area;
    double xext[2] = {0, 0};
    double yext[2] = {0, 0};
    double dou[2] = {100.0, 500.0};    
    regShape* cpy;
    int equal;

    if (shape == NULL)
      return;

    shape->toString(shape, buf, 1000);
    printf("%s \n", buf);
    
    area = shape->calcArea(shape);
    printf("Area=%f\n", area);
    
    printf("regInside (yes)=%d\n", shape->isInside(shape, 7, 7));
    printf("regInside (no)=%d\n", shape->isInside(shape, 1, 1));
    
    shape->calcExtent(shape, xext, yext);
    printf("regExtent (%f, %f) (%f, %f)\n", xext[0], yext[0], xext[1], yext[1]);

    dou[0] = 1; dou[1] = 2;
    cpy = regCreateSector(regInclude, dou, dou, dou, 1, 1);
    equal = shape->isEqual(shape, cpy);
    printf("different shape isEqual (no)=%d\n", equal);
    cpy = shape->copy(shape);
    equal = shape->isEqual(shape, cpy);
    printf("copy isEqual (yes)=%d\n", equal);
    
    // Can't do this for fields
    if (shape->type != regFIELD) {
        cpy->xpos[0]++;
    }
    equal = shape->isEqual(shape, cpy);
    printf("copy isEqual (no)=%d\n", equal);
    
    cpy->include = (cpy->include + 1) % 2;
    equal = shape->isEqual(shape, cpy);
    printf("copy isEqual (no)=%d\n", equal);
     
    printf("\n\n");
}
