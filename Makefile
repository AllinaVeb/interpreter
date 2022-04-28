CFLAGS= -fmax-errors=5 -fsanitize=address,leak

interpreter:  user/example.cpp
	g++ user/example.cpp source/interpreter.cpp -I headers/ -o bin/example $(CFLAGS)

