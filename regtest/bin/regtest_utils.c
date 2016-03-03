
 /*_C_INSERT_SAO_COPYRIGHT_HERE_(2007)_*/
 /*_C_INSERT_GPL_LICENSE_HERE_*/

 /* 
  * Collection of basic utility functions shared between
  * reglib tests.
  */

void printStatus(char*, char*, short);
void printMask(regRegion*);

void printStatus( char* name, char* expected, short status ) {
    printf( "Is point inside?  %s = %s (expected=%s)\n", name, status ? "yes" : "no", expected );
}

void printMask( regRegion* myRegion ) {
    
    short *mask; long xlen; long ylen; short ii; short jj;

    regRegionToMask( myRegion, 0, 9, 0, 9, 1, &mask, &xlen, &ylen );
    for (ii=ylen-1; ii>=0; ii--) 
    {
        for (jj=0; jj<xlen; jj++)
        {
            printf( "%c ", mask[jj+ii*xlen] == 0 ? 'O' : 'X' );
        }
        printf("\n");
    }        

    if (mask) free (mask); 

}
