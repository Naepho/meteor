APP = meteor
CC := gcc # This is the main compiler
# CC := clang --analyze # and comment out the linker last line for sanity
SRCDIR := src
BUILDDIR := build
TARGET := bin/$(APP)
TARGET_DEBUG := bin/$(APP)_debug
RUNARGS :=

 
SRCEXT := c
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
OBJECTS_DEBUG := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=_debug.o))
CFLAGS := -Wall -Wextra -Wpedantic
CFLAGS_DEBUG := $(CFLAGS) -ggdb3 -fanalyzer

GTKFLAGS = $(shell pkg-config --cflags gtk4)
GTKLIBS = $(shell pkg-config --libs gtk4)
LIB := $(GTKLIBS)
INC := -I include $(GTKFLAGS)


build: $(TARGET)

run: $(TARGET)
	@echo "Running...";
	./$(TARGET) $(RUNARGS)

debug: $(TARGET_DEBUG)
	@echo "Running debug...";
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=valgrind_log.txt ./$(TARGET_DEBUG) $(RUNARGS)

$(TARGET): $(OBJECTS)
	@mkdir -p $(dir $@)
	@echo " Linking...";
	@echo " $(CC) $^ -o $(TARGET) $(LIB)"; $(CC) $^ -o $(TARGET) $(LIB)

$(TARGET_DEBUG): $(OBJECTS_DEBUG)
	@echo " Linking debug...";
	@echo " $(CC) $^ -o $(TARGET_DEBUG) $(LIB)"; $(CC) $^ -o $(TARGET_DEBUG) $(LIB)

$(BUILDDIR)/%_debug.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(dir $@)
	@echo " $(CC) $(CFLAGS_DEBUG) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS_DEBUG) $(INC) -c -o $@ $<

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(dir $@)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo " Cleaning..."; 
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)

# Tests
tester:
	$(CC) $(CFLAGS) test/tester.cpp $(INC) $(LIB) -o bin/tester

# Spikes
ticket:
	$(CC) $(CFLAGS) spikes/ticket.cpp $(INC) $(LIB) -o bin/ticket

.PHONY: clean
