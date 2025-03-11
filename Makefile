APP = meteor
CC = gcc
SRCDIR = src
BINDIR = bin
BINDIR_release = bin_release
BUILDDIR = build
BUILDDIR_release = build_release
TARGET = $(BINDIR)/$(APP)
TARGET_release = $(BINDIR_release)/$(APP)
RUNARGS :=
CFLAGS_HARD = -Wall -Wextra -Werror -Wpedantic
CFLAGS = -Wall -Wextra
LIB = 
INC = -I include

SRCEXT = c
SOURCES = $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS = $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
OBJECTS_release = $(patsubst $(SRCDIR)/%,$(BUILDDIR_release)/%,$(SOURCES:.$(SRCEXT)=.o))

run: $(TARGET)
	@echo "Running...";
	./$(TARGET) $(RUNARGS)

$(TARGET): $(OBJECTS)
	@echo "Linking...";
	@mkdir -p $(dir $@)
	@echo "$(CC) $^ -o $(TARGET) $(LIB)"; $(CC) $^ -o $(TARGET) $(LIB)

$(TARGET_release): $(OBJECTS_release)
	@echo "Linking...";
	@echo "$(CC) $^ -o $(TARGET) $(LIB)"; $(CC) $^ -o $(TARGET) $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(dir $@)
	@echo "$(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

$(BUILDDIR_release)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(dir $@)
	@echo "$(CC) $(CFLAGS_HARD) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo "Cleaning...";
	$(shell rm -rf $(BUILDDIR))
	$(shell rm -rf $(BINDIR))
