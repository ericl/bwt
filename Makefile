default:
	g++ -g -std=c++0x -Wall -Wfatal-errors -lrt -O2 *.cpp -o bwt
	./bwt -test

clean:
	rm bwt
