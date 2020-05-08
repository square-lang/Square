# If user is under Windows
ifeq (Windows_NT,$(OS))
# Use '-d' will create the "parse.tab.h"
# Use '-y' will create the "parse.output"
YACC = win_bison -d -y
TARGET = bin/square
CC = gcc

all : $(TARGET)

.PHONY : all

# Compile to exe file in windows
TARGET:=$(TARGET).exe

src/parse.tab.c : src/parse.y
	$(YACC) src/parse.y

$(TARGET) : src/main.c src/node.c src/parse.tab.c
	mkdir -p "$$(dirname $(TARGET))"
	$(CC) src/main.c src/node.c src/parse.tab.c

clean :
	# clean the exe file
	rm -f src/*.exe
	# clean the .output file
	rm -f src/*/output

.PHONY : clean
endif

# In Linux or Mac:
YACC = bison -y -d
TARGET = bin/square
CC = gcc
CFLAGS = -g -Wall

all : $(TARGET)

.PHONY : all

src/parse.tab.c : src/parse.y
	$(YACC) -o src/parse.tab.c src/parse.y

src/main.o : src/main.c
	$(CC) -g -c src/main.c -o src/main.o

src/node.o : src/node.c
	$(CC) -g -c src/node.c -o src/node.o

src/parse.tab.o : src/parse.tab.c
	$(CC) -g -c src/parse.tab.c -o src/parse.tab.o

$(TARGET) : src/main.o src/node.o src/parse.tab.o
	mkdir -p "$$(dirname $(TARGET))"
	$(CC) $(CFLAGS) src/main.o src/node.o src/parse.tab.o -o $(TARGET)

clean:
	# Clean the out file
	rm -f src/*.o $(TARGET)
	# Clean the output file
	rm -f src/*.output