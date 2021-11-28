
ifeq ($(STAMPFILE),)
STAMPFILE := .stamp
endif

CONVERT_C_FILES := \
   unilayer/tools/convert.c \
   unilayer/src/memory/fakem.c \
   unilayer/tools/data/bmp.c \
   unilayer/tools/data/cga.c \
   unilayer/tools/data/icns.c \
   unilayer/src/dio.c

CONVERT := $(BINDIR)/convert

CFLAGS_CONVERT := -DNO_RESEXT -g -DRESOURCE_FILE -Iunilayer/src

$(CONVERT): $(CONVERT_C_FILES) | $(BINDIR)/$(STAMPFILE)
	$(HOST_CC) $(CFLAGS_CONVERT) -o $@ $^

