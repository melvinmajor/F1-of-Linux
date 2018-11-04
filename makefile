SRC_DIR=src
SRC=$(wildcard $(SRC_DIR)/*.c)
CFLAGS=-Wall -Wextra -lpthread -lrt
EXE=out/projet

compile:
	$(CC) $(SRC) $(CFLAGS) -o $(EXE)

format:
	clang-format -i -style="{IndentWidth: 4, ColumnLimit: 120}" $(SRC)

clean:
	rm $(EXE)
