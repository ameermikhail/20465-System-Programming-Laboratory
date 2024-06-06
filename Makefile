CC = gcc
CFLAGS = -g -Wall -ansi -pedantic-errors -Iinclude
SRCDIR = src
INCDIR = include
EXEC = assembler
SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(SRC:$(SRCDIR)/%.c=%.o)

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@ $(LDFLAGS)

$(OBJ): %.o : $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC) *.ext *.ent *.am *.ob
