rcp_extern rcp_command_type_t rcp_command_from_str(const char* str);

typedef struct rcp_connection rcp_connection;

typedef uint32_t rcp_login_id;
struct rcp_command_t{
	rcp_command_type_t type;
	rcp_connection *con;
	//forrowed by command specific paramaters
};

struct rcp_short_command_t{
	uint8_t len;				//	1byte
	rcp_command_type_t type;		//	1byte
	rcp_login_id login_id;		//	4byte
};


//command

//connection
//	userID
//	loginID
//	context	

///	server_states
//	user_database
//	type_manager
//	context_manager
//	connection_manager
//		epoll
