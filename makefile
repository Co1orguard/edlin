edlin: edlin.o

	g++ edlin.o -o edlin


edlin.o: edlin.cpp

	g++ -c edlin.cpp -o edlin.o

run:

	./edlin