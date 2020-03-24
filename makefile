des: main.o des.o base_n.o replace.o working_mode.o
	g++ main.o des.o base_n.o replace.o working_mode.o -o demo
main.o: main.cpp des.h working_mode.h
	g++ -c main.cpp -o main.o -std=c++11 -O3
des.o: des.cpp base_n.h
	g++ -c des.cpp -o des.o -std=c++11 -O3
base_n.o: base_n.cpp
	g++ -c base_n.cpp -o base_n.o -std=c++11 -O3
replace.o: replace.cpp
	g++ -c replace.cpp -o replace.o -std=c++11 -O3
working_mode.o: working_mode.cpp base_n.h
	g++ -c working_mode.cpp -o working_mode.o -std=c++11 -O3

clean:
	rm *.o
