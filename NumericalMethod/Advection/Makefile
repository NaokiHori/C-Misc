CC     := cc
CFLAG  := -std=c99 -Wall -Wextra -O3
INC    := -Iinclude
LIB    :=
SRCDIR := src
OBJDIR := obj
SRCS   := $(shell find $(SRCDIR) -type f -name "*.c")
OBJS   := $(patsubst %.c, $(OBJDIR)/%.o, $(SRCS))
DEPS   := $(patsubst %.c, $(OBJDIR)/%.d, $(SRCS))
TARGET := a.out
OUTDIR := output

help:
	@echo "all     : create \"$(TARGET)\""
	@echo "clean   : remove \"$(TARGET)\" and object files under \"$(OBJDIR)\""
	@echo "output  : create space for artifacts"
	@echo "datadel : remove files in \"$(OUTDIR)\""
	@echo "help    : show this message"

all: $(TARGET)

clean:
	$(RM) -r $(OBJDIR) $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAG) -o $@ $^ $(LIB)

$(OBJDIR)/%.o: %.c
	@if [ ! -e $(dir $@) ]; then \
		mkdir -p $(dir $@); \
	fi
	$(CC) $(CFLAG) -MMD $(INC) -c $< -o $@

-include $(DEPS)

output:
	@if [ ! -e $(OUTDIR) ]; then \
		mkdir -p $(OUTDIR); \
	fi

datadel:
	$(RM) -r $(OUTDIR)/*.dat

.PHONY : help all clean output datadel

