#include "rcp_pch.h"
#include "rcp_defines.h"
#include "rcp_utility.h"
#include "rcp_type.h"
#include "rcp_io.h"
#include "rcp_receiver.h"
#include "rcp_receiver_classes.h"

#include "rcp_sender.h"
#include "rcp_buffer.h"

#include "con_null_terminate.h"
#include "con_web_socket.h"
#include "con_json.h"

///
//receiver
//

struct rcp_receiver_class 
	con_nt_json_class = {
	//l2 null terminate
	sizeof (struct con_null_terminate),
	CON_NULL_TERMINAE_CLASS_PART,
	//l3 json (no data)
	0,
	CON_JSON_CLASS_PART
};

struct rcp_receiver_class
	con_ws_json_class = {
	//l2 null terminate
	sizeof (struct con_web_socket),
	CON_WEB_SOCKET_CLASS_PART,
	//l3 json (no data)
	0,
	CON_JSON_CLASS_PART
};

