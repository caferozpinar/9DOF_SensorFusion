# Compiler
CC = gcc

# Flags
CFLAGS = -Iinclude
LDFLAGS = -lGL -lGLU -lglut -lm -lpthread

# Source files and object files
SRCS = $(wildcard src/*.c)
OBJS = $(SRCS:src/%.c=obj/%.o)

# Output executable
TARGET = Debug/SensorFusion

# Default target
all: $(TARGET)

# Linking step
$(TARGET): $(OBJS)
	@mkdir -p Debug
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Compile source files to object files
obj/%.o: src/%.c
	@mkdir -p obj
	$(CC) -c $< -o $@ $(CFLAGS)

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)
	rm -rf Debug obj

# Run the program
run: all
	./$(TARGET)
