
INCS    := -I ../src
CWARNS  := -Wall -Wextra -Wno-unused-label
CC      := gcc
CFLAGS  := $(CWARNS) $(INCS)
#LDFLAGS := -L ../src -lluajit -lm -ldl
LDFLAGS := ../src/libluajit.a -lm -ldl

PROVE   := prove
PROVE   += $(if $(VERBOSE), --verbose,)

TESTS   := $(wildcard *.c)

ifeq ($(VERBOSE),1)
QUIET   :=
ECHO    :=@:
else
QUIET   :=@
ECHO    :=@echo
endif

.PHONY: build
build: $(TESTS:.c=.out)

.PHONY: test
test: build
	$(PROVE) --exec '' ./*.out

.PHONY: clean
clean:
	rm -f *.out

.PHONY: realclean
realclean: clean
	rm -f *.dep

%.out: %.c
	$(ECHO) "CCLD    $@"
	$(QUIET)$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

%.dep: %.c
	$(ECHO) "DEPEND  $@"
	$(QUIET)$(CC) $(INCS) -MM $< -MP -MT $(basename $<).out -MT $@ > $@

ifneq ($(MAKECMDGOALS),realclean)
include $(TESTS:.c=.dep)
endif

