CC = cc
CFLAGS = -std=c99 \
				 -Wall \
				 -Wextra \
				 -Werror \
				 -MMD \
				 -D_DEFAULT_SOURCE
INCLUDE = -I include \
					-I thirdparty/criterion/include
LFLAGS = -lcriterion \
				 -L thirdparty/criterion/lib \
				 -Wl,-rpath=thirdparty/criterion/lib

DEBUG_SOURCE_FILE = src/debug.c
DEBUG_OBJECT_FILE = build/src/debug.o
DEBUG_TARGET = build/debug.out

MAIN_SOURCE_FILE = src/main.c
MAIN_OBJECT_FILE = build/src/main.o
SOURCE_FILES = $(wildcard src/**/*.c)
SOURCE_OBJECT_FILES = $(patsubst src/%.c,build/src/%.o,$(SOURCE_FILES))
SOURCE_DEPS_FILES = $(SOURCE_OBJECT_FILES:.o=.d)
SOURCE_TARGET = build/source.out

UTEST_OBJECT_SOURCE_FILES = $(wildcard build/**/**/*.o)
UTEST_FILES = $(wildcard tests/unit/**/*.c)
UTEST_OBJECT_FILES = $(patsubst tests/unit/%.c,build/tests/unit/%.o,$(UTEST_FILES))
UTEST_DEPS_FILES = $(UTEST_OBJECT_FILES:.o=.d)
UTEST_TARGET = build/test.out


.PHONY: all run clean

all: debug

run:
	@$(SOURCE_TARGET)

utest:
	@$(UTEST_TARGET)

utest-v:
	@$(UTEST_TARGET) --verbose=1

clean:
	$(RM) $(shell find build -type f ! -name '.gitkeep')

nolog: CFLAGS += -DNODEBUG -DNOLOG
nolog: LFLAGS += -O3 -s
nolog: $(SOURCE_TARGET)

release: CFLAGS += -DNODEBUG
release: LFLAGS += -O3 -s
release: $(SOURCE_TARGET)

debug: CFLAGS += -g -fsanitize=address,undefined
debug: LFLAGS += -O0
debug: $(SOURCE_TARGET) $(DEBUG_TARGET) $(UTEST_TARGET)


-include $(SOURCE_DEPS_FILES)

build/src/%.o: src/%.c
	@mkdir -p $(dir $@)
	@echo "[src] $< : $@"
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(MAIN_OBJECT_FILE): $(MAIN_SOURCE_FILE)
	@echo "[src] $< : $@"
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(SOURCE_TARGET): $(SOURCE_OBJECT_FILES) $(MAIN_OBJECT_FILE)
	@echo "[src] $@"
	@$(CC) $(CFLAGS) $(LFLAGS) $^ -o $@

# debug
$(DEBUG_OBJECT_FILE): $(DEBUG_SOURCE_FILE)
	@echo "[src] $< : $@"
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(DEBUG_TARGET): $(SOURCE_OBJECT_FILES) $(DEBUG_OBJECT_FILE)
	@echo "[src] $@"
	@$(CC) $(CFLAGS) $(LFLAGS) $^ -o $@



# unit tests build targets
build/tests/unit/%.o: tests/unit/%.c
	@mkdir -p $(dir $@)
	@echo "[utest] $< : $@"
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(UTEST_TARGET): $(SOURCE_OBJECT_FILES) $(UTEST_OBJECT_FILES)
	@echo "[utest] $@"
	@$(CC) $(CFLAGS) $(LFLAGS) $^ -o $@

