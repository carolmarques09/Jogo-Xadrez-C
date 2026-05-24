CC = gcc

CFLAGS = -Wall -Wextra -std=c17 -Iinclude

RAYLIB_INCLUDE = -IC:/raylib/include
RAYLIB_LIB = -LC:/raylib/lib

MYSQL_INCLUDE = -IC:/mysql/include
MYSQL_LIB = -LC:/mysql/lib

LIBS = -lraylib -lmysqlclient

SRC = $(wildcard src/*.c)

TARGET = bin/chess.exe


all:
	$(CC) $(CFLAGS) \
	$(RAYLIB_INCLUDE) \
	$(MYSQL_INCLUDE) \
	$(SRC) \
	$(RAYLIB_LIB) \
	$(MYSQL_LIB) \
	$(LIBS) \
	-o $(TARGET)

run: all
	./$(TARGET)

clean:
	del /Q bin\*.exe