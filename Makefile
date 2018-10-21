CC = gcc
CFLAGS := -g -Wall -ansi
INCLUDE :=.
OBJECTS := test.o buddy16.o
TARGET := buddy

all: $(TARGET)

$(OBJECTS):%.o:%.c 
	@ $(CC) $(CFLAGS) -I$(INCLUDE) -c $<

$(TARGET):$(OBJECTS)
	@ $(CC) -o $@ $^

clean:
	@- rm -f $(TARGET) *.o