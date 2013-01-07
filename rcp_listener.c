#include "rcp_pch.h"
#include "rcp_defines.h"
#include "rcp_utility.h"
#include "rcp_event.h"

#include "rcp_io.h"
#include "rcp_sender.h"
#include "rcp_receiver.h"
#include "rcp_connection.h"
#include "rcp_context.h"
#include "rcp_timeout.h"

#include "rcp_listener.h"

#include "rcp_sender_classes.h"
#include "rcp_receiver_classes.h"

#include "connections/con_plain.h"
#include "connections/con_ssl.h"

#include "rcp_logger.h"
#include "rcp_server.h"

///
//listener
//

struct rcp_listener_class{
	struct rcp_io_class *io_klass;
	void(*io_set_fd)(rcp_io_ref io, rcp_event_action_ref unit,
			int epfd, int fd);
	rcp_sender_l1_ref (*sender_class)(rcp_sender_cluster_ref);
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

struct rcp_listener{
	struct rcp_listener_class *klass;
	int fd;
};

void rcp_listener_event_event(
		int epfd, rcp_event_ref ev, void *userdata);
rcp_connection_ref rcp_listener_connection_new(
		struct rcp_listener_class *klass, int epfd, int fd);
void rcp_listener_delete(rcp_event_action_ref unit);
rcp_event_action_ref rcp_listener_new(
		struct rcp_listener_class *klass, uint16_t port,  int epfd);


void rcp_connection_event_action(int epfd, rcp_event_ref ev,
		void *userdata)
{
	rcp_connection_ref con = userdata;
	rcp_connection_retain(con);
#ifdef RCP_USE_EPOLL
	if (ev->events & EPOLLIN){
		rcp_connection_on_receive(con);
	}
	if (ev->events & EPOLLRDHUP){
		rcp_connection_on_close(con);
	}
#endif
#ifdef RCP_USE_KQUEUE
	rcp_connection_on_receive(con);

	if (ev->flags & EV_EOF){
		rcp_connection_on_close(con);
	}
#endif
	rcp_connection_release(con);
}



rcp_event_action_ref rcp_listener_plain_json_new(int epfd)
{
	return rcp_listener_new(
			&lis_plain_json_class, RCP_RAW_JSON_PORT_NUMBER, epfd);	
}

rcp_event_action_ref rcp_listener_ws_json_new(int epfd)
{
	return rcp_listener_new(
			&lis_ws_json_class, RCP_WS_JSON_PORT_NUMBER, epfd);	
}

rcp_connection_ref rcp_listener_connection_new(
		struct rcp_listener_class *klass, int epfd, int fd)
{
	rcp_connection_ref con = rcp_connection_new();

	rcp_io_ref io = rcp_io_new(klass->io_klass);
	rcp_connection_set_io(con, io);

	rcp_event_action_ref unit = malloc(sizeof *unit);
	unit->action = rcp_connection_event_action;
	unit->userdata = con;
	klass->io_set_fd(io, unit, epfd, fd);

	rcp_sender_cluster_ref cls = rcp_shared_sender_cluster();
	rcp_sender_l1_ref sender = klass->sender_class(cls); 
	rcp_connection_set_sender(con, sender);

	rcp_receiver_ref receiver = rcp_receiver_new(klass->receiver_klass);
	rcp_connection_set_receiver(con, receiver);
	
	rcp_open_timeout_add(rcp_open_timeout(), con);
	rcp_connection_release(con);
	return con;
}

void rcp_listener_delete(rcp_event_action_ref unit)
{
	struct rcp_listener *lis = unit->userdata;
	close(lis->fd);
	free(lis);
	free(unit);
}

rcp_event_action_ref rcp_listener_new(
		struct rcp_listener_class *klass, uint16_t port,  int epfd)
{
	rcp_event_action_ref unit = malloc(sizeof *unit);
	struct rcp_listener *lis = malloc(sizeof *lis);
	unit->action= rcp_listener_event_event;
	unit->userdata = lis;
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

	int val = 1;
	err = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof val);
	if (err){
		rcp_error("sockopt");
		rcp_listener_delete(unit);
		return NULL;
	}

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

	// ! setup event event
	rcp_event_add_fd(epfd, fd, unit);

	rcp_info("listen_done");
	return unit;
}

void rcp_listener_event_event(
		int epfd, rcp_event_ref ev, void *userdata)
{
	struct rcp_listener *lis = userdata;
	int listener_fd = lis->fd;
	
	//connect
	struct sockaddr client_address;
	size_t client_address_size = sizeof client_address;
	int fd = accept(listener_fd, &client_address,
			(socklen_t*)&client_address_size);
	
	//printf("accept_done\n");
	rcp_info("accept_done");
	//set timeout
	struct timeval tv;
	tv.tv_sec = RCP_SEND_TIMEOUT_SEC;
	tv.tv_usec = 0;
	if (setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO,
				&tv, sizeof tv) <0){
		rcp_error("set socket timeout");
		close(fd);
		return;
	}

	rcp_log_access(&client_address);

	//make new connection
	rcp_listener_connection_new(lis->klass, epfd, fd);
}
