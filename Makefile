
ifeq ($(STAMPFILE),)
STAMPFILE := .stamp
endif

MD := mkdir -v -p

# === Convert Utility ===

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

# === Headpack Utility ===

HEADPACK_C_FILES_BASE := \
   unilayer/tools/headpack.c \
   unilayer/tools/data/bmp.c \
   unilayer/tools/data/cga.c \
   unilayer/tools/data/icns.c \
   unilayer/src/resource/file.c \
   unilayer/src/graphics.c \
   unilayer/src/graphics/nullg.c \
   unilayer/src/memory/fakem.c \
   unilayer/src/dio.c

HEADPACK := $(BINDIR)/headpack

CFLAGS_HEADPACK := -g -Iunilayer/src -Iunilayer/tools -DNO_RESEXT -DDEBUG_THRESHOLD=3 -DRESOURCE_FILE -DASSETS_PATH="\"$(ASSETPATH)\"" -DDEBUG_LOG -DDISABLE_WEATHER_EFFECTS

# === Mkresh Utility ===

MKRESH_C_FILES := \
   unilayer/tools/mkresh.c \
   unilayer/src/resource/file.c \
   unilayer/src/memory/fakem.c \
   unilayer/src/dio.c

MKRESH := $(BINDIR)/mkresh

CFLAGS_MKRESH := -DNO_RESEXT -g -DDEBUG_LOG -DDEBUG_THRESHOLD=0 -DRESOURCE_FILE -Iunilayer/src -DASSETS_PATH="\"$(ASSETPATH)\""

$(MKRESH): $(MKRESH_C_FILES) | $(BINDIR)/$(STAMPFILE)
	$(HOST_CC) $(CFLAGS_MKRESH) -o $@ $^

# === Tests ===

TESTS_C_FILES := \
   unilayer/src/asn.c \
   unilayer/src/memory/fakem.c \
   unilayer/tests/check.c \
   unilayer/tests/chkasn.c

CFLAGS_TESTS := -DNO_RESEXT -DDEBUG_LOG -DDEBUG_THRESHOLD=3

test_unilayer: $(addprefix obj/tests/,$(subst .c,.o,$(TESTS_C_FILES)))
	$(MD) $(dir $@)
	$(CC) -o $@ $^ $(shell pkg-config --libs check)

obj/tests/%.o: %.c
	$(MD) $(dir $@)
	$(CC) -c -o $@ $(CFLAGS_TESTS) $<

