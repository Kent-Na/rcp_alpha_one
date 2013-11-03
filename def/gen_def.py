
typeList = [
	'context',
	'connection',
	'sender_l0',
	'sender_l1',
	'sender_cluster',
	'receiver',
	'io',

	'open_timeout',

	#'event', special case
	'event_action',

	'command',
]

template = """
#ifndef RCP_DEF_{capitalName}
#define RCP_DEF_{capitalName}
typedef struct rcp_{name}_core *rcp_{name}_ref;
#endif
"""

for name in typeList:
	with open('rcp_{name}.h'.format(name = name),'w') as f:
		f.write(template.format(name = name, capitalName = name.upper()))

eventTemplate = """
#ifndef RCP_DEF_EVENT
#define RCP_DEF_EVENT
#ifdef __linux__
typedef struct epoll_event *rcp_event_ref;
#define RCP_USE_EPOLL
#else
typedef struct kevent *rcp_event_ref;
#define RCP_USE_KQUEUE
#endif
#endif
"""

with open('rcp_event.h','w') as f:
	f.write(eventTemplate);
