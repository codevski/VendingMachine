CFLAGS = -ansi -Wall -pedantic -g
CC = gcc
SOURCES=ppd_main.c ppd_menu.c ppd_options.c ppd_utility.c ppd_stock.c \
ppd_coin.c
HEADERS=ppd_main.h ppd_menu.h ppd_options.h ppd_utility.h ppd_stock.h \
ppd_coin.h
OBJ = ppd_main.o ppd_menu.o ppd_options.o ppd_utility.o ppd_stock.o \
ppd_coin.o
README=ppd_readme
MAKEFILE=Makefile

all: vm

vm: $(OBJ)
		$(CC) $(CFLAGS) $(OBJ) -o $@

%.o: %.c $(HEADERS)
			$(CC) $(CFLAGS) -c $< 
	
clean:
		rm -f *.o vm

########################################################################
# Move this target to the end of the Makefile to zip up your code 
# when submitting. Do not submit your .dat files, or directories. 
# We only want the files that are part of your implementation.
########################################################################
archive:
	zip $(USER)-a2 $(SOURCES) $(HEADERS) $(README) $(MAKEFILE)
