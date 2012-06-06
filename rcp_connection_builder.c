#include "rcp_pch.h"
#include "rcp_defines.h"
#include "rcp_utility.h"
#include "rcp_epoll.h"
#include "rcp_connection.h"

#include "rcp_connection_builder.h"

#include "con_buffer.h"
#include "con_plain.h"
#include "con_null_terminate.h"
#include "con_web_socket.h"
#include "con_json.h"

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
//typedef void SSL;
//struct rcp_connection_ssl{
	//int fd;
	//SSL *ssl;
//};

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

struct rcp_connection_layer2_class_part
con_web_socket_class_part = CON_WEB_SOCKET_CLASS_PART;
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
	CON_JSON_CLASS_PART
};

struct con_plain_ws_json{
	//struct rcp_connection_class *klass;

	struct rcp_connection_core core;
	//layer 1 (plain)
	struct con_plain l1;
	//layer 2 (null terminate)
	struct con_web_socket l2;
	//layer 3 (json) (no state)
};


static struct rcp_connection_class 
	con_plain_ws_json_class = {
	//l1 plain
	sizeof (struct con_plain),
	CON_PLAIN_CLASS_PART,
	//l2 null terminate
	sizeof (struct con_web_socket),
	CON_WEB_SOCKET_CLASS_PART,
	//l3 json (no data)
	0,
	CON_JSON_CLASS_PART
};

struct rcp_listener_class{
	struct rcp_connection_class *connection_class;
};

static struct rcp_listener_class lis_plain_json_class = {
	&con_plain_json_class
};

static struct rcp_listener_class lis_plain_ws_json_class = {
	&con_plain_ws_json_class
};

struct rcp_listener{
	struct rcp_listener_class *klass;
	int fd;
};

rcp_connection_ref rcp_listener_connection_new(
		struct rcp_listener_class *klass, int epfd, int fd);
void rcp_listener_release(rcp_epoll_action_ref unit);
rcp_epoll_action_ref rcp_listener_new(
		struct rcp_listener_class *klass, uint16_t port,  int epfd);
void rcp_listener_plain_json_epoll_event(
		int epfd, struct epoll_event *ev, epoll_data_t userdata);

rcp_epoll_action_ref rcp_listener_plain_json_new(int epfd)
{
	return rcp_listener_new(
			&lis_plain_json_class, RCP_RAW_JSON_PORT_NUMBER, epfd);	
}

rcp_epoll_action_ref rcp_listener_plain_ws_json_new(int epfd)
{
	return rcp_listener_new(
			&lis_plain_ws_json_class, RCP_RAW_WS_JSON_PORT_NUMBER, epfd);	
}


rcp_connection_ref rcp_listener_connection_new(
		struct rcp_listener_class *klass, int epfd, int fd)
{
	rcp_connection_ref con = rcp_connection_new(
			klass->connection_class);
	con_plain_set_fd(epfd, fd, con);
}

void rcp_listener_release(rcp_epoll_action_ref unit)
{
	struct rcp_listener *lis = unit->userdata.ptr;
	close(lis->fd);
	free(lis);
	free(unit);
}

rcp_epoll_action_ref rcp_listener_new(
		struct rcp_listener_class *klass, uint16_t port,  int epfd)
{
	struct rcp_epoll_action* unit = malloc(sizeof *unit);
	struct rcp_listener *lis = malloc(sizeof *lis);
	unit->action= rcp_listener_plain_json_epoll_event;
	unit->userdata.ptr = lis;
	lis->fd = -1;
	lis->klass = klass;

	struct sockaddr_in sockadd;
	bzero(&sockadd, sizeof sockadd);
	sockadd.sin_family = AF_INET;
	sockadd.sin_addr.s_addr = INADDR_ANY;
	sockadd.sin_port = htons(port);

	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == -1){
		rcp_listener_release(unit);
		return NULL;
	}
	lis->fd = fd;

	int err;

	err = bind(fd, (struct sockaddr*)&sockadd, sizeof sockadd);
	if (err){
		rcp_listener_release(unit);
		return NULL;
	}

	err = listen(fd, RCP_LISTEN_SOCKET_NUMBER);
	if (err){
		rcp_error("listen");
		rcp_listener_release(unit);
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
	rcp_info("listen_done");
	return unit;
}

void rcp_listener_plain_json_epoll_event(
		int epfd, struct epoll_event *ev, epoll_data_t userdata)
{
	struct rcp_listener *lis = userdata.ptr;
	int listener_fd = lis->fd;
	
	//connect
	struct sockaddr_in client_address;
	size_t client_address_size = sizeof client_address;
	int fd = accept(listener_fd, (struct sockaddr*)&client_address,
			(socklen_t*)&client_address_size);
	
	//printf("accept_done\n");
	rcp_info("accept_done");
	//set timeout

	//log

	//make new connection
	rcp_connection_ref con = rcp_listener_connection_new(
			lis->klass, epfd, fd);
}
