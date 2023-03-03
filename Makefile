
INSTALL=/usr/bin/
TARGET=pom
SOURCES=pom.c
HEADERS=argp.h

CC=cc

all: $(SOURCES) $(HEADERS)
	$(CC) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET)

install:
	cp $(TARGET) $(INSTALL)

uninstall:
	rm -f $(INSTALL)$(TARGET)


