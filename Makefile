GCC = g++
FLAGS = -std=c++17 -Wall -Iinclude

SRC = src/sorting_maths.cpp src/big_int.cpp src/complex.cpp

OBJ = src/sorting_maths.o src/big_int.o src/complex.o

TARGET = sorting_maths

$(TARGET): $(OBJ)
	$(GCC) $(OBJ) -o $(TARGET)

src/%.o: src/%.cpp
	$(GCC) $(FLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)
