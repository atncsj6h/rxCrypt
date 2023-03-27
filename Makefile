# A make file that should work on most Unix-like platforms

LIBP = lib
LIB1 = rxcrypt

REXX_CFLAGS := $(shell oorexx-config  --cflags)

VERSION = -DVERSION_STRING="\"1.0.0-arm64\""
SHABITS = -DENABLE_SHA384 -DENABLE_SHA224
OTHR_CFLAGS  = -fPIC -Isrc/include -Ilib/include $(VERSION) $(SHABITS)


REXX_LFLAGS := $(shell oorexx-config --libs)
REXX_RPATH  := $(shell oorexx-config --rpath)

SRCS := src/rxcrypt.cpp lib/aes.c lib/cbc.c lib/ctr.c lib/ecb.c lib/md5.c lib/ofb.c lib/sha1.c lib/sha2.c
OBJS := $(addsuffix .o,$(basename $(SRCS)))

UNAME := $(shell uname -s)
ifeq ($(UNAME),Darwin)
	EXT = dylib
	OTHR_LFLAGS := -dynamiclib
else
  EXT = so
  OTHR_LFLAGS := -shared -export-dynamic -nostartfiles
endif

CPPFLAGS = $(REXX_CFLAGS) $(OTHR_CFLAGS)

# What we want to build.
all: $(LIBP)$(LIB1).$(EXT)

$(LIBP)$(LIB1).$(EXT): $(OBJS)
	$(CC) $(OBJS) -o $@ $(REXX_LFLAGS) $(OTHR_LFLAGS)

.PHONY: clean
clean:
	$(RM) $(LIBP)$(LIB1).$(EXT) $(OBJS) $(DEPS)
