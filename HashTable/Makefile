run: build
	./HashTable.out
build: HashTable.o List.o
	g++ HashTable.o List.o -o HashTable.out

List.o: List.cpp
	g++ -c List.cpp -o List.o
HashTable.o: HashTable.cpp
	g++ -c HashTable.cpp -o HashTable.o
clean:
	rm *.o
	rm *.log