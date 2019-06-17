CC=g++
CFLAGS=-g -c
LDFLAGS=
SOURCES=lexer.cpp types.cpp scanner.cpp spars.cpp sinter.cpp main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=rbasic

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -r $(OBJECTS)
