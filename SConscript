env = Environment()
Import('env')

types = [
	'types/rcp_array.c',
	'types/rcp_array_list.c',
	'types/rcp_string.c',
	'types/rcp_type_list.c',
	'types/rcp_type_utility.c',
	'types/rcp_dict.c',
	'types/rcp_dict_list.c',
	'types/rcp_number.c',
	'types/rcp_struct.c',
	'types/rcp_type_etc.c',
	'types/rcp_alias.c',
	]

connections = [
	'connections/con_file.c',
	'connections/con_null_terminate.c',
	'connections/con_json.c',
	'connections/con_plain.c',
	'connections/con_web_socket.c',
	'connections/con_pgsql_lo.c',
	'connections/con_dummy.c',
	]

commands = [
	'cmd_impl.c',
	'cmd_table.c',
	'cmd_types.c',
	]


mainFiles = [
	'rcp_buffer.c',
	'rcp_command.c',
	'rcp_command_type.c',
	'rcp_connection.c',
	'rcp_context.c',
	'rcp_context_file.c',
	'rcp_context_pgsql.c',
	'rcp_logger_pgsql.c',
	'rcp_event_epoll.c',
	'rcp_event_kqueue.c',
	'rcp_json.c',
	'rcp_json_write.c',
	'rcp_send_as_command.c',
	'rcp_listener.c',
	'rcp_timeout.c',
	'rcp_receiver.c',
	'rcp_receiver_classes.c',
	'rcp_sender.c',
	'rcp_sender_classes.c',
	'rcp_server.c',
	'rcp_ssl.c',
	'rcp_tree.c',
	'rcp_record.c',
	'rcp_caster.c',
	'rcp_type.c',
	'rcp_user.c',
	'rcp_utility.c',
	'rcp_user_pgsql.c',
	'rcp_time.c',
]

mainFiles.extend(types)
mainFiles.extend(connections)
mainFiles.extend(commands)

tests = [
	'tests/test_json.cpp',
	'tests/test_tree.cpp',
	'tests/test_array.cpp',
	'tests/test_user.cpp',
	'tests/test_at.cpp',
]

main = ['rcp_main.c']
main.extend(mainFiles)

env.Program(target = 'main', source = main)

mainFiles.extend(tests)
main = ['tests/test_main.cpp']
main.extend(mainFiles)
env.Program(target = 'test', source = main)

