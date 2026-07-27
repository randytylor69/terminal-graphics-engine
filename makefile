FLAGS = -std=c++23

main.out : main.o consoleEngine3D.o
	g++ ${FLAGS} $^ -o main.out

main.o : main.cpp
	g++ ${FLAGS} -c $^ -o main.o

consoleEngine3D.o : consoleEngine3D.cpp
	g++ ${FLAGS} -c $^ -o consoleEngine3D.o

