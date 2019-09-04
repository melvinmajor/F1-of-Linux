SRC_DIR=src
SRC=$(wildcard $(SRC_DIR)/*.c)
LIBS=$(wildcard lib/*.c)
CFLAGS=-Wall -Wextra -lpthread -lrt -std=gnu99 -lncurses
EXE=f1

.PHONY: compile run format watch clean

compile: $(EXE)

$(EXE):	$(SRC)
	$(CC) $(SRC) \
	    $(LIBS) \
	    $(CFLAGS) \
	    -o $(EXE)

run:
	./$(EXE)

format:
	clang-format -i $(SRC)

watch:
	echo $(SRC) | entr -s 'make --no-print-directory compile' 

clean:
	rm $(EXE)
