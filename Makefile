# Settings
NAME = Chaos
SRC_DIR = src
OBJECT_DIR = $(SRC_DIR)/objects
INCLUDE_DIR = $(SRC_DIR)/include

# Sources
SOURCES = $(wildcard $(SRC_DIR)/*.c)
# Object files
OBJECTS = $(patsubst $(SRC_DIR)%,$(OBJECT_DIR)%, $(patsubst %.c,%.o, $(SOURCES)))
# Dependencies
DEPS = $(wildcard $(INCLUDE_DIR)/*.h)

# Compiler settings
CC = clang
OPT = -O0
CFLAGS = -Wall -Wextra -Werror -g -I $(INCLUDE_DIR) $(OPT)
LDLIBS = -lm -ldl
SQLITE = -lsqlite3

all: $(NAME)

$(NAME): $(OBJECTS)
	$(CC) $(SQLITE) $(LDLIBS) -o $@ $^

$(OBJECT_DIR)%.o: $(SRC_DIR)%.c $(DEPS)
	@mkdir -p $(OBJECT_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(NAME) $(OBJECT_DIR)