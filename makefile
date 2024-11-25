# Compiler
CC = gcc

# Flags
CFLAGS = -Iinclude -lm

# Source files and object files
SRCS = src/main.c src/SensorFusion.c
OBJS = $(SRCS:.c=.o)

# Output executable
TARGET = ./Debug/SensorFusion

# Default target
all: $(TARGET)

# Linking step
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(CFLAGS)

# Compile source files to object files
%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)

# Run the program
run: all
	./$(TARGET)
