# These are variables. We usually define CC, CCLINK, CFLAGS, and OBJS
CC=gcc
CCLINK=$(CC) # Read variable CC using $(CC)
CFLAGS=-g -Wall -std=c99
OBJS=grades.o libgrades.so 
EXEC=prog.exe
RM=rm -rf *.o *.exe libgrades.so

#First rule that will compile all - default when running the file.
all: $(OBJS)
	$(CCLINK) $(OBJS) -o $(EXEC)
	$(CCLINK) grades.o -o $(EXEC) -lgrades -L
	$(CCLINK) grades.o -o $(EXEC) -llinked-list -L

libgrades.so: grades.o
	$(CCLINK) -shared grades.o -o libgrades.so

grades.o: grades.c grades.h linked-list.h
	$(CC) $(CFLAGS) -fpic grades.c

#Clean operation to delete all automatic generated files.
clean:
	$(RM)