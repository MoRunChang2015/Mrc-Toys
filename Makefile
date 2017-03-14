CC := g++
# CC=clang++ --analyze

SRCDIR := example
BUILDDIR := build
TARGET := bin/httpServer

SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -g -w -std=c++11
INC := -I include
BOOST_INC := -I/usr/include/boost
LIB := -L/usr/lib -lboost_system -pthread

$(TARGET) : $(OBJECTS)
	@echo " Linking "
	@mkdir -p bin
	@echo " $(CC) $^ -o $(TARGET) $(LIB) $(LINKER) $(BOOST_INC)"; $(CC) $^ -o $(TARGET) $(LIB) $(BOOST_INC)

$(BUILDDIR)/%.o : $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) $(LIB) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<


clean:
	@echo " Cleaning... "
	@echo " rm -rf $(BUILDDIR) $(TARGET)"; rm -rf $(BUILDDIR) $(TARGET)


.PHONY: clean

