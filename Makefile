CC = gcc

CFLAGS = -Wall -Wextra -pedantic -ansi -g3
LDFLAGS = -lm

INCLUDES = -Iinclude
LIBS = sdl2 SDL2_image

SOURCEDIR = src
BUILDDIR = build
BINDIR = bin
PROGRAM = mutant

#--- DO NOT CHANGE ANYTHING BELOW ---#

ifdef LIBS
CFLAGS += `pkg-config $(LIBS) --cflags`
LDFLAGS += `pkg-config $(LIBS) --libs`
endif

SOURCES := $(wildcard $(SOURCEDIR)/*.c)
OBJECTS := $(SOURCES:$(SOURCEDIR)/%.c=$(BUILDDIR)/%.o)
DEPENDENCIES := $(OBJECTS:.o=.d)
EXECUTABLE = $(BINDIR)/$(PROGRAM)

.PHONY: all clean
.SUFFIXES: .c .o .d

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	@mkdir -p $(@D)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

-include $(DEPENDENCIES)

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
	@$(CC) $(CFLAGS) $(INCLUDES) -MM $< > $(@:.o=.d)
	@mv -f $(@:.o=.d) $(@:.o=.d).tmp
	@sed -e 's|$(@F):|$@:|' < $(@:.o=.d).tmp > $(@:.o=.d)
	@rm -f $(@:.o=.d).tmp

clean:
	$(RM) $(OBJECTS) $(DEPENDENCIES) $(EXECUTABLE)
