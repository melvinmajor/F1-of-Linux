SRC_DIR=src
SRC = $(wildcard $(SRC_DIR)/*.c)
EXE=out/projet

all:
	$(CC) $(SRC)  -lpthread -lrt -o $(EXE)

format:
	clang-format -i -style="{IndentWidth: 4, ColumnLimit: 120}" $(SRC)

clean:
	rm $(EXE)
