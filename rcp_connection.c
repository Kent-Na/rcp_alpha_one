#include "rcp_pch.h"
#include "rcp_defines.h"
#include "rcp_epoll.h"
#include "rcp_connection.h"
#include "rcp_utility.h"
#include "con_plain.h"
#include "con_null_terminate.h"
#include "con_json_cpp.h"


struct rcp_connection_core{
	struct rcp_connection_class *klass;
	uint32_t loginID;
	uint32_t userID;
};

void rcp_connection_core_init(
		void *state, struct rcp_connection_class* klass)
{
	struct rcp_connection_core *st = state;
	st->klass = klass;
	st->loginID = 0;
	st->userID = 0;
}

rcp_connection_ref rcp_connection_new(struct rcp_connection_class* klass)
{
	size_t l1_data_offset = sizeof (struct rcp_connection_core);
	size_t l2_data_offset = l1_data_offset + klass->layer1_data_size;
	size_t l3_data_offset = l2_data_offset + klass->layer2_data_size;
	size_t total_size = l3_data_offset + klass->layer3_data_size;

	rcp_connection_ref con = malloc(total_size);

	struct rcp_connection_core *core = (void*)con;
	void *l1_state = con + l1_data_offset;
	void *l2_state = con + l2_data_offset;
	void *l3_state = con + l3_data_offset;

	rcp_connection_core_init(core, klass);
	if (klass ->l1.init)
		klass->l1.init(l1_state);
	if (klass ->l2.init)
		klass->l2.init(l2_state);
	if (klass ->l3.init)
		klass->l3.init(l3_state);

	return (rcp_connection_ref) con;
};

void rcp_connection_free(rcp_connection_ref con)
{
	struct rcp_connection_core *core = (void*)con;
	struct rcp_connection_class *klass = core->klass;

	size_t l1_data_offset = sizeof (struct rcp_connection_core);
	size_t l2_data_offset = l1_data_offset + klass->layer1_data_size;
	size_t l3_data_offset = l2_data_offset + klass->layer2_data_size;
	size_t total_size = l2_data_offset + klass->layer3_data_size;

	void *l1_state = con + l1_data_offset;
	void *l2_state = con + l2_data_offset;
	void *l3_state = con + l3_data_offset;

	if (klass ->l1.release)
		klass->l1.release(l1_state);
	if (klass ->l2.release)
		klass->l2.release(l2_state);
	if (klass ->l3.release)
		klass->l3.release(l3_state);
	free(con);
}


void rcp_connection_on_receive(rcp_connection_ref con)
{
	struct rcp_connection_core *core = (void*)con;
	struct rcp_connection_class *klass = core->klass;

	size_t l1_data_offset = sizeof (struct rcp_connection_core);
	size_t l2_data_offset = l1_data_offset + klass->layer1_data_size;
	size_t l3_data_offset = l2_data_offset + klass->layer2_data_size;
	size_t total_size = l2_data_offset + klass->layer3_data_size;

	void *l1_state = con + l1_data_offset;
	void *l2_state = con + l2_data_offset;
	void *l3_state = con + l3_data_offset;

	void* space = klass->l2.space_ptr(l2_state);
	size_t space_len = klass->l2.space_len(l2_state);

	//printf ("%p\n",con);
	//printf ("%p\n",space);

	size_t len = klass->l1.receive(l1_state, space, space_len);

	klass->l2.data_added(l2_state, len);

	void *cmd_begin;
	void *cmd_end;
	rcp_err err;
	while (1){
		err = klass->l2.next_command(l2_state, &cmd_begin, &cmd_end);
		if (err)
			break;
		//rcp_info(cmd);
		klass->l3.execute_command(con, cmd_begin, cmd_end);
	}

	klass->l2.clean_space(l2_state);
}

void rcp_connection_epoll_action(int epfd, struct epoll_event *ev,
		epoll_data_t data)
{
	if (ev->events & EPOLLIN){
		rcp_connection_on_receive((rcp_connection_ref)data.ptr);
	}
	if (ev->events & EPOLLRDHUP){
		rcp_connection_free((rcp_connection_ref)data.ptr);
	}
}



///
//layer 1
//	send/receive
//

//rcp_connection_plain has no data
typedef void SSL;
struct rcp_connection_ssl{
	int fd;
	SSL *ssl;
};

///
//con_plain

static struct rcp_connection_layer1_class_part
	con_plain_class_part = CON_PLAIN_CLASS_PART;

///
//layer 2
//	command separation
//

struct rcp_connection_layer2_class_part
con_null_terminate_class_part = CON_NULL_TERMINAE_CLASS_PART;

///
//layer 3
//	command execution 
//

///
//merged
//
struct con_plain_json{
	//struct rcp_connection_class *klass;

	struct rcp_connection_core core;
	//layer 1 (plain)
	struct con_plain l1;
	//layer 2 (null terminate)
	struct con_null_terminate l2;
	//layer 3 (json) (no state)
};


static struct rcp_connection_class 
	con_plain_json_class = {
	//l1 plain
	sizeof (struct con_plain),
	CON_PLAIN_CLASS_PART,
	//l2 null terminate
	sizeof (struct con_null_terminate),
	CON_NULL_TERMINAE_CLASS_PART,
	//l3 json (no data)
	0,
	CON_JSON_CPP_CLASS_PART
};

rcp_connection_ref rcp_connection_plain_json_new(int epfd, int fd)
{
	rcp_connection_ref con = rcp_connection_new(&con_plain_json_class);

	struct rcp_connection_core *core = (void*)con;
	//struct rcp_connection_class *klass = core->klass;

	size_t l1_data_offset = sizeof (struct rcp_connection_core);
	//size_t l2_data_offset = l1_data_offset + klass->layer1_data_size;
	//size_t l3_data_offset = l2_data_offset + klass->layer2_data_size;
	//size_t total_size = l2_data_offset + klass->layer3_data_size;

	void *l1_state = con + l1_data_offset;
	//void *l2_state = con + l2_data_offset;
	//void *l3_state = con + l3_data_offset;

	con_plain_set_fd(epfd, fd, con, l1_state);

	struct con_plain* plain_state = l1_state;
	plain_state->fd = fd;
}

struct rcp_epoll_action* rcp_listener_plain_json_new(int epfd)
{
	struct rcp_epoll_action* unit = malloc(sizeof *unit);
	unit->action= rcp_raw_json_listener_epoll_event;
	unit->userdata.fd = -1;

	struct sockaddr_in sockadd;
	bzero(&sockadd, sizeof sockadd);
	sockadd.sin_family = AF_INET;
	sockadd.sin_addr.s_addr = INADDR_ANY;
	sockadd.sin_port = htons(RCP_RAW_JSON_PORT_NUMBER);

	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == -1){
		rcp_raw_json_listener_release(unit);
		return NULL;
	}
	unit->userdata.fd = fd;

	int err;

	err = bind(fd, (struct sockaddr*)&sockadd, sizeof sockadd);
	if (err){
		rcp_raw_json_listener_release(unit);
		return NULL;
	}

	err = listen(fd, RCP_LISTEN_SOCKET_NUMBER);
	if (err){
		rcp_raw_json_listener_release(unit);
		return NULL;
	}

	// ! setup epoll event
	struct epoll_event ev;
	ev.events = EPOLLIN | EPOLLPRI | EPOLLERR | EPOLLHUP;
	ev.data.ptr = unit;
	err = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
	if (err){
		rcp_error("errorrrr");
	}
	rcp_error("listen_done");
	return unit;
}

void rcp_raw_json_listener_release(struct rcp_epoll_action *unit)
{
	close(unit->userdata.fd);
	free(unit);
}

void rcp_raw_json_listener_epoll_event(
		int epfd, struct epoll_event *ev, epoll_data_t userdata)
{
	int listener_fd = userdata.fd;
	
	//connect
	struct sockaddr_in client_address;
	size_t client_address_size = sizeof client_address;
	int fd = accept(listener_fd, (struct sockaddr*)&client_address,
			(socklen_t*)&client_address_size);
	
	//printf("accept_done\n");
	rcp_error("accept_done");
	//set timeout

	//log

	//make new connection
	rcp_connection_ref con = rcp_connection_plain_json_new(epfd, fd);
}
