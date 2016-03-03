 /*_C_INSERT_SAO_COPYRIGHT_HERE_(2007)_*/
 /*_C_INSERT_GPL_LICENSE_HERE_*/

#include "cxcregion.h"
#include "region_priv.h"
#include "regtest_utils.c"

void check_extent(regRegion *, double *, double *);

int main(int argc, char* argv[]) {

    regRegion *myRegion;
    double fieldx[2], fieldy[2];

    printf(">>> null region check:\n");
    regPrintRegion(myRegion);
    fieldx[0] = 0; fieldy[0] = 0;
    fieldx[1] = 1; fieldy[1] = 1;
    check_extent(myRegion, fieldx, fieldy);


    printf(">>> Check field extent:\n");
    fieldx[0] = 0; fieldy[0] = 0;
    fieldx[1] = 10; fieldy[1] = 10;
    myRegion = regParse("field()");
    regPrintRegion(myRegion);
    check_extent(myRegion, fieldx, fieldy); 

   
    printf(">>> Check overlapping shapes:\n");
    myRegion = regParse("circle(5,5,2)&rect(3,3,5,5)");
    regPrintRegion(myRegion);
    check_extent(myRegion, fieldx, fieldy); 


    printf(">>> Check multiple components:\n");
    myRegion = regParse("circle(3,3,2)|poly(7,7,7,8,8,8,8,7,7,7)");
    regPrintRegion(myRegion);
    check_extent(myRegion, fieldx, fieldy); 


    printf(">>> Check trim field bounds\n");
    myRegion = regParse("rect(1,1,8,8)");
    regPrintRegion(myRegion);
    fieldx[0] = 2; fieldy[0] = 2;
    fieldx[1] = 10; fieldy[1] = 10;
    check_extent(myRegion, fieldx, fieldy); 


    printf(">>> Check trim both field bounds\n");
    myRegion = regParse("rect(1,1,8,8)");
    regPrintRegion(myRegion);
    fieldx[0] = 2; fieldy[0] = 2;
    fieldx[1] = 10; fieldy[1] = 7;
    check_extent(myRegion, fieldx, fieldy); 
    
    
    printf(">>> Check field bounds off region negative \n");
    myRegion = regParse("rect(1,1,8,8)");
    regPrintRegion(myRegion);
    fieldx[0] = -5; fieldy[0] = 0;
    fieldx[1] = 2; fieldy[1] = 10;
    check_extent(myRegion, fieldx, fieldy); 
    
    
    printf(">>> Check field bounds off region positive \n");
    myRegion = regParse("rect(1,1,8,8)");
    regPrintRegion(myRegion);
    fieldx[0] = 10; fieldy[0] = 0;
    fieldx[1] = 15; fieldy[1] = 10;
    check_extent(myRegion, fieldx, fieldy); 


    printf(">>> Check field bounds off region all directions\n");
    myRegion = regParse("rect(1,1,8,8)");
    regPrintRegion(myRegion);
    fieldx[0] = 10; fieldy[0] = 10;
    fieldx[1] = 15; fieldy[1] = 15;
    check_extent(myRegion, fieldx, fieldy); 


    printf(">>> Test for Bug 12013\n");
    myRegion = regParse("circle(-46.625,149.25,3.5)");
    regPrintRegion(myRegion);
    fieldx[0] = 0, fieldy[0];
    fieldx[1] = 1024, fieldy[1] = 1024;
    check_extent(myRegion, fieldx, fieldy);

    return 0;
}

void check_extent(regRegion *myRegion, double *fieldx, double *fieldy) {
    double xx[2], yy[2];
    regExtent(myRegion, fieldx, fieldy, xx, yy);
    printf("field bounds = (%.1f, %.1f) (%.1f, %.1f)\n", fieldx[0], fieldy[0], fieldx[1], fieldy[1]);
    printf("region bounds = (%.1f, %.1f) (%.1f, %.1f)\n", xx[0], yy[0], xx[1], yy[1]);
    printf("\n\n");
}
