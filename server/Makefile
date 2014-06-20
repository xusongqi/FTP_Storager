# ------------------------------------------------------------------------
#
# 作者：韩佩言
#
# ------------------------------------------------------------------------
#!Makefile
C_SOURCES = $(shell find . -name "*.c")
C_OBJECTS = $(patsubst %.c, %.o, $(C_SOURCES))

CC = gcc
C_FLAGS = -D_REENTRANT -DSERVER_DEBUG -c -Wall -Iinclude -g
LINK_FLAGS = -I/usr/include/mysql -L/usr/lib64/mysql -lmysqlclient -lpthread
PROGRAM = server

all: $(C_OBJECTS) 
	@echo 链接 ...
	$(CC) $(C_OBJECTS) $(LINK_FLAGS) -o $(PROGRAM)
	$(RM) $(C_OBJECTS)

.c.o:
	@echo 编译 $< ...
	$(CC) $(C_FLAGS) $< -o $@

.PHONY:clean
clean:
	@echo 清除 ...
	$(RM) $(PROGRAM)

