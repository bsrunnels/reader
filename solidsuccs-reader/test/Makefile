CC = g++
test:
	$(CC) test.cpp -o test -I../
	./test
test-muparser:
	$(CC) test.cpp -o test -I../ -DMUPARSER -lmuparser
	./test
clean:
	rm test
