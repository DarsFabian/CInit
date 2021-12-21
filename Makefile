CC = gcc -Wall
TARGET = CInit

SOURCE_FILES = $(shell find src/ -name '*.c')
OBJECT_FILES = $(SOURCE_FILES:src/%.c=build/obj/%.o)

all : $(TARGET)

$(OBJECT_FILES) : $(SOURCE_FILES)
	$(CC) -I src/include -o $@ -c $<

$(TARGET): $(OBJECT_FILES)
	$(CC) $^ -o build/$@ -I src/include

clear:
	if [ -e build/obj/*.o ]; then rm build/obj/*.o; fi;
	if [ -f build/$(TARGET) ]; then rm build/$(TARGET); fi;

.SILENT:clear