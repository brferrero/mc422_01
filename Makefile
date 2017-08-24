OUTPUT=ep1sh

CC=gcc
CC_OPT=-std=c11 -Wall -ansi

.PHONY: all
$(OUTPUT): $(OUTPUT).c
	$(CC) -o $(OUTPUT) $(CC_OPT) $(OUTPUT).c 

.PHONY: clean
clean:
	rm -f $(OUTPUT)
