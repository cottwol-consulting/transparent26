# Make file for source for mqtt adapter

# (C)2026 Cottwol Consulting

PROGS= osdp-mqtt-subscriber

CC=clang
CFLAGS=-c -g -I ../include -Wall -Werror
LINK=clang
LDFLAGS=-g -ljansson

all:	${PROGS}
	mkdir -p ../opt/osdp-conformance/bin
	cp ${PROGS} ../opt/osdp-conformance/bin

clean:
	rm -f a.out *.o ${PROGS}

osdp-mqtt-subscriber:	osdp-mqtt-subscriber.o Makefile
	${LINK} -o osdp-mqtt-subscriber osdp-mqtt-subscriber.o

osdp-mqtt-subscriber.o:	osdp-mqtt-subscriber.c
	${CC} ${CFLAGS} osdp-mqtt-subscriber.c

