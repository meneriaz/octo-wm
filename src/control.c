/*!
 * \file control.c
 */

#include "control.h"
#include <unistd.h>
#include <string.h>

int create_socket(const char *path)
{
    int fd;
    int rc;
    struct sockaddr_un addr;

    fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd < 0)
        return -1;

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, path, sizeof(addr.sun_path) - 1);
    rc = bind(fd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un));
    if (rc < 0)
        return -1;

    rc = listen(fd, SOMAXCONN);
    if (rc < 0)
        return -1;

    return fd;
}

void control(int sock_fd, xcb_connection_t *c, winv_t *ws, int *running)
{
    char msg[1];
    ssize_t n;

    sock_fd = accept(sock_fd, NULL, 0);
    if (sock_fd < 0)
        return; /* Check errno. */

    n = recv(sock_fd, msg, sizeof(msg), 0);
    if (n != 1)
        return; /* Check errno. */

    switch (msg[0]) {
    case 0:
    case 'q':
        *running = 0;
        break;
    }
    close(sock_fd);
}

