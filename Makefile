BUILDDIR := build
TARGET := bin/httpServer bin/httpsServer

all:
	@make http
	@make https

http:
	@make -f Makefile_http

.PHONY: http

https:
	@make -f Makefile_https

.PHONY: https

clean:
	@echo " Cleaning... "
	@echo " rm -rf $(BUILDDIR) $(TARGET)"; rm -rf $(BUILDDIR) $(TARGET)


.PHONY: clean

