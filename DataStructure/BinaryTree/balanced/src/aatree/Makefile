CFLAG := -std=c99 -Wall -Wextra -Wpedantic -O0
TARGET := ./a.out

help:
	@echo "test_skew"
	@echo "test_split"
	@echo "test_insert"
	@echo "test_delete"
	@echo "test_search"
	@echo "test_all"
	@echo "clean"

test_skew:
	make clean && \
	cc $(CFLAG) -DAATREE_SKEW_TEST skew.c -o $(TARGET) && \
	$(TARGET) && \
	make clean

test_split:
	make clean && \
	cc $(CFLAG) -DAATREE_SPLIT_TEST split.c -o $(TARGET) && \
	$(TARGET) && \
	make clean

test_insert:
	make clean && \
	cc $(CFLAG) -DAATREE_INSERT_TEST skew.c split.c check.c insert.c -o $(TARGET) && \
	$(TARGET) && \
	make clean

test_delete:
	make clean && \
	cc $(CFLAG) -DAATREE_DELETE_TEST skew.c split.c check.c delete.c -o $(TARGET) && \
	$(TARGET) && \
	make clean

test_search:
	make clean && \
	cc $(CFLAG) -DAATREE_SEARCH_TEST check.c search.c -o $(TARGET) && \
	$(TARGET) && \
	make clean

test_all:
	make test_skew && \
	make test_split && \
	make test_insert && \
	make test_delete && \
	make test_search

clean:
	rm -f $(TARGET)

