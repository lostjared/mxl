CC=g++
CXXFLAGS= -I/usr/local/include
LDFLAGS= -L/usr/local/lib -lmasc -lmxl


all:
	$(CC) $(CXXFLAGS)  main.cpp -o test_of_so $(LDFLAGS)

clean:
	rm -rf *~ test_of_so
