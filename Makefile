CC := clang
MKDIR := mkdir
RM := rm -rf
INCLUDE := ./include
SRC := ./src
OBJ := ./obj
BIN := ./bin
SRCS := $(wildcard $(SRC)/*.c)
OBJS := $(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(SRCS))
TARGET := $(BIN)/remindme
CFLAGS := -std=c99 -O2 -Wall -Wextra -Wformat=2 -pedantic -pipe -I/usr/local/include -I$(INCLUDE)
LDFLAGS := -L/usr/local/lib
LIBS := -lncurses -lmpv

.PHONY: default all clean

default: $(TARGET)
all: default

$(TARGET): $(OBJS) | $(BIN)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ $(LIBS)

$(OBJ)/%.o: $(SRC)/%.c | $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

$(BIN) $(OBJ):
	$(MKDIR) $@

clean:
	$(RM) $(OBJ) $(BIN)

