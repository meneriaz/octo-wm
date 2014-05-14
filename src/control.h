/*!
 * \file control.h
 *
 * Window manager control.
 */

#include "win.h"
#include <xcb/xcb.h>
#include <sys/socket.h>
#include <sys/un.h>

/*!
 * Create an unbound socket, bind the given path to the socket and listen for
 * connections.
 *
 * \param path The path that will be bound to the socket.
 * \return A non-negative integer, the socket file descriptor, on success and
 *         -1 on failure, in which case `errno` will be set to indicate the
 *         error.
 */
int create_socket(const char *path);

/*!
 * \brief Receive and parse all outstanding control messages from socket.
 * \param sock_fd File descriptor for the socket to accept connections on.
 * \param conn Active connection to the X server.
 * \param ws Vector of managed windows.
 * \param running Pointer to integer that gracefully stops program execution
 *        when set to 0.
 */
void control(int sock_fd, xcb_connection_t *conn, winv_t *ws, int *running);


