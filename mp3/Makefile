#build variable
CC = g++

#option variable
CFLAGS = -std=c++11
LFLAGS = -o # Linker flags

#object variable
OBJS = mp3_20201588.o 

TARGET = mp3_20201588

all: $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) $(OBJS) $(LFLAGS) $(TARGET)

mp3_20201588.o: mp3_20201588.cc
	$(CC) $(CFLAGS) -c mp3_20201588.cc -o mp3_20201588.o

clean :
	rm -rf $(OBJS) $(TARGET)
