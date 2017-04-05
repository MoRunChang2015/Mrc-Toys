CC := g++
# CC=clang++ --analyze

SRCDIR := src
BUILDDIR := build
TESTDIR := test
TARGET := bin/MD5
TARGETDIR := bin

SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -g -w -std=c++11
INC := -I include
STATIC_ANALYZER := oclint
TESTLIB := -lgtest -lgtest_main -lpthread

$(TARGET) : $(OBJECTS)
	@echo " Linking "
	@mkdir -p bin
	@echo " $(CC) $^ -o $(TARGET) $(LIB) $(LINKER)"; $(CC) $^ -o $(TARGET)

$(BUILDDIR)/%.o : $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) $(LIB) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

$(TESTDIR)/md5EncryptTest : $(BUILDDIR)/md5EncryptTest.o $(BUILDDIR)/MD5.o
	@echo " Linking "
	@mkdir -p bin
	@echo " $(CC) -o bin/md5EncryptTest $^ $(TESTLIB)"; $(CC) $^ -o bin/md5EncryptTest $(TESTLIB)
	@echo " running "; bin/md5EncryptTest

$(BUILDDIR)/md5EncryptTest.o: $(TESTDIR)/md5EncryptTest.cpp
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) $(LIB) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) $(LIB) -c -o $@ $<

static-analyze:
	$(STATIC_ANALYZER) $(SOURCES) -- -c $(CFLAGS) $(INC) $(LIB)

clean:
	@echo " Cleaning... "
	@echo " rm -rf $(BUILDDIR) $(TARGETDIR)/*"; rm -rf $(BUILDDIR) $(TARGETDIR)/*


.PHONY: clean
