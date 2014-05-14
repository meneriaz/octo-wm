/*!
 * \file events.h
 */

#ifndef H_EVENTS
#define H_EVENTS

#include "win.h"
#include <xcb/xcb.h>

/*!
 * \brief Handle all pending events on the given connection.
 */
void handle_events(xcb_connection_t *c, winv_t *ws);

void event_map_request(xcb_connection_t *c, winv_t *ws,
                       xcb_generic_event_t *ge);
void event_configure_request(xcb_connection_t *c, winv_t *ws,
                             xcb_generic_event_t *ge);
void event_destroy_notify(xcb_connection_t *c, winv_t *ws,
                          xcb_generic_event_t *ge);
void event_unmap_notify(xcb_connection_t *c, winv_t *ws,
                        xcb_generic_event_t *ge);
void event_enter_notify(xcb_connection_t *c, winv_t *ws,
                        xcb_generic_event_t *ge);

#endif /* H_EVENTS */

