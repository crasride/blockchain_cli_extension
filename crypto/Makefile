# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -Werror -pedantic -Wno-deprecated-declarations

# Source files
SRC_FILES = sha256.c ec_create.c ec_to_pub.c ec_from_pub.c ec_save.c ec_load.c ec_sign.c ec_verify.c

# Object files (generated from source files)
OBJ_FILES = $(SRC_FILES:.c=.o)

# Name of the static library
LIB_NAME = libhblk_crypto.a

# Default target
all: $(LIB_NAME)

# Rule to create the static library
$(LIB_NAME): $(OBJ_FILES)
	ar rcs $@ $^

# Rule to compile individual source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up object files
clean:
	rm -f $(OBJ_FILES)

# Clean up object files and the static library
fclean: clean
	rm -f $(LIB_NAME)

# Recompile everything
re: fclean all

# Define phony targets to prevent conflicts with file names
.PHONY: all clean fclean re
