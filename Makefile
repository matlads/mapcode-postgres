EXTENSION  = $(shell grep -m 1 '"name":' META.json | \
             sed -e 's/[[:space:]]*"name":[[:space:]]*"\([^"]*\)",/\1/')
EXTVERSION = $(shell grep -m 1 '[[:space:]]\{8\}"version":' META.json | \
             sed -e 's/[[:space:]]*"version":[[:space:]]*"\([^"]*\)",\{0,1\}/\1/')

DATA       = $(wildcard sql/*sql)
DOCS       = $(wildcard doc/*.$(EXTENSION))
MODULE_big = $(EXTENSION)
OBJS       = src/mapcoder/mapcoder.o src/funcs.o
PG_CONFIG ?= pg_config
PG91 = $(shell $(PG_CONFIG) --version | grep -qE " 8\.| 9\.0" && echo no || echo yes)

ifeq ($(PG91),yes)
DATA = $(wildcard sql/*--*.sql)
EXTRA_CLEAN = sql/$(EXTENSION)--$(EXTVERSION).sql
endif

PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)

ifeq ($(PG91),yes)
all: sql/$(EXTENSION)--$(EXTVERSION).sql
sql/$(EXTENSION)--$(EXTVERSION).sql: sql/$(EXTENSION).sql
	cp $< $@
endif

dist:
	git archive --format zip --prefix=$(EXTENSION)-$(EXTVERSION)/ -o $(EXTENSION)-$(EXTVERSION).zip HEAD
