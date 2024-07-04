TARGET := a.out

find_arrow:
	cc \
		-std=c99 \
		-I../../../include \
		-Wall -Wextra \
		-DTEST_MARCHING_SQUARE \
		../memory.c main.c test_find_arrow_in_triangle.c \
		-o $(TARGET)

clean:
	rm -f $(TARGET)
