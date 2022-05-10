

OBJECTS= gswiss.o listing.o pairing.o sorting.o basiclistoperations.o
TARGET= gswiss
CC= gcc
CFLAGS= -Wall
CONFFLAG=
-include config
srcdir= src

all: $(OBJECTS)
	$(CC) $(CFLAGS) $(CONFFLAG) -o bin/$(TARGET) $(addprefix bin/,$(OBJECTS))
gswiss.o : $(srcdir)/gswiss.c
	$(CC) $(CFLAGS) $(CONFFLAG) -c $< -o bin/$@
listing.o : $(srcdir)/listing.c
	$(CC) $(CFLAGS) $(CONFFLAG) -c $< -o bin/$@
pairing.o : $(srcdir)/pairing.c
	$(CC) $(CFLAGS) $(CONFFLAG) -c $< -o bin/$@
sorting.o : $(srcdir)/sorting.c
	$(CC) $(CFLAGS) $(CONFFLAG) -c $< -o bin/$@
basiclistoperations.o : $(srcdir)/basiclistoperations.c
	$(CC) $(CFLAGS) $(CONFFLAG) -c $< -o bin/$@
clean:
	rm bin/*.o
