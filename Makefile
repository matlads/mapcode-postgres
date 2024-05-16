EXTENSION    = mapcode
EXTVERSION   = $(shell grep -m 1 '"version":' META.json | \
               sed -e 's/[[:space:]]*"version":[[:space:]]*"\([^"]*\)",/\1/')
DATA         = $(filter-out $(wildcard sql/*--*.sql),$(wildcard sql/*.sql))
TESTS        = $(wildcard test/sql/*.sql)
REGRESS      = $(patsubst test/sql/%.sql,%,$(TESTS))
REGRESS_OPTS = --inputdir=test --load-language=plpgsql
DOCS         = $(wildcard doc/*md)
MODULE_big   = $(EXTENSION)
OBJS         = src/mapcode-cpp/mapcodelib/mapcoder.o src/funcs.o
PG_CONFIG   ?= pg_config
PG91         = $(shell $(PG_CONFIG) --version | grep -qE " 8\.| 9\.0" && echo no || echo yes)

ifeq ($(PG91),yes)
all: sql/$(EXTENSION)--$(EXTVERSION).sql

sql/$(EXTENSION)--$(EXTVERSION).sql: sql/$(EXTENSION).sql
	cp $< $@

DATA = $(wildcard sql/*--*.sql) sql/$(EXTENSION)--$(EXTVERSION).sql
EXTRA_CLEAN = sql/$(EXTENSION)--$(EXTVERSION).sql
endif

PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)

dist:
	git archive --format zip --prefix $(EXTENSION)-$(EXTVERSION)/  -o $(EXTENSION)-$(EXTVERSION).zip HEAD
