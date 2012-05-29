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
	'rcp_type.c',
	'rcp_type_number.c',
	'rcp_type_etc.c',
	'rcp_map.c',
	'rcp_string_map.c',
	'rcp_array.c',
	'rcp_json.c',
	'con_plain.c',
	'con_null_terminate.c',
	'con_json.c',
]

testFiles = [
	'rcp_pch.h',
	'rcp_json.c',
	'rcp_type_number.c',
	'rcp_type.c',
	'rcp_tree.c',
	'rcp_map.c',
	'rcp_utility.c',
	'rcp_string.c',
	'rcp_command.c',
	'rcp_command_list.c',
	'rcp_struct.c',
	'test_json.cpp',
	'test_tree.cpp',
	'test_main.cpp',
	'test_map.cpp',
	'rcp_array.c',
]

env.Program(target = 'main', source = mainFiles)
env.Program(target = 'test', source = testFiles)

