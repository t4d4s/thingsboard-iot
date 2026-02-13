CFLAGS = -Wall
LDFLAGS = -lmosquitto -lcurl -lcjson

SOURCES = $(wildcard *.c)

OBJECTS = $(SOURCES:.c=.o)

TARGET = tb

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	
clean:
	rm -f $(TARGET) $(OBJECTS)
