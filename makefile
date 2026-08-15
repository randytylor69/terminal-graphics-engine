FLAGS = -std=c++23

main.out : main.o termigine.o
	g++ ${FLAGS} $^ -o main.out

main.o : main.cpp
	g++ ${FLAGS} -c $^ -o main.o

termigine.o : termigine.cpp
	g++ ${FLAGS} -c $^ -o termigine.o


