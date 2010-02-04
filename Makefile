LDFLAGS=-lustr -ldb
CFLAGS=-Wall -g

SRCS=$(wildcard *.c)
OBJS=$(patsubst %.c,%.o,$(SRCS))
BINS=$(patsubst %.c,%,$(SRCS))

SHELL_WRAP_SH=hrai.sh
SHELL_WRAP=`basename $(SHELL_WRAP_SH) .sh`
INSTALL_TO=/opt/hrai

all: $(BINS)

list: lib/hdb.o list.o
add: lib/hdb.o add.o
init: lib/hdb.o init.o

install:
	mkdir -p $(INSTALL_TO)/bin
	mkdir -p $(INSTALL_TO)/libexec
	cp $(BINS) $(INSTALL_TO)/libexec
	cp $(SHELL_WRAP_SH) $(INSTALL_TO)/bin/$(SHELL_WRAP)
	sed -i 's/INSTALLED=/INSTALLED=yes/' $(INSTALL_TO)/bin/$(SHELL_WRAP)

clean:
	rm -f $(BINS) *.o lib/*.o
