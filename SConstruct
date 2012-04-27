env = Environment()
VariantDir('build/','./')

#env.Append(CCFLAGS = ['-O3'])
env.Append(CCFLAGS = ['-g'])

cFiles = [
	'rcp_pch.h',
	'rcp_command.c',
	'rcp_command_list.c',
	'rcp_epoll.c',
	'rcp_connection.c',
	'rcp_test.c',
	'rcp_utility.c',
	'con_plain.c',
	'con_null_terminate.c',
	'con_json_cpp.cpp',
	'json/jsoncpp.cpp',
]

env.Program(target = 'test', source = cFiles)
