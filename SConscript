env = Environment()

#env.Append(CCFLAGS = ['-O3'])
env.Append(CCFLAGS = ['-g'])
env.Append(CXXFLAGS = ['-std=gnu++0x'])

mainFiles = [
	'rcp_pch.h',
	'rcp_main.c',
	'rcp_command.c',
	'rcp_command_list.c',
	'rcp_epoll.c',
	'rcp_connection.c',
	'rcp_utility.c',
	'rcp_tree.c',
	'rcp_server.c',
	'rcp_string.c',
	'rcp_struct.c',
	'rcp_context.c',
	'rcp_type_list.c',
	'con_plain.c',
	'con_null_terminate.c',
	'con_json_cpp.cpp',
	'json/jsoncpp.cpp',
]

testFiles = [
	'rcp_pch.h',
	'rcp_json.c',
	'rcp_type_list.c',
	'rcp_tree.c',
	'rcp_utility.c',
	'rcp_string.c',
	'rcp_command.c',
	'rcp_command_list.c',
	'rcp_struct.c',
	'test_json.cpp',
	'test_tree.cpp',
	'test_main.cpp',
]

env.Program(target = 'main', source = mainFiles)
env.Program(target = 'test', source = testFiles)

