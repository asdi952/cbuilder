
basefolder= $(notdir $(CURDIR))

srcunit_folder= $(CURDIR)
srcunit_tmp_folder= $(tmp_path)/$(basefolder)

srcunit_folders_abs= $(subst /.,, $(patsubst %, $(CURDIR)/%, $(src_folders)))

srcunit_cfiles= $(foreach folder, $(srcunit_folders_abs), $(wildcard $(folder)/*.c))
srcunit_ofiles= $(patsubst $(src_path)/%.c, $(tmp_path)/%.o, $(srcunit_cfiles))
srcunit_dfiles= $(patsubst %.o, %.d, $(srcunit_ofiles))


include_srcunits_folders_abs= $(patsubst %, $(src_path)/%, $(include_srcunits_folders))
join_srcunits_ofiles = $(foreach srcunit, $(join_srcunits_builds), $(shell make -C $(src_path)/$(srcunit) -f zMakefile.mak get TARGET=srcunit_all_ofiles --no-print-directory))

srcunit_all_include_folders= $(sort $(include_folders) $(include_srcunits_folders_abs))

include_folders_proc= $(patsubst %, -I%, $(srcunit_all_include_folders))
lib_folders_proc= $(patsubst %, -L%, $(lib_folders))
lib_names_proc= $(patsubst %, -l%, $(lib_names))

srcunit_all_ofiles= $(sort $(srcunit_ofiles) $(join_srcunits_ofiles))

srcunit_exefile= $(bin_path)/$(exename)

#------------------------------------------------------------------------
ifeq ($(WANT_INTERFACE),)
.PHONY: run_exe build_exe clean print

run_exe: run_srcunit_exe

build_exe: build_srcunit_exe

clean: clean_srcunit

print: print_srcunit

endif
#------------------------------------------------------------------------
.PHONY: run_srcunit_exe build_srcunit_exe build_srcunit_ofiles clean_srcunit print_srcunit

run_srcunit_exe: build_srcunit_exe
	@$(srcunit_exefile) 

build_srcunit_exe: build_srcunit_ofiles
	@gcc -g $(srcunit_all_ofiles) -o $(srcunit_exefile) $(lib_folders_proc) $(lib_names_proc)

build_srcunit_ofiles: $(srcunit_ofiles) 
	@for srcunit in $(join_srcunits_builds); do \
		make -C $(src_path)/$$srcunit -f zMakefile.mak build_srcunit_ofiles --no-print-directory; \
	done


$(srcunit_tmp_folder)/%.o: $(srcunit_folder)/%.c
	@echo "-->" $(patsubst $(src_path)/%, %, $<)
	@mkdir -p $(dir $@)
	@gcc -g -MMD -MP -c $< -o $@ $(include_folders_proc) 

clean_srcunit: $(clean_deps)
	@rm $(srcunit_exefile) 2> /dev/null || true
	@rm $(srcunit_tmp_folder)/* 2> /dev/null || true

-include $(srcunit_dfiles)
#------------------------------------------------------------------------
define print_var
	@echo $1: $(patsubst %, "\n\t"%, $(2))
endef

print_srcunit:
	$(call print_var, src_folders_abs, $(srcunit_folders_abs))
	$(call print_var, srcunit_cfiles, $(srcunit_cfiles))
	$(call print_var, srcunit_ofiles, $(srcunit_ofiles))
	$(call print_var, srcunit_dfiles, $(srcunit_dfiles))
	$(call print_var, srcunit_all_include_folders, $(srcunit_all_include_folders))
	$(call print_var, include_folders_proc, $(include_folders_proc))
	$(call print_var, join_srcunits_ofiles, $(join_srcunits_ofiles))

included:
	@echo $(word 1, $(MAKEFILE_LIST) )
	

get:
	@echo $($(TARGET))

