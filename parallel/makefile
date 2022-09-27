all: clean parallel

parallel : main.cpp
	g++ -std=c++11 -pthread -w main.cpp -o ImageFilters.out

clean:
	rm -f ImageFilters.out
