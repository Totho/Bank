#This is a variable
CC=g++
ZMQ=/home/utp/cs/HaloCS/zmq
ZMQ_HDRS=$(ZMQ)/include
ZMQ_LIBS=$(ZMQ)/lib

#regla principal
all: client server

cliente: client.c
$(CC) -std=c++11 -I/home/utp/cs/HaloCS/zmq/include -c client.c
$(CC) -std=c++11 -L/home/utp/cs/HaloCS/zmq/lib -o client client.o -lzmq -lczmq


server: server.c
$(CC) -std=c++11 -I/home/utp/cs/HaloCS/zmq/include -c server.c
$(CC) -std=c++11 -L/home/utp/cs/HaloCS/zmq/lib -o server server.o -lzmq -lczmq

clean:
rm -f client.o server.o client server *~

#export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/utp/csHaloCS/zmq/lib
