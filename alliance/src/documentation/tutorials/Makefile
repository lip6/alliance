ALL_DIRS = simulation/src simulation/tex synthesis/src synthesis/tex place_and_route/src place_and_route/tex



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
