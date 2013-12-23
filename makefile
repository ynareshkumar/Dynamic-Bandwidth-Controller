all:bandwidthlimiter
bandwidthlimiter:bandwidthlimiter.o
        g++ -Wall -o bandwidthlimiter bandwidthlimiter.o 
clean:
        rm -f *.o
        rm -f bandwidthlimiter
