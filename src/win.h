/*!
 * \file win.h
 */

#ifndef H_WIN
#define H_WIN

#include <xcb/xcb.h>
#include <stddef.h>

#define WINV_MIN_CAP    8   /*!< Starting capacity of a winv object. */
#define WINV_STEP       4   /*!< Step size of increases in winv capacity. */

/*!
 * A managed window.
 */
typedef struct win_t {
    xcb_window_t id;        /*!< ID of the window on the X server. */
} win_t;

/*!
 * Vector of managed windows.
 */
typedef struct winv_t {
    size_t cap;             /*!< Size of memory allocated for data. */
    size_t size;            /*!< Number of elements in data. */
    win_t *data;            /*!< Vector elements. */
} winv_t;

/*!
 * \brief Set a window vector to an useable state.
 */
void winv_init(winv_t *winv);

/*!
 * \brief Add a window to the end of a window vector.
 */
void winv_push_back(winv_t *winv, win_t win);

/*!
 * \brief Remove the window at the given position from a vector.
 *
 * If the given position is out of range for the given vector no action is
 * performed.
 */
void winv_erase(winv_t *winv, size_t pos);

/*!
 * \brief Returns the position of the first window with the given id.
 *
 * Returns the position of the first window with the given id in the given
 * vector, if such a window exists. If no such window exists the position
 * past the end of the vector is returned.
 */
size_t winv_find(winv_t *winv, xcb_window_t id);

void manage(xcb_connection_t *c, winv_t *ws, xcb_window_t id);
void unmanage(xcb_connection_t *c, winv_t *ws, xcb_window_t id);
void arrange(xcb_connection_t *c, winv_t *ws);
/*!
 * \brief Scan for children of the given window with desireable attributes.
 *
 * Returns a vector containing all children of the given root window that are
 * viewable and have their override-redirect attribute set to false.
 */
winv_t scan(xcb_connection_t *conn, xcb_window_t root);

#endif /* H_WIN */

