# Compile the blockchain library
CC = gcc
# Compiler flags
CFLAGS = -Wall -Werror -Wno-deprecated-declarations -Wextra -pedantic -std=gnu90 -ggdb3 -c -I. -Itransaction/ -Iprovided/ -I../../crypto -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread

# Source files and object files
SRC := $(wildcard *.c) $(wildcard transaction/*.c) # Get all source files
OBJ := $(patsubst %.c, %.o, $(filter-out provided/%.c, $(SRC))) # Get all object files
NAME := libhblk_blockchain.a # Name of the library

# Phony targets
.PHONY: clean oclean flcean libhblk_blockchain.a 

# Compile the object files
clean:
	$(RM) $(OBJ) # Remove object files

# Remove the library
oclean:
	$(RM) $(NAME) # Remove the library

# Remove object files and the library
fclean: clean oclean # Remove object files and the library

# Target to build the library
libhblk_blockchain.a: blockchain.h transaction/transaction.h $(OBJ)
	ar rcs $(NAME) $(OBJ)
