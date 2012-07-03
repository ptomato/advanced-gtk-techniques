#ifndef P_INFO_WINDOW_H
#define P_INFO_WINDOW_H

#include <gio/gio.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define P_TYPE_INFO_WINDOW            (p_info_window_get_type())
#define P_INFO_WINDOW(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), P_TYPE_INFO_WINDOW, PInfoWindow))
#define P_INFO_WINDOW_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass), P_TYPE_INFO_WINDOW, PInfoWindowClass))
#define P_IS_INFO_WINDOW(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), P_TYPE_INFO_WINDOW))
#define P_IS_INFO_WINDOW_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), P_TYPE_INFO_WINDOW))
#define P_INFO_WINDOW_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), P_TYPE_INFO_WINDOW, PInfoWindowClass))

typedef struct {
	GtkWindow parent_instance;
} PInfoWindow;

typedef struct {
	GtkWindowClass parent_class;
} PInfoWindowClass;

GType p_info_window_get_type(void) G_GNUC_CONST;
void p_info_window_set_displayed_file(PInfoWindow *self, GFile *file);
GFile *p_info_window_get_displayed_file(PInfoWindow *self);

G_END_DECLS

#endif /* P_INFO_WINDOW_H */
