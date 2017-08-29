OUTPUT=ep1sh

CC=gcc
CC_OPT=-std=c11 -Wall -ansi -lreadline

.PHONY: all
$(OUTPUT): $(OUTPUT).c
	$(CC) -o $(OUTPUT)  $(OUTPUT).c $(CC_OPT)

.PHONY: clean
clean:
	rm -f $(OUTPUT)
