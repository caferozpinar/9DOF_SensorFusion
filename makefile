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
DEBUG_TARGET_LIVE = $(BINDIR)/SensorFusionLive
RELEASE_TARGET = $(RELEASEDIR)/SensorFusion

# Default target (Debug build)
all: $(DEBUG_TARGET) $(DEBUG_TARGET_LIVE)

# Debug build for main.c
$(DEBUG_TARGET): CFLAGS = $(DEBUG_CFLAGS)
$(DEBUG_TARGET): $(OBJDIR)/main.o $(filter-out $(OBJDIR)/mainLiveStream.o, $(OBJS))
	@mkdir -p $(BINDIR)
	$(CC) $^ -o $@ $(LDFLAGS)

# Debug build for mainLiveStream.c
$(DEBUG_TARGET_LIVE): CFLAGS = $(DEBUG_CFLAGS)
$(DEBUG_TARGET_LIVE): $(OBJDIR)/mainLiveStream.o $(filter-out $(OBJDIR)/main.o, $(OBJS))
	@mkdir -p $(BINDIR)
	$(CC) $^ -o $@ $(LDFLAGS)

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
	rm -f $(OBJDIR)/*.o $(DEBUG_TARGET) $(DEBUG_TARGET_LIVE) $(RELEASE_TARGET)
	rm -rf $(BINDIR) $(RELEASEDIR) $(OBJDIR)

# Run the program in Debug mode with a default sensor_bitmap (Data mode)
run: $(DEBUG_TARGET)
	./$(DEBUG_TARGET) 110000000

# Run the live stream program in Debug mode (Live mode)
run-live: $(DEBUG_TARGET_LIVE)
	./$(DEBUG_TARGET_LIVE) 110000000

# Run the program in Release mode with a default sensor_bitmap (Data mode)
run-release: release
	./$(RELEASE_TARGET) 110000000

# Phony targets
.PHONY: all clean run release run-release run-live
