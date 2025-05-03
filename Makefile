CC=gcc
CFLAGS=-g

AppAliveMonitor.bin:AppAliveMonitor.o
	${CC} ${CFLAGS} AppAliveMonitor.o -o AppAliveMonitor.bin -L AAM_Lib/AAM_Lib.a

AppAliveMonitor.o:AppAliveMonitor.c
	${CC} ${CFLAGS} -c AppAliveMonitor.c -I . -o AppAliveMonitor.o

all:
	make

clean:
	rm *.o
	rm *.bin