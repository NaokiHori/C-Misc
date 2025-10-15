CFLAG := -std=c99 -Wall -Wextra -Wpedantic -O0
TARGET := ./a.out

help:
	@echo "test"
	@echo "clean"

test:
	make clean && \
	cc \
		$(CFLAG) \
		-DAATREE_TEST \
		aatree/check.c \
		aatree/delete.c \
		aatree/destroy.c \
		aatree/display.c \
		aatree/insert.c \
		aatree/search.c \
		aatree/skew.c \
		aatree/split.c \
		aatree.c \
		-o $(TARGET) && \
	$(TARGET) && \
	make clean

clean:
	rm -f $(TARGET)

