#include "skynet.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFSIZE 20

typedef struct telnetd_s {
    int fd;
    uint32_t watchdog;
} telnetd_t;

static int
_cb(struct skynet_context * context, void * ud, int type, int session, uint32_t source, const void * msg, size_t sz) {
    telnetd_t *t = ud;
    struct sockaddr_in peer_addr;
    int addrlen = sizeof(peer_addr);
    int sock = accept(t->fd, (struct sockaddr*)&peer_addr, (socklen_t*)&addrlen);
    char buf[BUFSIZE];
    if(sock >= 0) {
        skynet_error(context, "[telnetd] addr: %s, port: %hu, fd:%d", inet_ntoa(peer_addr.sin_addr),
                ntohs(peer_addr.sin_port), sock);
        snprintf(buf, BUFSIZE, "%d", sock);
        skynet_send(context, 0, t->watchdog, PTYPE_TEXT, 0, buf, strlen(buf));
    }
	skynet_command(context, "TIMEOUT", "500");
    return 0;
}

static int
create_listen_socket(ctx, addr, port) {
    printf("listen socket, addr:%d, port:%d\n", addr, port);
    int fd, flag;
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1) {
        return -1;
    }

    // set nonblocking
	flag = fcntl(fd, F_GETFL, 0);
	if ( -1 == flag ) {
        close(fd);
		return -1;
	}

	if(-1 == fcntl(fd, F_SETFL, flag | O_NONBLOCK)) {
        close(fd);
        return -1;
    }

    // set reuse
	int reuse = 1;
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));

    // bind
    struct sockaddr_in listen_addr;
    listen_addr.sin_family = AF_INET;
    listen_addr.sin_port   = htons(port);
    listen_addr.sin_addr.s_addr   = addr;

    if(bind(fd, (struct sockaddr*)&listen_addr, sizeof(listen_addr)) == -1) {
        close(fd);
        return -1;
    }

    // listen
    if(listen(fd, 5) == -1) {
        close(fd);
        return -1;
    }

    return fd;
}

int
telnetd_init(telnetd_t *t, struct skynet_context *ctx, const char* args) {
    int sz = strlen(args) + 1;
    char ip[sz];
    int watchdog, port, fd;
    uint32_t addr = INADDR_ANY;
    
    sscanf(args, "%d %s %d", &watchdog, ip, &port);
    if(strlen(ip) > 0) {
        addr = inet_addr(ip);
    }

    fd = create_listen_socket(ctx, addr, port);
    if(fd == -1) {
		skynet_error(ctx, "create listen socket failed, ip:%s, port %d",ip, port);
        return 1;
    }

    t->fd = fd;
    t->watchdog = watchdog;
    skynet_callback(ctx, t, _cb);
	skynet_command(ctx, "TIMEOUT", "500");
    return 0;
}

telnetd_t*
telnetd_create(void) {
    telnetd_t *t = malloc(sizeof(*t));
	memset(t,0,sizeof(*t));

    t->fd = -1;
    return t;
}

void
telnetd_release(telnetd_t *t) {
    close(t->fd);
    free(t);
}

