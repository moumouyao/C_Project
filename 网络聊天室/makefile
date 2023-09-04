CC = gcc
CFLAGS = -Wall -Wextra -std=c11
LDFLAGS = -pthread
MYSQL_LIB = -L /path/to/mysql/lib -lmysqlclient

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
EXES = $(SRCS:.c=)

all: $(EXES)

$(EXES): $(OBJS)
	$(CC) $(CFLAGS) $@.o -o $@ $(LDFLAGS) $(MYSQL_LIB)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ -I ./ -I /path/to/mysql/include

clean:
	rm -f $(OBJS) $(EXES)

