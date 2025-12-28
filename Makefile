TARGET = bin/dbview
SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, obj/%.o, $(SRC))

run: clean default
	./$(TARGET) -nf ./mynewdb.db 
	./$(TARGET) -f ./mynewdb.db -a "Timmy H.,123 S Oak Ln,121"

default: $(TARGET)

clean:
	rm -rf obj/*.o
	rm -rf bin/*
	rm -rf *.db

$(TARGET): $(OBJ)
	@mkdir -p bin
	gcc -o $@ $^

obj/%.o: src/%.c
	@mkdir -p obj
	gcc -c $< -o $@ -Iinclude
