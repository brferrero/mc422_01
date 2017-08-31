OUTPUT=ep1sh

CC=gcc

CC_OPT=-std=c11 -Wall -ansi
CC_LIBS=-lreadline
CC_pth=-pthread

.PHONY: all
all: $(OUTPUT) ep1

$(OUTPUT): $(OUTPUT).c
	$(CC) -o $(OUTPUT) $(CC_pth) $(CC_OPT) $(OUTPUT).c $(CC_LIBS)

ep1: ep1.c
	$(CC) -o ep1 $(CC_pth) ep1.c 

.PHONY: clean
clean:
	rm -f $(OUTPUT) ep1
