 /*_C_INSERT_SAO_COPYRIGHT_HERE_(2007)_*/
 /*_C_INSERT_GPL_LICENSE_HERE_*/

 /*
  * Simple coverage test for creating regions with a single shape.
  * Verification of regArea, regExtent, regInsideRegion, regCreateShape, 
  * regCreateMask, and regFree for all shapes defined in regGeometry.
  */

#include "cxcregion.h"
#include "region_priv.h"
#include "regtest_utils.c"

int regVerifyExtent(void);
int regVerifyCornerCases();
void check_extent(regRegion * region);
void printStatus(char*, char*, short);
void printMask(regRegion*);

int main( int argc, char* argv[] )
{
    regRegion *myRegion;
    char* name;

    double xx[4]; // x coordinates
    double yy[4]; // y coordinates
    
    double rr[2]; // radii
    double aa[2]; // rotation

    double area;  // area of region shape
    double fieldx[2];  // region field x
    double fieldy[2];  // region field y
    double bin;

    double xpos[2];
    double ypos[2];

    short status;


    fieldx[0] = 0;
    fieldx[1] = 10;

    fieldy[0] = 0;
    fieldy[1] = 10;

    bin = 1;


    // Test basic shape creation and in/out verification for the following:
    // regPOINT, regBOX, regROTBOX (is regBOX), regRECTANGLE, 
    // regROTRECTANGLE (is regRECTANGLE), regCIRCLE, regELLIPSE, regPIE,
    // regSECTOR, regPOLYGON, regANNULUS, regFIELD
    
    // create region
    myRegion = regCreateRegion(NULL, NULL);

    /* 
     * regPOINT verification
     */
    xx[0] = 1;
    yy[0] = 1;
    name = "regPOINT";
    regCreateShape( myRegion, regAND, regPOINT, regInclude, xx, yy, 1, NULL, NULL, 0, 0 );
    regPrintRegion( myRegion );
    printMask( myRegion );

    status = regInsideRegion( myRegion, 1, 1);
    printStatus(name, "yes", status);

    status = regInsideRegion( myRegion, 2, 2);
    printStatus(name, "no", status);    

    area = regArea( myRegion, fieldx, fieldy, bin);
    printf("Geometric Area: %lf\n", area);

    area = regComputePixellatedArea( myRegion, fieldx, fieldy, bin);
    printf("Pixellated Area: %lf\n", area);

    regExtent( myRegion, fieldx, fieldy, xpos, ypos);
    printf("Extent: (%lf, %lf), (%lf, %lf)\n", xpos[0], ypos[0], xpos[1], ypos[1]);
    
    regFree( myRegion );
    myRegion = regCreateRegion(NULL, NULL); 
   
    /*
     * regBOX verification
     */
    name = "regBOX";
    xx[0] = 3;
    yy[0] = 3;
    rr[0] = 3;
    rr[1] = 2;
    aa[0] = -45;
    
    regCreateShape( myRegion, regAND, regBOX, regInclude, xx, yy, 1, rr, aa, 0, 0 );
    regPrintRegion( myRegion );
    printMask( myRegion );

    status = regInsideRegion( myRegion, 4, 3 );
    printStatus(name, "yes", status);

    status = regInsideRegion( myRegion, 0, 0 );
    printStatus(name, "no", status);

    area = regArea( myRegion, fieldx, fieldy, bin);
    printf("Geometric Area: %lf\n", area);

    area = regComputePixellatedArea( myRegion, fieldx, fieldy, bin);
    printf("Pixellated Area: %lf\n", area);

    regExtent( myRegion, fieldx, fieldy, xpos, ypos);
    printf("Extent: (%lf, %lf), (%lf, %lf)\n", xpos[0], ypos[0], xpos[1], ypos[1]);

    regFree( myRegion );
    myRegion = regCreateRegion(NULL, NULL); 
    
    /*
     * regRECTANGLE verification
     */
    name = "regRECTANGLE";
    xx[0] = 2; yy[0] = 2;
    xx[1] = 4; yy[1] = 5;
    rr[0] = 0; rr[1] = 0;
    aa[0] = 0;

    regCreateShape( myRegion, regAND, regRECTANGLE, regInclude, xx, yy, 2, rr, aa, 0, 0 );
    regPrintRegion( myRegion);
    printMask( myRegion );

    status = regInsideRegion( myRegion, 3, 3 );
    printStatus( name, "yes", status );

    status = regInsideRegion( myRegion, 5, 6 );
    printStatus( name, "no", status );

    area = regArea( myRegion, fieldx, fieldy, bin);
    printf("Geometric Area: %lf\n", area);

    area = regComputePixellatedArea( myRegion, fieldx, fieldy, bin);
    printf("Pixellated Area: %lf\n", area);

    regExtent( myRegion, fieldx, fieldy, xpos, ypos);
    printf("Extent: (%lf, %lf), (%lf, %lf)\n", xpos[0], ypos[0], xpos[1], ypos[1]);
    
    regFree( myRegion );
    myRegion = regCreateRegion(NULL, NULL);  

    /*
     * regCIRCLE verification
     */
    name = "regCIRCLE";
    xx[0] = 4; yy[0] = 4;
    rr[0] = 2;
    
    regCreateShape( myRegion, regAND, regCIRCLE, regInclude, xx, yy, 1, rr, aa, 0, 0 );
    regPrintRegion( myRegion );
    printMask( myRegion );

    status = regInsideRegion( myRegion, 3, 3 );
    printStatus( name, "yes", status );
    
    status = regInsideRegion( myRegion, 5, 6 );
    printStatus( name, "no", status );

    area = regArea( myRegion, fieldx, fieldy, bin);
    printf("Geometric Area: %lf\n", area);

    area = regComputePixellatedArea( myRegion, fieldx, fieldy, bin);
    printf("Pixellated Area: %lf\n", area);

    regExtent( myRegion, fieldx, fieldy, xpos, ypos);
    printf("Extent: (%lf, %lf), (%lf, %lf)\n", xpos[0], ypos[0], xpos[1], ypos[1]);
    
    regFree( myRegion );
    myRegion = regCreateRegion(NULL, NULL);

    /*
     * regELLIPSE verification
     */
    name = "regELLIPSE";
    xx[0] = 4; yy[0] = 3;
    rr[0] = 2; rr[1] = 3;
    aa[0] = -45;

    regCreateShape( myRegion, regAND, regELLIPSE, regInclude, xx, yy, 1, rr, aa, 0, 0 );
    regPrintRegion( myRegion );
    printMask( myRegion );
    
    status = regInsideRegion( myRegion, 3, 2 );
    printStatus( name, "yes", status );

    status = regInsideRegion( myRegion, 6, 4 );
    printStatus( name, "no", status );
    
    area = regArea( myRegion, fieldx, fieldy, bin);
    printf("Geometric Area: %lf\n", area);

    area = regComputePixellatedArea( myRegion, fieldx, fieldy, bin);
    printf("Pixellated Area: %lf\n", area);

    regExtent( myRegion, fieldx, fieldy, xpos, ypos);
    printf("Extent: (%lf, %lf), (%lf, %lf)\n", xpos[0], ypos[0], xpos[1], ypos[1]); 

    regFree( myRegion );
    myRegion = regCreateRegion(NULL, NULL);

    /*
     * regPIE verification
     */
    name = "regPIE";
    xx[0] = 0; yy[0] = 0;
    aa[0] = 30; aa[1] = 60; 
    rr[0] = 2; rr[1] = 5;

    regCreateShape( myRegion, regAND, regPIE, regInclude, xx, yy, 1, rr, aa, 0, 0 );
    regPrintRegion( myRegion );
    printMask( myRegion );

    status = regInsideRegion( myRegion, 2, 2 );
    printStatus( name, "yes", status );

    status = regInsideRegion( myRegion, 1, 1 );
    printStatus( name, "no", status );


    area = regArea( myRegion, fieldx, fieldy, bin);
    printf("Geometric Area: %lf\n", area);

    area = regComputePixellatedArea( myRegion, fieldx, fieldy, bin);
    printf("Pixellated Area: %lf\n", area);

    regExtent( myRegion, fieldx, fieldy, xpos, ypos);
    printf("Extent: (%lf, %lf), (%lf, %lf)\n", xpos[0], ypos[0], xpos[1], ypos[1]);
    
    regFree( myRegion );
    myRegion = regCreateRegion(NULL, NULL);

    /*
     * regSECTOR verification
     */
    name = "regSECTOR";
    xx[0] = 0; yy[0] = 0;
    aa[0] = 30; aa[1] = 60; 

    regCreateShape( myRegion, regAND, regSECTOR, regInclude, xx, yy, 1, rr, aa, 0, 0 );
    regPrintRegion( myRegion );
    printMask( myRegion );

    status = regInsideRegion( myRegion, 6, 6 );
    printStatus( name, "yes", status );

    status = regInsideRegion( myRegion, 3, 6 );
    printStatus( name, "no", status );


    area = regArea( myRegion, fieldx, fieldy, bin);
    printf("Geometric Area: %lf\n", area);

    area = regComputePixellatedArea( myRegion, fieldx, fieldy, bin);
    printf("Pixellated Area: %lf\n", area);

    regExtent( myRegion, fieldx, fieldy, xpos, ypos);
    printf("Extent: (%lf, %lf), (%lf, %lf)\n", xpos[0], ypos[0], xpos[1], ypos[1]);
    
    regFree( myRegion );
    myRegion = regCreateRegion(NULL, NULL);

    /*
     * regANNULUS verification
     */
    name = "regANNULUS";
    xx[0] = 3; yy[0] = 3;
    rr[0] = 1; rr[1] = 3;

    regCreateShape( myRegion, regAND, regANNULUS, regInclude, xx, yy, 1, rr, aa, 0, 0 );
    regPrintRegion( myRegion ); 
    printMask( myRegion );

    status = regInsideRegion( myRegion, 2, 2 );
    printStatus( name, "yes", status );

    status = regInsideRegion( myRegion, 0, 0 );
    printStatus( name, "no", status );


    area = regArea( myRegion, fieldx, fieldy, bin);
    printf("Geometric Area: %lf\n", area);

    area = regComputePixellatedArea( myRegion, fieldx, fieldy, bin);
    printf("Pixellated Area: %lf\n", area);

    regExtent( myRegion, fieldx, fieldy, xpos, ypos);
    printf("Extent: (%lf, %lf), (%lf, %lf)\n", xpos[0], ypos[0], xpos[1], ypos[1]);
    
    regFree( myRegion );
    myRegion = regCreateRegion(NULL, NULL);
    
    /*
     * regFIELD verification
     */
    name = "regFIELD";
    xx[0] = 3; yy[0] = 3;
    rr[0] = 1; rr[1] = 3;

    regCreateShape( myRegion, regAND, regFIELD, regInclude, xx, yy, 1, rr, aa, 0, 0 );
    regPrintRegion( myRegion ); 
    printMask( myRegion );

    status = regInsideRegion( myRegion, 2, 2 );
    printStatus( name, "yes", status );


    area = regArea( myRegion, fieldx, fieldy, bin);
    printf("Geometric Area: %lf\n", area);

    area = regComputePixellatedArea( myRegion, fieldx, fieldy, bin);
    printf("Pixellated Area: %lf\n", area);

    regExtent( myRegion, fieldx, fieldy, xpos, ypos);
    printf("Extent: (%lf, %lf), (%lf, %lf)\n", xpos[0], ypos[0], xpos[1], ypos[1]);
    
    regFree( myRegion );
    myRegion = regCreateRegion(NULL, NULL);
   
    /*
     * regPOLYGON verification
     */
    name = "regPOLYGON";  
    xx[0] = 1; yy[0] = 3;
    xx[1] = 4; yy[1] = 1;
    xx[2] = 4; yy[2] = 5;
    xx[3] = 1; yy[3] = 3;

    regCreateShape( myRegion, regAND, regPOLYGON, regInclude, xx, yy, 4, NULL, NULL, 0, 0 );
    regPrintRegion( myRegion ); 
    printMask( myRegion );

    status = regInsideRegion( myRegion, 3, 3 );
    printStatus( name, "yes", status );

    status = regInsideRegion( myRegion, 2, 2 );
    printStatus( name, "no", status );


    area = regArea( myRegion, fieldx, fieldy, bin);
    printf("Geometric Area: %lf\n", area);

    area = regComputePixellatedArea( myRegion, fieldx, fieldy, bin);
    printf("Pixellated Area: %lf\n", area);

    regExtent( myRegion, fieldx, fieldy, xpos, ypos);
    printf("Extent: (%lf, %lf), (%lf, %lf)\n", xpos[0], ypos[0], xpos[1], ypos[1]);
    
    regFree( myRegion );


    regVerifyExtent();
    regVerifyCornerCases();
    
    return 0;
}


int regVerifyExtent() {
    
    regRegion * region;
    double xx[5];
    double yy[5];
    double rr[2];
    double aa[2];
    long npoints;
    double fieldx[2];
    double fieldy[2];
    double area;
    double pixArea;

    // Empty region test
    region = regCreateEmptyRegion();
    regPrintRegion(region);
    check_extent(region); 

    fieldx[0] = 0; fieldy[0] = 0;
    fieldx[1] = 20; fieldy[1] = 20;
    area = regArea(region, fieldx, fieldy, 1);
    printf("Area = %f  (expected=0)\n", area);
    pixArea = regComputePixellatedArea(region, fieldx, fieldy, 1);
    printf("Pixellated area = %f  (expected=0)\n\n", pixArea); 

    // Add a polygon
    printf("\n>>> Simple polygon area check:\n");
    xx[0] = 5;  yy[0] = 5;
    xx[1] = 10; yy[1] = 5;
    xx[2] = 10; yy[2] = 10;
    xx[3] = 5;  yy[3] = 10;
    xx[4] = 5;  yy[4] = 5;
    npoints = 5; 
    regCreateShape(region, regAND, regPOLYGON, regInclude, xx, yy, npoints, rr, aa, 0, 0);
    regPrintRegion(region);
    check_extent(region); 

    fieldx[0] = 0; fieldy[0] = 0;
    fieldx[1] = 20; fieldy[1] = 20;
    area = regArea(region, fieldx, fieldy, 1);
    printf("Area of polygon = %f  (expected=25)\n", area);
    pixArea = regComputePixellatedArea(region, fieldx, fieldy, 1);
    printf("pixellated area of polygon = %f  (expected=36)\n\n", pixArea); 


    // Add a circle of radius 2, overlaps polygon with 1 quarter. Thus counts pixels to 
    // determine area, should end up with ~34.42/4
    printf("\n>>> Polygon and a circle overlap check:\n");
    xx[0] = 5; yy[0] = 5;
    rr[0] = 2;
    npoints = 1;
    regCreateShape(region, regAND, regCIRCLE, regInclude, xx, yy, npoints, rr, aa, 0, 0);
    regPrintRegion(region); 
    check_extent(region); 

    area = regArea(region, fieldx, fieldy, .05);
    printf("Area of polygon and circle = %f  (expected ~= PI)\n", area);
    pixArea = regComputePixellatedArea(region, fieldx, fieldy, 1);
    printf("Pixellated area of polygon and circle = %f  (expected=6)\n\n", pixArea); 

    // Start with inverted box 
    printf("\n>>> Simple inverted 4x6 box check:\n");
    region = regCreateEmptyRegion();
    xx[0] = 5; yy[0] = 5;
    rr[0] = 4; rr[1] = 6;
    aa[0] = 0.0;
    npoints = 1;
    regCreateShape(region, regAND, regBOX, regExclude, xx, yy, npoints, rr, aa, 0, 0);
    regPrintRegion(region); 
    check_extent(region);

    area = regArea(region, fieldx, fieldy, .05);
    printf("Area of !box in field = %f (expected=376)\n", area);
    pixArea = regComputePixellatedArea(region, fieldx, fieldy, 1);
    printf("Pixellated area of !box = %f  (expected=406)\n\n", pixArea); 
    
    // add a circle that doesn't overlap
    printf("\n>>> Non overlapping circle included check:\n");
    xx[0] = 9; yy[0] = 5;
    rr[0] = 1;
    npoints = 1;
    regCreateShape(region, regAND, regCIRCLE, regInclude, xx, yy, npoints, rr, aa, 0, 0);
    regPrintRegion(region); 
    check_extent(region); 

    area = regArea(region, fieldx, fieldy, .05);
    printf("area of !box and circle = %f (expected=PI)\n", area);
    pixArea = regComputePixellatedArea(region, fieldx, fieldy, 1);
    printf("Pixellated area of !box and circle = %f  (expected=5)\n\n", pixArea); 

    // Or a circle on the other side, will force computation by pixellation
    printf("\n>>> Additional overlapping circle included check:\n");
    xx[0] = 3; yy[0] = 2;
    rr[0] = 1;
    npoints = 1;
    regCreateShape(region, regOR, regCIRCLE, regInclude, xx, yy, npoints, rr, aa, 0, 0);
    regPrintRegion(region); 
    check_extent(region); 

    area = regArea(region, fieldx, fieldy, .05);
    printf("Area of region = %f (expected~=2PI)\n", area);
    pixArea = regComputePixellatedArea(region, fieldx, fieldy, 1);
    printf("Pixellated area of region = %f  (expected=10)\n\n", pixArea); 

    // Complex polygon
    region = regCreateEmptyRegion();
    printf("\n>>> Complex polygon check:\n");
    xx[0] = 5;  yy[0] = 5;
    xx[1] = 10; yy[1] = 10;
    xx[2] = 10; yy[2] = 5;
    xx[3] = 5;  yy[3] = 10;
    xx[4] = 5;  yy[4] = 5;
    npoints = 5; 
    regCreateShape(region, regAND, regPOLYGON, regInclude, xx, yy, npoints, rr, aa, 0, 0);
    regPrintRegion(region);
    check_extent(region); 
    
    area = regArea(region, fieldx, fieldy, 1);
    printf("Area of polygon = %f  (expected=24)\n", area);
    pixArea = regComputePixellatedArea(region, fieldx, fieldy, 1);
    printf("pixellated area of polygon = %f  (expected=24)\n\n", pixArea); 

    regFree(region);

    return 0;
}

void check_extent(regRegion * region) {
    double extentx[2];
    double extenty[2];
    double fieldx[2];
    double fieldy[2];

    fieldx[0] = 0; fieldy[0] = 0;
    fieldx[1] = 20; fieldy[1] = 20;

    regExtent(region, fieldx, fieldy, extentx, extenty);
    printf("Extent = (%f, %f) (%f, %f) \n", extentx[0], extenty[0], extentx[1], extenty[1]);
}

/*
 * Corner cases for regions with 0 width.
 */
int regVerifyCornerCases() {
    regRegion *myRegion;
    double area;
    double fieldx[2];
    double fieldy[2];

    fieldx[0] = -100; fieldy[0] = -100;
    fieldx[1] = 100; fieldy[1] = 100;
    
    printf(">>> Testing corner cases\n");

    myRegion = regParse("annulus(0,0,55,55)");
    regPrintRegion(myRegion);
    area = regArea(myRegion, fieldx, fieldy, 1);
    printf("region area = %lf\n", area);
    regFree(myRegion);

    myRegion = regParse("ellipse(0,0,0,55,0)");
    regPrintRegion(myRegion);
    area = regArea(myRegion, fieldx, fieldy, 1);
    printf("region area = %lf\n", area);
    regFree(myRegion);

    myRegion = regParse("circle(0,0,0)");
    regPrintRegion(myRegion);
    area = regArea(myRegion, fieldx, fieldy, 1);
    printf("region area = %lf\n", area);
    regFree(myRegion);

    myRegion = regParse("circle(0,0,0)|box(5,5,3,3)");
    regPrintRegion(myRegion);
    area = regArea(myRegion, fieldx, fieldy, 1);
    printf("region area = %lf\n", area);
    regFree(myRegion);

    myRegion = regParse("field()");
    regPrintRegion(myRegion);
    area = regArea(myRegion, fieldx, fieldy, 1);
    printf("region area = %lf\n", area);
    regFree(myRegion);

    myRegion = regParse("circle(0,0,0)|field()");
    regPrintRegion(myRegion);
    area = regArea(myRegion, fieldx, fieldy, 1);
    printf("region area = %lf\n", area);
    regFree(myRegion);
    
    return 0;
}

