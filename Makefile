OUTPUT=ep1sh

CC=gcc

CC_OPT=-std=c11 -Wall -ansi
CC_LIBS=-lreadline
CC_pth=-lpthread

.PHONY: all
all: $(OUTPUT) ep1

$(OUTPUT): $(OUTPUT).c
	$(CC) -o $(OUTPUT) $(CC_pth) $(CC_OPT) $(OUTPUT).c $(CC_LIBS)

ep1: ep1.c
	$(CC) -o ep1 $(CC_OPT) ep1.c $(CC_pth)

.PHONY: clean
clean:
	rm -f $(OUTPUT) ep1
