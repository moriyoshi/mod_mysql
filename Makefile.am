SUBDIRS=lib server modules
DOXYGEN=@DOXYGEN@

doc:
	[ ! -z $(DOXYGEN) ] && $(DOXYGEN) doxyconfig || \
		echo "Please install Doxygen." 2>&1

.PHONY: doc
