ALL_DIRS = addaccu_beh addaccu_struct

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
