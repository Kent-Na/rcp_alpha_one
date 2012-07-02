#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>

#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#ifdef __FreeBSD__
#include <sys/endian.h>
#endif

#ifdef __APPLE__
#include <sys/_endian.h>
#endif

#ifdef __linux__
#include <sys/epoll.h>
#else
#include <sys/event.h>
#endif

#include <openssl/ssl.h>
