CC ?= cc
CFLAGS = -Wall -Wextra -std=c99 -pedantic -g
ODIR = ./bin
SDIR = ./src

_OBJ = main matrix parse beam
OBJ = $(patsubst %,$(ODIR)/%.o,$(_OBJ))

all: $(OBJ)
	$(CC) $(OBJ) -o feasolver $(CFLAGS)

$(ODIR)/%.o : $(SDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm $(OBJ) ./feasolver
