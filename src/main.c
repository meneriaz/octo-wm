/*!
 * \file main.c
 */

#include "control.h"
#include "events.h"
#include "win.h"
#include <xcb/xcb.h>
#include <xcb/xcb_aux.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/un.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*!
 * \brief Return the largest of the two given expressions.
 *
 * Beware that the returned expression is evaluated twice!
 */
#define MAX(a, b) (a < b ? b : a)

int main(int argc, char **argv)
{
    fd_set fds;
    int scr_n;
    int cmd_fd;
    int srv_fd;
    int running;
    uint32_t values[1];
    winv_t winv;
    xcb_connection_t *conn;
    xcb_screen_t *scr;
    xcb_void_cookie_t cookie;
    xcb_generic_error_t *err;

    conn = NULL;
    cmd_fd = create_socket("/tmp/yolo-dangerzone");
    if (cmd_fd < 0)
        goto error_socket;

    conn = xcb_connect(NULL, &scr_n);
    if (xcb_connection_has_error(conn))
        goto error_connection;
    srv_fd = xcb_get_file_descriptor(conn);
    scr = xcb_aux_get_screen(conn, scr_n);

    /* Grab the server so we don't suffer from lost windows while
     * scanning for windows and registering for events. */
    xcb_grab_server(conn);
    winv = scan(conn, scr->root);
    values[0] = XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT
        | XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY;
    cookie = xcb_change_window_attributes_checked(conn, scr->root,
                                                  XCB_CW_EVENT_MASK, values);
    err = xcb_request_check(conn, cookie);
    if (err != NULL)
        goto error_event;
    arrange(conn, &winv);
    xcb_ungrab_server(conn);

    running = 1;

    while (running) {
        FD_ZERO(&fds);
        FD_SET(srv_fd, &fds);
        FD_SET(cmd_fd, &fds);

        select(MAX(srv_fd, cmd_fd) + 1, &fds, NULL, NULL, NULL);
        if (FD_ISSET(cmd_fd, &fds))
            control(cmd_fd, conn, &winv, &running);
        if (FD_ISSET(srv_fd, &fds))
            handle_events(conn, &winv);
    }

    goto cleanup;
error_socket:
    fprintf(stderr,
            "%s: Could not create the command socket. errno %d.\n",
            argv[0], errno);
    goto cleanup;
error_connection:
    fprintf(stderr,
            "%s: Could not connect to the X server. Error %d (see XCB_CONN_* in <xcb/xcb.h>).\n",
            argv[0], xcb_connection_has_error(conn));
    goto cleanup;
error_event:
    fprintf(stderr,
            "%s: Another window manager is already running.\n",
            argv[0]);
    goto cleanup;
cleanup:
    xcb_disconnect(conn);
    close(cmd_fd);
    unlink("/tmp/yolo-dangerzone");
    return 0;
}

