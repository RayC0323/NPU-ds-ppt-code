CC ?= gcc
CFLAGS ?= -std=c11 -Wall -Wextra -pedantic

TOPIC_SRCS := $(wildcard DS-Chap*/*.c)
CHAPTER_SRCS := $(wildcard src/DS-Chap*.c)

TOPIC_BINS := $(patsubst %.c,build/%,$(TOPIC_SRCS))
CHAPTER_BINS := $(patsubst src/%.c,build/src/%,$(CHAPTER_SRCS))

.PHONY: all topics chapters run clean list

all: topics chapters

topics: $(TOPIC_BINS)

chapters: $(CHAPTER_BINS)

build/%: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@

build/src/%: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@

run: topics
	@for exe in $(TOPIC_BINS); do \
		echo "== $$exe =="; \
		./$$exe; \
	done

list:
	@printf "%s\n" $(TOPIC_SRCS)

clean:
	rm -rf build

