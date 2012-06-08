env = Environment()

env.Append(CCFLAGS = ['-O3'])
env.Append(CCFLAGS = ['-Wall','-g'])
env.Append(CXXFLAGS = ['-std=gnu++0x'])
env.Append(LIBS = ['ssl'])

mainFiles = [
	'rcp_main.c',
	'rcp_command.c',
	'rcp_command_list.c',
	'rcp_epoll.c',
	'rcp_connection.c',
	'rcp_connection_builder.c',
	'rcp_utility.c',
	'rcp_tree.c',
	'rcp_server.c',
	'rcp_string.c',
	'rcp_struct.c',
	'rcp_context.c',
	'rcp_type.c',
	'rcp_number.c',
	'rcp_type_etc.c',
	'rcp_type_list.c',
	'rcp_type_utility.c',
	'rcp_map.c',
	'rcp_array.c',
	'rcp_json.c',
	'con_buffer.c',
	'con_plain.c',
	'con_null_terminate.c',
	'con_web_socket.c',
	'con_json.c',
]

testFiles = [
	'rcp_pch.h',
	'rcp_json.c',
	'rcp_number.c',
	'rcp_type.c',
	'rcp_type_etc.c',
	'rcp_type_list.c',
	'rcp_type_utility.c',
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
	'test_array.cpp',
	'test_pm.cpp',
	'rcp_array.c',
]

env.Program(target = 'main', source = mainFiles)
env.Program(target = 'test', source = testFiles)

