
.PHONEY:clean
../bin/lemon:../src/*.cpp
	g++ -o ../bin/lemon main.cpp ../src/*.cpp -static -lmxml -lpthread -I ../include/
clean:
	rm -f *.o ../bin/lemon
