CC = gcc
CFLAGS = -Wall -Wextra -MMD
LDFLAGS =
PACKAGE = pkg-config --cflags --libs gtk4
LDLIBS = $(shell pkg-config --libs gtk4)
EXE = Gui

all : unlockMaker

unlockMaker : main.o Project.o Card.o Edge.o EdgeList.o Vertex.o VertexList.o 
	$(CC) $(LDFLAGS) $^ -o $@ $(EXE) $(LDLIBS)
	
	
main.o : main.h
Project.o : Project.h	
	
%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(PACKAGE) 


clean :
	rm main
	rm main.o
