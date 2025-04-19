# Get all subdirectories
COMMON = libs/. include/.
SUBDIRS := $(filter-out $(COMMON), $(wildcard */.))
ACTIONS = build debug static clean
# SUBDIRS := $(filter-out libs/. include/., $(wildcard */.))

.PHONY: $(SUBDIRS) $(SUBDIRS:%=%-target) $(ACTIONS)

$(SUBDIRS):
	@$(MAKE) -C $@ $(word 2, $(MAKECMDGOALS))

%-target:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir $*; \
	done

build debug static clean:
	@$(MAKE) $@-target

%:
	@:
