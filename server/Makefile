CC      ?= gcc
WSDIR    = $(CURDIR)/libs/wsServer
INCLUDE  = -I $(WSDIR)/include
CFLAGS   =  -Wall -Wextra -O2
CFLAGS  +=  $(INCLUDE) -std=c99 -pthread -pedantic
LIB      =  $(WSDIR)/libws.a

# Check if verbose examples
ifeq ($(VERBOSE_EXAMPLES), no)
	CFLAGS += -DDISABLE_VERBOSE
endif

.PHONY: all clean

# Examples
all: main

# Send receive
main: main.c $(LIB)
	$(CC) $(CFLAGS) $(LDFLAGS) main.c -o main $(LIB)

# Clean
clean:
	@rm -f main