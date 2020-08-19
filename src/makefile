CC = gcc
TOP = ..
TARGET = $(TOP)/bin/square

ifeq (Windows_NT, $(OS))
TARGET := $(TARGET).exe
endif

$(TARGET) :
	mkdir -p "$$(dirname $(TARGET))"
	$(CC) main.c parse.tab.c squ_run.c node.c vm_stack.c vm_exec.c -o $(TARGET)