ALL_DIRS = amd2901 amdbug meter5 digicode

all :
	@for FILE in $(ALL_DIRS)                ;\
        do $(MAKE) -C $$FILE                    ;\
        if [ $$? -ne 0 ]                        ;\
          then  exit 1                                  ;\
        fi                                      ;\
        done

% :
	@for FILE in $(ALL_DIRS)                ;\
        do $(MAKE) -C $$FILE  $@                ;\
        if [ $$? -ne 0 ]                        ;\
          then  exit 1                                  ;\
        fi                                      ;\
        done
