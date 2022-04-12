CC = gcc
CFLAGS = -g -O2
TARGET = server
OBJECTS	= 
MAIN_SOURCE	= server.c

.SUFFIEXES: .c .o

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) $(MAIN_SOURCE)

.c .o : 
	rm -f $(OBJECTS)
	rm -f $(TARGET)
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJECTS)
	rm -f $(TARGET)