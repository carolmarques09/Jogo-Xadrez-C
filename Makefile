CC = gcc

CFLAGS = -Wall -Wextra -std=c17 -Iinclude

RAYLIB_INCLUDE = -IC:/raylib/include
RAYLIB_LIB = -LC:/raylib/lib

MYSQL_HDR = C:/mysql/include/mysql/mysql.h
ifneq ($(wildcard $(MYSQL_HDR)),)
  MYSQL_INCLUDE = -IC:/mysql/include
  MYSQL_LIB = -LC:/mysql/lib
  MYSQL_LIBS = -lmysqlclient
else
  CFLAGS += -DNO_MYSQL
  $(info MySQL nao encontrado em C:/mysql - compilando sem banco de dados)
endif

WIN_LIBS = -lopengl32 -lgdi32 -lwinmm
LIBS = -lraylib $(MYSQL_LIBS) $(WIN_LIBS)

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
	del /Q bin\*.exe 2>nul || true
