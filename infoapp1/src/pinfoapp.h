#ifndef P_INFO_APP_H
#define P_INFO_APP_H

#include <gio/gio.h>
#include <gtk/gtk.h>

#ifdef MAC_INTEGRATION
#include <gtkmacintegration/gtkosxapplication.h>
#endif

G_BEGIN_DECLS

#define P_TYPE_INFO_APP            (p_info_app_get_type())
#define P_INFO_APP(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), P_TYPE_INFO_APP, PInfoApp))
#define P_INFO_APP_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass), P_TYPE_INFO_APP, PInfoAppClass))
#define P_IS_INFO_APP(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), P_TYPE_INFO_APP))
#define P_IS_INFO_APP_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), P_TYPE_INFO_APP))
#define P_INFO_APP_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), P_TYPE_INFO_APP, PInfoAppClass))

typedef struct {
#ifndef MAC_INTEGRATION
	GApplication parent_instance;
#else
	GtkOSXApplication parent_instance;
#endif
} PInfoApp;

typedef struct {
#ifndef MAC_INTEGRATION
	GApplicationClass parent_class;
#else
	GtkOSXApplicationClass parent_class;
#endif
} PInfoAppClass;

GType p_info_app_get_type(void) G_GNUC_CONST;
PInfoApp *p_info_app_get(void);
GtkActionGroup *p_info_app_get_action_group(PInfoApp *self);

G_END_DECLS

#endif /* P_INFO_APP_H */
