MK_TOP = ../../../..
include $(MK_TOP)/include/Makefile.cxc

# Region library source root
REGIONROOT = $(MK_TOP)/lib/region/src

# Regression test script
PERLSCRIPTS     = region_test.t

# Test code install tags.
TEST_SCRIPTS    = $(PERLSCRIPTS)
TEST_EXES	= converttest polytest shapetest logictest \
		shapeopstest allshapestest asciitest extenttest

MAKEALL   +=  $(TEST_SCRIPTS) 
MAKECLEAN +=  $(TEST_SCRIPTS) $(TEST_EXES)

INSTALL_TEST_EXES = $(TEST_EXES:%=$(TST_DIR)/bin/reglib/%)
INSTALL_TEST_SCRIPTS = $(TEST_SCRIPTS:%=$(TST_DIR)/bin/%)

LIBPATH		+= -lregion
LOCAL_INCLUDES  = -I$(REGIONROOT) -I$(MK_TOP)/include
LOCAL_LIBS      = 
INCLUDES        = -I.  $(LOCAL_INCLUDES) $(LOCAL_CFLAGS)
LIBS            =  $(LOCAL_LIBS)

include $(MK_TOP)/Makefile.all



converttest:  reg_convertcoord_test.o
	$(CC) $(LFLAGS) reg_convertcoord_test.o ${LIBPATH} -o $(@) $(LDFLAGS)

polytest:  polygon_area_test.o
	$(CC) $(LFLAGS) polygon_area_test.o ${LIBPATH} -o $(@)

shapetest:  reg_shape_test.o
	$(CC) $(LFLAGS) reg_shape_test.o ${LIBPATH} -o $(@)

logictest:  reg_logic_test.o
	$(CC) $(LFLAGS)  reg_logic_test.o ${LIBPATH} -o $(@)

shapeopstest:  reg_shape_ops_test.o
	$(CC) $(LFLAGS) reg_shape_ops_test.o ${LIBPATH} -o $(@)

allshapestest:  reg_all_shapes_test.o
	$(CC) $(LFLAGS) reg_all_shapes_test.o ${LIBPATH} -o $(@)

asciitest:  reg_ascii_test.o
	$(CC) $(LFLAGS) reg_ascii_test.o ${LIBPATH} -o $(@)

extenttest: reg_extent_test.o
	$(CC) $(LFLAGS) reg_extent_test.o ${LIBPATH} -o $(@)

