/*!
 * \file events.c
 */

#include "events.h"
#include "win.h"
#include <stdlib.h>
#include <stdio.h>

void handle_events(xcb_connection_t *c, winv_t *ws)
{
    xcb_generic_event_t *ge;

    while ((ge = xcb_poll_for_event(c))) {
        switch (ge->response_type & ~0x80) {
        case XCB_MAP_REQUEST:
            event_map_request(c, ws, ge);
            break;
        case XCB_UNMAP_NOTIFY:
            event_unmap_notify(c, ws, ge);
            break;
        case XCB_DESTROY_NOTIFY:
            event_destroy_notify(c, ws, ge);
            break;
        }

        free(ge);
    }
}

void event_map_request(xcb_connection_t *c, winv_t *ws,
                       xcb_generic_event_t *ge)
{
    xcb_map_request_event_t *evt;

    evt = (xcb_map_request_event_t *)ge;
    manage(c, ws, evt->window);
}

void event_configure_request(xcb_connection_t *c, winv_t *ws,
                             xcb_generic_event_t *ge)
{
}

void event_destroy_notify(xcb_connection_t *c, winv_t *ws,
                          xcb_generic_event_t *ge)
{
    xcb_destroy_notify_event_t *evt;

    evt = (xcb_destroy_notify_event_t *)ge;
    unmanage(c, ws, evt->window);
}

void event_unmap_notify(xcb_connection_t *c, winv_t *ws,
                        xcb_generic_event_t *ge)
{
    xcb_unmap_notify_event_t *evt;
    
    evt = (xcb_unmap_notify_event_t *)ge;
    unmanage(c, ws, evt->window);
}

void event_enter_notify(xcb_connection_t *c, winv_t *ws,
                        xcb_generic_event_t *ge)
{
}

