C1=client1
C2=client2
S=server
CFLAGS=-g -std=c++14


all: $(S).o $(C1).o $(C2).o
	g++ $(CFLAGS) $(S).o -o $(S)
	g++ $(CFLAGS) $(C1).o -o $(C1)
	g++ $(CFLAGS) $(C2).o -o $(C2)

$(C1).o : $(C1).cpp
	g++ $(CFLAGS) -c $(C1).cpp -o $(C1).o

$(C2).o : $(C2).cpp
	g++ $(CFLAGS) -c $(C2).cpp -o $(C2).o

$(S).o: $(S).cpp
	g++ $(CFLAGS) -c $(S).cpp -o $(S).o

clean:
	rm $(S) $(C1) $(C2) *.o
