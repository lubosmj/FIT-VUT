#basic name of file
NAME= proj2
#executable file
EXEC= $(NAME)

#source code file
CFILE= $(NAME).c
#header file
HEADFILE= $(NAME).h
#object file
OBJFILE= $(NAME).o

#c compiler
CC= gcc
#flags passed to C pre-processor
CFLAGS= -std=gnu99 -Wall -Wextra -Werror -pedantic 
#flags passed to the linker
LDFLAGS= -lrt -pthread

#fake target
.PHONY: clear

#default rule co compile
all: $(NAME)

#generate executable file
$(EXEC): $(OBJFILE)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

#generate object file
$(OBJFILE): $(CFILE) $(HEADFILE)
	$(CC) $(CFLAGS) -c $< -o $@

#clear object file and executable file
clear:
	-rm -f *.o $(NAME)
