CC = cc
CFLAGS = -std=c99 -Wall -Wextra -Werror -MMD -D_DEFAULT_SOURCE
INCLUDE = -I include
LFLAGS =

SOURCE_FILES = $(wildcard src/*.c)
OBJECT_FILES = $(patsubst src/%.c,build/%.o,$(SOURCE_FILES))
DEPS_FILES = $(OBJECT_FILES:.o=.d)
TARGET_BIN = build/target.out


.PHONY: all run clean

all: debug

run:
	@$(TARGET_BIN)

clean:
	$(RM) $(wildcard build/*)


release: CFLAGS += -DNODEBUG
release: LFLAGS += -O3 -s
release: $(TARGET_BIN)

#-fsanitize=leak
debug: CFLAGS += -g -fsanitize=address,undefined
debug: LFLAGS += -O0
debug: $(TARGET_BIN)



-include $(DEPS_FILES)

build/%.o: src/%.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(TARGET_BIN): $(OBJECT_FILES)
	$(CC) $(CFLAGS) $(LFLAGS) $^ -o $@


