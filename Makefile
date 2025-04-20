# Get all subdirectories
COMMON = libs/. include/.
SUBDIRS := $(patsubst %/., %, $(filter-out $(COMMON), $(wildcard */.)))
ACTIONS = build debug clean test

.PHONY: $(SUBDIRS) $(ACTIONS)

$(SUBDIRS):
	@$(MAKE) -C $@ $(word 2, $(MAKECMDGOALS))

ifeq ($(filter $(firstword $(MAKECMDGOALS)), $(SUBDIRS)),)
$(ACTIONS):
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir $@; \
	done
else
$(ACTIONS):
	@:
endif

%:
	@:
