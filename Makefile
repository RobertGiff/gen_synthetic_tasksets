TARGETS = generator
CC = g++ -I../confs/inc -I../sim/inc
CFLAGS = -c -Wall -g

all: $(TARGETS)

generator: main.o tinystr.o tinyxml.o tinyxmlerror.o tinyxmlparser.o
	$(CC) $^ -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -rf *.o $(TARGETS)
