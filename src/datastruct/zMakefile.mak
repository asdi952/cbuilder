
#----------------------------------------------------------------------------------
export src_folders= .

export include_folders=
export lib_folders=
export lib_names=

export include_srcunits_folders= utils
export join_srcunits_builds=

export exename= datastruct

build_makefile_name= basicbuild

#----------------------------------------------------------------------------------
%:
	@make -C . -f $(build_path)/$(build_makefile_name).mak $@ --no-print-directory
	