# Compiler
CC = gcc

# Flags
DEBUG_CFLAGS = -Iinclude -Wall -Wextra -g
RELEASE_CFLAGS = -Iinclude -Wall -Wextra -O2 -DNDEBUG
LDFLAGS = -lGL -lGLU -lglut -lm -lpthread

# Directories
SRCDIR = src
OBJDIR = obj
BINDIR = Debug
RELEASEDIR = Release

# Source files and object files
SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))

# Output executables
DEBUG_TARGET = $(BINDIR)/SensorFusion
RELEASE_TARGET = $(RELEASEDIR)/SensorFusion

# Default target (Debug build)
all: $(DEBUG_TARGET)

# Debug build
$(DEBUG_TARGET): CFLAGS = $(DEBUG_CFLAGS)
$(DEBUG_TARGET): $(OBJS)
	@mkdir -p $(BINDIR)
	$(CC) $(OBJS) -o $(DEBUG_TARGET) $(LDFLAGS)

# Release build
release: CFLAGS = $(RELEASE_CFLAGS)
release: TARGET = $(RELEASE_TARGET)
release: $(OBJS)
	@mkdir -p $(RELEASEDIR)
	$(CC) $(OBJS) -o $(RELEASE_TARGET) $(LDFLAGS)

# Compile source files to object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) -c $< -o $@ $(CFLAGS)

# Clean up build files
clean:
	rm -f $(OBJDIR)/*.o $(DEBUG_TARGET) $(RELEASE_TARGET)
	rm -rf $(BINDIR) $(RELEASEDIR) $(OBJDIR)

# Run the program in Debug mode with a default sensor_bitmap
run: all
	./$(DEBUG_TARGET) 110110000

# Run the program in Release mode with a default sensor_bitmap
run-release: release
	./$(RELEASE_TARGET) 110110000

# Phony targets
.PHONY: all clean run release run-release
