EXEC = main.exe

SRCDIR = src
INCDIR = include

CC = gcc
CFLAGS = -Wall -Werror -I$(INCDIR)
LDFLAGS = -lraylib -lgdi32 -lwinmm

SRC = $(wildcard $(SRCDIR)/*.c)

all: $(EXEC)

$(EXEC): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(EXEC) $(LDFLAGS)

clean:
	rm -f $(EXEC)

debug: CFLAGS += -g
debug: clean $(EXEC)

release: CFLAGS += -O3
release: clean $(EXEC)

run: $(EXEC)
	$(EXEC)

.PHONY: all clean debug release run
