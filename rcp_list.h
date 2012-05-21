
typedef rcp_list_ref *struct rcp_list_core;

struct rcp_list_core{
	rcp_list_ref next;
};


rcp_list_ref rcp_list_new(size_t s);
void rcp_list_add_front(rcp_list_ref ,rcp_list_ref );
