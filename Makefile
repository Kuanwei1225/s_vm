CROSS_COMPILE		=
AS					=   $(CROSS_COMPILE)as
AR					=   $(CROSS_COMPILE)ar
CC					=   $(CROSS_COMPILE)gcc
CPP					=   $(CC) -E
LD					=   $(CROSS_COMPILE)ld
NM					=   $(CROSS_COMPILE)nm
OBJCOPY				=   $(CROSS_COMPILE)objcopy
OBJDUMP				=   $(CROSS_COMPILE)objdump
RANLIB				=   $(CROSS_COMPILE)ranlib
READELF				=   $(CROSS_COMPILE)readelf
SIZE				=   $(CROSS_COMPILE)size
STRINGS				=   $(CROSS_COMPILE)strings
STRIP				=   $(CROSS_COMPILE)strip

CFLAGS				=	-g
LDFLAGS				=   
OBJS				=   svm
CFILES				=	scc.c vm.c
OUTPUT_PATH=bin


all:
ifeq "$(wildcard $(OUTPUT_PATH))" ""
	mkdir $(OUTPUT_PATH)
endif
	$(CC) $(CFLAGS) -o $(OBJS) $(CFILES) $(LDFLAGS)
	mv -f svm bin

clean:
	$(MAKE) -C svm clean
	rm -rf bin


