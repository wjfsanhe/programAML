CC=arc-linux-uclibc-gcc
SRC_FILES=main.c Amlogicframe.c 
OBJ_FILE=main.o Amlogicframe.o
TARGET=ztetest 
all:$(TARGET) copy
$(TARGET):$(OBJ_FILE)
	$(CC) -pthread -lrt -o $(TARGET) $(OBJ_FILE) 
$(MK_OBJS):%.o:%.c
	$(CC) -c $< $@

clean:
	-rm -rf ztetest main.o Amlogicframe.o 
copy:
	cp ztetest /nfsroot/wangjf/rootfs_a3/usr/bin 
	#cp pic /home/fengliang/test/Pre_build_V1.1/rootfs/bin
