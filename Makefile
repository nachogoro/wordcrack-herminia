TARGET =	src/main.o \
			src/Board.o \
			src/Dictionary.o \

GPP = g++ -c -g -pg -W -Wall -Werror -std=c++11 -O2

herminia: $(TARGET)
	g++ -std=c++11 -W -Wall -Werror -O2 -o herminia *.o -pthread

src/main.o: src/main.cpp \
		src/Board.cpp \
		src/Dictionary.cpp
	$(GPP) src/main.cpp

src/Board.o: src/Board.cpp
	$(GPP) src/Board.cpp

src/Dictionary.o: src/Dictionary.cpp
	$(GPP) src/Dictionary.cpp

clean:
	rm -f *.o
	rm -f *~
	rm -f herminia
