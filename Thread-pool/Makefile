CC := g++

SRCDIR := src
BUILDDIR := build
TARGET := bin/threadPool
TARGETDIR := bin

SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -g -w -std=c++14
INC := -I include
LIB := -pthread
STATIC_ANALYZER := oclint

$(TARGET) : $(OBJECTS)
	@echo " Linking "
	@mkdir -p bin
	@echo " $(CC) $^ -o $(TARGET) $(LIB) $(LINKER)"; $(CC) $^ -o $(TARGET) $(LIB)

$(BUILDDIR)/%.o : $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

static-analyze:
	$(STATIC_ANALYZER) $(SOURCES) -- -c $(CFLAGS) $(INC) $(LIB)

clean:
	@echo " Cleaning... "
	@echo " rm -rf $(BUILDDIR) $(TARGETDIR)/*"; rm -rf $(BUILDDIR) $(TARGETDIR)/*


.PHONY: clean
