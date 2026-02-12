CXX      = g++
# Agrego las flags que tenías en tu comentario: -g (debug) y -pthread
CXXFLAGS = -std=c++17 -Wall -Wextra -g -pthread
SRC      = main.cpp DemoStack.cpp 
TARGET   = main
all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

run: $(TARGET)
	./$(TARGET)
clean:
	rm -f $(TARGET) $(TARGET).exe *.o