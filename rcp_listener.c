#include "rcp_pch.h"
#include "rcp_defines.h"
#include "rcp_utility.h"
#include "rcp_epoll.h"

#include "rcp_types.h"
#include "rcp_io.h"
#include "rcp_sender.h"
#include "rcp_receiver.h"
#include "rcp_connection.h"

#include "rcp_listener.h"

#include "rcp_sender_classes.h"
#include "rcp_receiver_classes.h"

#include "con_plain.h"
#include "con_ssl.h"

///
//listener
//

struct rcp_listener_class{
	struct rcp_io_class *io_klass;
	void(*io_set_fd)(rcp_io_ref io, struct rcp_epoll_action *unit,
			int epfd, int fd);
	rcp_sender_ref (*sender_class)(rcp_sender_cluster_ref);
	struct rcp_receiver_class *receiver_klass;
};

static struct rcp_listener_class lis_plain_json_class = {
	&con_plain_class,
	con_plain_set_fd,
	rcp_sender_cluster_json_nt,
	&con_nt_json_class,
};

static struct rcp_listener_class lis_ws_json_class = {
	&con_plain_class,
	con_plain_set_fd,
	rcp_sender_cluster_json_ws,
	&con_ws_json_class
};

static struct rcp_listener_class lis_wss_json_class = {
	&con_ssl_class,
	con_ssl_set_fd,
	rcp_sender_cluster_json_ws,
	&con_ws_json_class
};

struct rcp_listener{
	struct rcp_listener_class *klass;
	int fd;
};

void rcp_listener_epoll_event(
		int epfd, struct epoll_event *ev, epoll_data_t userdata);
rcp_connection_ref rcp_listener_connection_new(
		struct rcp_listener_class *klass, int epfd, int fd);
void rcp_listener_delete(rcp_epoll_action_ref unit);
rcp_epoll_action_ref rcp_listener_new(
		struct rcp_listener_class *klass, uint16_t port,  int epfd);


void rcp_connection_epoll_action(int epfd, struct epoll_event *ev,
		epoll_data_t data)
{
	rcp_connection_ref con = (rcp_connection_ref)data.ptr;
	if (ev->events & EPOLLIN){
		rcp_connection_on_receive(con);
	}
	if (ev->events & EPOLLRDHUP){
		rcp_connection_on_close(con);
	}
}



rcp_epoll_action_ref rcp_listener_plain_json_new(int epfd)
{
	return rcp_listener_new(
			&lis_plain_json_class, RCP_RAW_JSON_PORT_NUMBER, epfd);	
}

rcp_epoll_action_ref rcp_listener_ws_json_new(int epfd)
{
	return rcp_listener_new(
			&lis_ws_json_class, RCP_WS_JSON_PORT_NUMBER, epfd);	
}

rcp_epoll_action_ref rcp_listener_wss_json_new(int epfd)
{
	return rcp_listener_new(
			&lis_wss_json_class, RCP_WSS_JSON_PORT_NUMBER, epfd);	
}

rcp_connection_ref rcp_listener_connection_new(
		struct rcp_listener_class *klass, int epfd, int fd)
{
	rcp_connection_ref con = rcp_connection_new();

	rcp_io_ref io = rcp_io_new(klass->io_klass);
	rcp_connection_set_io(con, io);

	struct rcp_epoll_action *unit = malloc(sizeof *unit);
	unit->action = rcp_connection_epoll_action;
	unit->userdata.ptr = con;
	klass->io_set_fd(io, unit, epfd, fd);

	rcp_sender_cluster_ref cls = rcp_shared_sender_cluster();
	rcp_sender_ref sender = klass->sender_class(cls); 
	rcp_connection_set_sender(con, sender);

	rcp_receiver_ref receiver = rcp_receiver_new(klass->receiver_klass);
	rcp_connection_set_receiver(con, receiver);
	return con;
}

void rcp_listener_delete(rcp_epoll_action_ref unit)
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
	unit->action= rcp_listener_epoll_event;
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
		rcp_error("socket");
		rcp_listener_delete(unit);
		return NULL;
	}
	lis->fd = fd;

	int err;

	err = bind(fd, (struct sockaddr*)&sockadd, sizeof sockadd);
	if (err){
		rcp_error("bind");
		rcp_listener_delete(unit);
		return NULL;
	}

	err = listen(fd, RCP_LISTEN_SOCKET_NUMBER);
	if (err){
		rcp_error("listen");
		rcp_listener_delete(unit);
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

void rcp_listener_epoll_event(
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
	rcp_listener_connection_new(lis->klass, epfd, fd);
}
