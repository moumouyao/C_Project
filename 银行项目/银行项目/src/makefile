CC = gcc
CFLAGS = -Wall -Wextra -std=c11

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
EXES = $(SRCS:.c=)

all: $(EXES)

$(EXES): $(OBJS)
	$(CC) $(CFLAGS) $@.o -o $@ -I ../include

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ -I ../include

clean:
	rm -f $(OBJS) $(EXES)
