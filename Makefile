CFLAGS= -fmax-errors=5 -fsanitize=address,leak

interpreter:  source/interpreter.cpp
	g++ source/interpreter.cpp -o bin/interpreter $(CFLAGS)

