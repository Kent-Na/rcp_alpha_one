from colorizer import colorizer
env = Environment()

col = colorizer()
col.colorize(env)

#env.Append(CCFLAGS = ['-O3'])
env.Append(CCFLAGS = ['-Wall','-g'])
env.Append(CFLAGS = ['-Wno-pointer-sign'])
env.Append(CXXFLAGS = ['-std=gnu++0x'])
env.Append(LIBS = ['ssl'])

mainFiles = [
	'rcp_main.c',
	'rcp_command.c',
	'rcp_command_list.c',
	'rcp_epoll.c',
	'rcp_io.c',
	'rcp_sender.c',
	'rcp_receiver.c',
	'rcp_connection.c',
	'rcp_listener.c',
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
	'rcp_buffer.c',
	'con_plain.c',
	'con_null_terminate.c',
	'con_web_socket.c',
	'con_ssl.c',
	'con_json.c',
	'cmd_types.c'
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

