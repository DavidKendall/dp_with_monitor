all: dp

dp: dpmonitor.o main.o
	gcc -Wall -pthread -std=c99 -D_POSIX_C_SOURCE=199309L -o dp dpmonitor.o main.o

dpmonitor.o: dpmonitor.h dpmonitor.c
	gcc -Wall -pthread -std=c99 -D_POSIX_C_SOURCE=199309L -c dpmonitor.c

main.o: main.c dpmonitor.h
	gcc -Wall -pthread -std=c99 -D_POSIX_C_SOURCE=199309L -c main.c

clean:
	-rm dp *.o *~
