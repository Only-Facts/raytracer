##
## EPITECH PROJECT, 2026
## Makefile
## File description:
## Makefile for the compilation of the executable
##

TARGET := interstonar
BUILD_DIR := build

all: $(TARGET)

$(TARGET):
	cmake -S . -B $(BUILD_DIR)
	cmake --build $(BUILD_DIR) --parallel $$(nproc)

clean:
	-cmake --build $(BUILD_DIR) --target clean 2> /dev/null

fclean: clean
	-rm -rf $(BUILD_DIR)

.NOTPARALLEL:
re: fclean all

.PHONY: all lib clean fclean re
