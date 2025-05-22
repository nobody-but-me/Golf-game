
INCLUDE := -I./include/ -I./lib/ -I./lib/glm/ -I./lib/imgui/
CFLAGS := -Wl,--copy-dt-needed-entries -lglfw -lglut -ldl -Werror -Wall -pedantic -Wno-unused-but-set-variable
CC := g++

SRC_PATH := ./src

TARGET := ./build/Golf
SRCS := $(wildcard $(SRC_PATH)/*.cpp ./lib/imgui/*.cpp ./lib/imgui/backends/*.cpp)
OBJS := ./objs/golf.o

exec: $(TARGET)
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

.PHONY: clean run
clean:
	rm -rf ./build ./objs
	rm -rf ./*.o
run:
	$(TARGET)
