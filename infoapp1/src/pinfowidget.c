#include <glib/gi18n.h>
#include <gtk/gtk.h>
#include "pinfoapp.h"
#include "pinfowindow.h"
#include "pinfowidget.h"

#define P_INFO_WIDGET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), \
	P_TYPE_INFO_WIDGET, PInfoWidgetPrivate))
#define P_INFO_WIDGET_USE_PRIVATE \
	PInfoWidgetPrivate *priv = P_INFO_WIDGET_PRIVATE(self)

typedef struct {
	GIcon *icon;
	char *display_name;
	GtkWidget *icon_widget;
	GtkWidget *name_widget;
} PInfoWidgetPrivate;

enum {
	PROP_0,
	PROP_ICON,
	PROP_DISPLAY_NAME
};

G_DEFINE_TYPE(PInfoWidget, p_info_widget, GTK_TYPE_GRID);

static void p_info_widget_finalize(GObject *self);
static void p_info_widget_get_property(GObject *self, unsigned prop_id, GValue *value, GParamSpec *pspec);
static void p_info_widget_set_property(GObject *self, unsigned prop_id, const GValue *value, GParamSpec *pspec);

static void
p_info_widget_init(PInfoWidget *self)
{
	P_INFO_WIDGET_USE_PRIVATE;
	priv->icon = NULL;
	priv->display_name = NULL;

	/* Construct GUI */
	priv->icon_widget = gtk_image_new_from_stock(GTK_STOCK_MISSING_IMAGE, GTK_ICON_SIZE_DIALOG);
	priv->name_widget = gtk_label_new(_("(none)"));
	gtk_widget_show(priv->icon_widget);
	gtk_widget_show(priv->name_widget);
	gtk_grid_attach_next_to(GTK_GRID(self), priv->icon_widget, NULL, GTK_POS_RIGHT, 1, 1);
	gtk_grid_attach_next_to(GTK_GRID(self), priv->name_widget, NULL, GTK_POS_RIGHT, 1, 1);
}

static void
p_info_widget_class_init(PInfoWidgetClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->finalize = p_info_widget_finalize;
	object_class->get_property = p_info_widget_get_property;
	object_class->set_property = p_info_widget_set_property;

	/* Add private indirection member */
	g_type_class_add_private(klass, sizeof(PInfoWidgetPrivate));

	/* Add properties */
	g_object_class_install_property(object_class, PROP_ICON,
		g_param_spec_object("icon", "Icon",
			"The icon currently displaying",
			G_TYPE_ICON,
			G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
	g_object_class_install_property(object_class, PROP_DISPLAY_NAME,
		g_param_spec_string("display-name", "Display name",
			"The name of the file to display",
			NULL,
			G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
}

static void
p_info_widget_finalize(GObject *self)
{
	P_INFO_WIDGET_USE_PRIVATE;

	if(priv->icon)
		g_object_unref(priv->icon);
	g_free(priv->display_name);

	G_OBJECT_CLASS(p_info_widget_parent_class)->finalize(self);
}

static void
p_info_widget_get_property(GObject *self, unsigned prop_id, GValue *value, GParamSpec *pspec)
{
	switch(prop_id) {
		case PROP_ICON:
			g_value_take_object(value, G_OBJECT(p_info_widget_get_icon(P_INFO_WIDGET(self))));
			break;
		case PROP_DISPLAY_NAME:
			g_value_set_string(value, p_info_widget_get_display_name(P_INFO_WIDGET(self)));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(self, prop_id, pspec);
	}
}

static void
p_info_widget_set_property(GObject *self, unsigned prop_id, const GValue *value, GParamSpec *pspec)
{
	switch(prop_id) {
		case PROP_ICON:
			p_info_widget_set_icon(P_INFO_WIDGET(self), G_ICON(g_value_get_object(value)));
			break;
		case PROP_DISPLAY_NAME:
			p_info_widget_set_display_name(P_INFO_WIDGET(self), g_value_get_string(value));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(self, prop_id, pspec);
	}
}

/* "Public" API */

GtkWidget *
p_info_widget_new(void)
{
	return GTK_WIDGET(g_object_new(p_info_widget_get_type(), NULL));
}

GIcon *
p_info_widget_get_icon(PInfoWidget *self)
{
	g_return_if_fail(self || P_IS_INFO_WIDGET(self));
	P_INFO_WIDGET_USE_PRIVATE;
	if(priv->icon == NULL)
		return NULL;
	return g_object_ref(priv->icon);
}

void
p_info_widget_set_icon(PInfoWidget *self, GIcon *icon)
{
	g_return_if_fail(self || P_IS_INFO_WIDGET(self));
	g_return_if_fail(icon && G_IS_ICON(icon));
	P_INFO_WIDGET_USE_PRIVATE;

	if(priv->icon)
		g_object_unref(priv->icon);

	priv->icon = icon? g_object_ref(icon) : NULL;
	if(icon) {
		priv->icon = g_object_ref(icon);
		gtk_image_set_from_gicon(GTK_IMAGE(priv->icon_widget), icon, GTK_ICON_SIZE_DIALOG);
	} else {
		priv->icon = NULL;
		gtk_image_set_from_stock(GTK_IMAGE(priv->icon_widget), GTK_STOCK_MISSING_IMAGE, GTK_ICON_SIZE_DIALOG);
	}

	g_object_notify(G_OBJECT(self), "icon");
}

const char *
p_info_widget_get_display_name(PInfoWidget *self)
{
	g_return_if_fail(self || P_IS_INFO_WIDGET(self));
	P_INFO_WIDGET_USE_PRIVATE;
	return priv->display_name;
}

void
p_info_widget_set_display_name(PInfoWidget *self, const char *display_name)
{
	g_return_if_fail(self || P_IS_INFO_WIDGET(self));
	P_INFO_WIDGET_USE_PRIVATE;

	g_free(priv->display_name);

	if(display_name) {
		priv->display_name = g_strdup(display_name);
		gtk_label_set_text(GTK_LABEL(priv->name_widget), priv->display_name);
	} else {
		priv->display_name = NULL;
		gtk_label_set_text(GTK_LABEL(priv->name_widget), _("(none)"));
	}
	
	g_object_notify(G_OBJECT(self), "display-name");
}
