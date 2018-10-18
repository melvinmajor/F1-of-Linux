CC=gcc
SRC=projet.c
OUT=projet

make:
	$(CC) $(SRC) -o $(OUT)

format:
	clang-format -i -style="{IndentWidth: 4, ColumnLimit: 120}" $(SRC)

clean:
	rm $(OUT)
