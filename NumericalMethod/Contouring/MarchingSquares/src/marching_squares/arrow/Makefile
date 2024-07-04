TARGET := a.out

find_arrows:
	cc \
		-std=c99 \
		-I../../../include \
		-Wall -Wextra \
		-DTEST_MARCHING_SQUARE \
		../memory.c main.c test_find_arrows_of_cell.c \
		-o $(TARGET)

clean:
	rm -f $(TARGET)
