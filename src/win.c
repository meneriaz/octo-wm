/*!
 * \file win.c
 */

#include "win.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void winv_init(winv_t *winv)
{
    winv->cap = WINV_MIN_CAP;
    winv->size = 0;
    winv->data = malloc(WINV_MIN_CAP * sizeof(win_t));
}

static void _winv_reserve(winv_t *winv, size_t n)
{
    if (n <= winv->cap)
        return;
    winv->cap = n;
    winv->data = realloc(winv->data, n);
}

void winv_push_back(winv_t *winv, win_t win)
{
    if (winv->size == winv->cap)
        _winv_reserve(winv, winv->cap + WINV_STEP);
    winv->data[winv->size] = win;
    winv->size++;
}

size_t winv_find(winv_t *winv, xcb_window_t id)
{
    size_t pos;

    for (pos = 0; pos < winv->size; pos++)
        if (winv->data[pos].id == id)
            return pos;
    return pos;
}

void winv_erase(winv_t *winv, size_t pos) {
    if (pos < winv->size) {
        winv->size--;
        memmove(winv->data + pos, winv->data + pos + 1, winv->size - pos);
    }
}

void manage(xcb_connection_t *c, winv_t *ws, xcb_window_t id)
{
    win_t win;

    printf("New window!\n");

    win.id = id;
    winv_push_back(ws, win);
    xcb_map_window(c, id);
    arrange(c, ws);
}

void unmanage(xcb_connection_t *c, winv_t *ws, xcb_window_t id)
{
    winv_erase(ws, winv_find(ws, id));
}

void arrange(xcb_connection_t *conn, winv_t *winv)
{
    uint16_t mask;

    if (winv->size == 0)
        return;

    mask = XCB_CONFIG_WINDOW_X
         | XCB_CONFIG_WINDOW_Y
         | XCB_CONFIG_WINDOW_WIDTH
         | XCB_CONFIG_WINDOW_HEIGHT;

    const uint32_t values[] = {
        0,    /* x */
        0,    /* y */
        950, /* width */
        200  /* height */
    };

    xcb_configure_window(conn, winv->data[0].id, mask, values);
}

winv_t scan(xcb_connection_t *conn, xcb_window_t root)
{
    xcb_query_tree_cookie_t qt_cookie;
    xcb_query_tree_reply_t *qt_reply;
    int childc;
    xcb_window_t *childv;
    int i;
    xcb_get_window_attributes_cookie_t *wa_cookies;
    xcb_get_window_attributes_reply_t *wa_reply;
    win_t win;
    winv_t winv;

    qt_cookie = xcb_query_tree(conn, root);
    qt_reply = xcb_query_tree_reply(conn, qt_cookie, NULL);

    childc = xcb_query_tree_children_length(qt_reply);
    childv = xcb_query_tree_children(qt_reply);

    wa_cookies =
        malloc((size_t)childc * sizeof(xcb_get_window_attributes_cookie_t));

    for (i = 0; i < childc; i++)
        wa_cookies[i] = xcb_get_window_attributes(conn, childv[i]);

    winv_init(&winv);
    for (i = 0; i < childc; i++) {
        wa_reply = xcb_get_window_attributes_reply(conn, wa_cookies[i], NULL);

        if (wa_reply->map_state == XCB_MAP_STATE_VIEWABLE &&
                !(wa_reply->override_redirect)) {
            win.id = childv[i];
            winv_push_back(&winv, win);
        }
        free(wa_reply);
    }

    free(wa_cookies);
    free(qt_reply);

    return winv;
}

