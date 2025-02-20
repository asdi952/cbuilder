
#----------------------------------------------------------------------------------
export src_folders= .

export include_folders=
export lib_folders=
export lib_names=

export test_include_folders=
export test_lib_folders=
export test_lib_names=

export include_srcunits_folders= utils datastruct 
export join_srcunits_builds= datastruct

build_makefile_name= testbuild

#----------------------------------------------------------------------------------
%:
	@make  -C . -f $(build_path)/$(build_makefile_name).mak $@ --no-print-directory 
	