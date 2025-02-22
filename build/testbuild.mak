
WANT_INTERFACE=NO
-include $(build_path)/basicbuild.mak
WANT_INTERFACE=	

ifeq ($(TARGET),)
$(error "TARGET" is not defined)
endif

target_folder= $(src_path)/$(TARGET)
target_tmp_folder= $(tmp_path)/$(TARGET)

target_tmp_test_folders= $(patsubst $(src_path)/%, $(tmp_path)/%, $(target_test_folders))

target_srcunit_folders_abs= $(shell make -C $(target_folder) -f zMakefile.mak get TARGET=srcunit_folders_abs --no-print-directory) 
target_test_folders= $(foreach folder, $(target_srcunit_folders_abs), $(wildcard $(folder)/test))
target_test_cfiles= $(foreach folder, $(target_test_folders), $(wildcard $(folder)/*.c))
target_test_ofiles= $(patsubst $(src_path)/%.c, $(tmp_path)/%.o, $(target_test_cfiles))
target_test_dfiles= $(patsubst %.o, %.d, $(target_test_ofiles))

target_srcunit_all_ofiles= $(shell make -C $(target_folder) -f zMakefile.mak get TARGET=srcunit_all_ofiles --no-print-directory) 

target_all_ofiles= $(target_srcunit_all_ofiles) $(target_test_ofiles)

target_sharedlib_file= $(bin_path)/test_$(TARGET).so
srcunit_exefile= $(bin_path)/test_$(TARGET)

#------------------------------------------------------------------------
ifeq ($(WANT_INTERFACE),)
.PHONY:  run_exe build_exe clean print

run_exe: build_exe run_srcunit_exe_w_target

build_exe: build_target build_srcunit_exe

clean: clean_target clean_srcunit

print: print_srcunit print_test

endif
#------------------------------------------------------------------------
.PHONY: build_target build_target_ofiles build_exe
run_srcunit_exe_w_target: 
	@$(srcunit_exefile)  $(shell nm -D --defined $(target_sharedlib_file) | awk '{ print $$3}')

build_target: build_target_ofiles build_srcunit_ofiles
	@gcc -g -shared $(target_all_ofiles) -o $(target_sharedlib_file) $(lib_folders_proc) $(lib_names_proc)

build_target_ofiles: $(target_test_ofiles) 
	@make -C $(target_folder) -f zMakefile.mak build_srcunit_ofiles --no-print-directory

$(target_test_ofiles): $(srcunit_folder)/test.h

$(target_tmp_folder)/%.o: $(target_folder)/%.c
	@echo "-->" $(patsubst $(src_path)/%, %, $<)
	@mkdir -p $(dir $@)
	@gcc -g -fPIC -c $< -o $@ $(include_folders_proc) -I$(srcunit_folder)

clean_target:
	@rm $(target_sharedlib_file) 2> /dev/null || true
	@rm -r $(target_tmp_test_folders) 2> /dev/null || true

-include $(target_test_dfiles)


print_test:
	@echo $(MAKELEVEL)
	@echo -- $(MAKEFILE_LIST)
	$(call print_var, target_srcunit_folders_abs, $(target_srcunit_folders_abs))
	$(call print_var, target_test_folders, $(target_test_folders))
	$(call print_var, target_test_cfiles, $(target_test_cfiles))
	$(call print_var, target_test_ofiles, $(target_test_ofiles))
	$(call print_var, target_test_dfiles, $(target_test_dfiles))
	$(call print_var, target_all_ofiles, $(target_all_ofiles))
	$(call print_var, target_tmp_test_folders, $(target_tmp_test_folders))


