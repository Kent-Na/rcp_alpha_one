
compile_source_message = '\033[34mCompiling > \033[0m$SOURCE'
link_program_message = '\033[34mLinking > \033[0m$TARGET'

env = Environment(
	CXXCOMSTR = compile_source_message,
	CCCOMSTR = compile_source_message,
#ARCOMSTR = link_library_message,
#	RANLIBCOMSTR = ranlib_library_message,
#	SHLINKCOMSTR = link_shared_library_message,
	LINKCOMSTR = link_program_message,
)

Export('env')

SConscript('SConscript', variant_dir='build', duplicate=0)

