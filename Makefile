
INCLUDE := -I./include/ -I./lib/ -I./lib/glm/
CFLAGS := -lglfw -ldl -Werror -Wall -pedantic -Wno-unused-but-set-variable
CC := g++

SRC_PATH := ./src

TARGET := ./build/Golf
SRCS := $(wildcard $(SRC_PATH)/*.cpp)
OBJS := ./objs/golf.o

run: $(TARGET)
	$(TARGET)
	rm -rf ./*.o

$(TARGET): $(OBJS)
	$(CC) ./*.o -o $(TARGET) $(INCLUDE) $(CFLAGS)

$(OBJS): ./objs
	$(CC) -c $(SRCS) ./src/glad.c $(INCLUDE) $(CFLAGS)

./objs: ./build
	mkdir -p ./objs

./build:
	mkdir -p ./build

.PHONY: clean
clean:
	rm -rf ./build ./objs
	rm -rf ./*.o
