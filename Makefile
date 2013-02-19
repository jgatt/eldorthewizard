CXX= g++
CXXFLAGS = -g -MMD -L/usr/X11r6/lib -lX11
OBJECTS = $(shell find . -name "*.cc" | sed "s/\.cc/\.o/g" | tr "\n" " ")
EXEC = prog

$(EXEC): $(OBJECTS)
	$(CXX) $(OBJECTS) $(CXXFLAGS) -o $(EXEC)

-include ${OBJECTS:.o=.d}

assign1: $(EXEC)

all: $(EXEC) run

clean:
	rm -rf $(EXEC) $(OBJECTS) $(OBJECTS:.o=.d)
	clear

run:
	./$(EXEC)
