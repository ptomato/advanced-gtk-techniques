#ifndef P_INFO_WIDGET_H
#define P_INFO_WIDGET_H

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define P_TYPE_INFO_WIDGET            (p_info_widget_get_type())
#define P_INFO_WIDGET(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), P_TYPE_INFO_WIDGET, PInfoWidget))
#define P_INFO_WIDGET_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass), P_TYPE_INFO_WIDGET, PInfoWidgetClass))
#define P_IS_INFO_WIDGET(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), P_TYPE_INFO_WIDGET))
#define P_IS_INFO_WIDGET_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), P_TYPE_INFO_WIDGET))
#define P_INFO_WIDGET_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), P_TYPE_INFO_WIDGET, PInfoWidgetClass))

typedef struct {
	GtkGrid parent_instance;
} PInfoWidget;

typedef struct {
	GtkGridClass parent_class;
} PInfoWidgetClass;

GType p_info_widget_get_type(void) G_GNUC_CONST;
GtkWidget *p_info_widget_new(void);
GIcon *p_info_widget_get_icon(PInfoWidget *self);
void p_info_widget_set_icon(PInfoWidget *self, GIcon *icon);
const char *p_info_widget_get_display_name(PInfoWidget *self);
void p_info_widget_set_display_name(PInfoWidget *self, const char *display_name);

G_END_DECLS

#endif /* P_INFO_WIDGET_H */
