#include <glib.h>
#include <glib-object.h>
#include <glib/gi18n.h>
#include <gio/gio.h>
#include <gtk/gtk.h>
#include "pinfoapp.h"

#ifdef MAC_INTEGRATION
#include <gtkmacintegration/gtkosxapplication.h>
#endif

#define P_INFO_APP_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), \
	P_TYPE_INFO_APP, PInfoAppPrivate))
#define P_INFO_APP_USE_PRIVATE \
	PInfoAppPrivate *priv = P_INFO_APP_PRIVATE(self)

typedef struct {
	GtkActionGroup *app_actions;
	GtkWidget *font_dialog;
} PInfoAppPrivate;

G_DEFINE_TYPE(PInfoApp, p_info_app,
#ifndef MAC_INTEGRATION
              G_TYPE_APPLICATION);
#else
              GTK_TYPE_OSX_APPLICATION);
#endif

static void p_info_app_activate(PInfoApp *theapp);
static void p_info_app_finalize(GObject *theapp);

static void
p_info_app_init(PInfoApp *self)
{
	P_INFO_APP_USE_PRIVATE;
	GError *error = NULL;

	GtkBuilder *builder = gtk_builder_new();
	char *objects[] = { "app-actions", NULL };
	if(gtk_builder_add_objects_from_file(builder, "../data/info-app.ui", objects, &error) == 0)
	{
		g_error("Problem loading user interface file: %s", error->message);
	}

	priv->app_actions = g_object_ref(GTK_ACTION_GROUP(gtk_builder_get_object(builder, "app-actions")));

	gtk_builder_connect_signals(builder, self);

	g_object_unref(builder);
}

static void
p_info_app_class_init(PInfoAppClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->finalize = p_info_app_finalize;

#ifndef MAC_INTEGRATION
	GApplicationClass *application_class = G_APPLICATION_CLASS(klass);
	application_class->activate = p_info_app_activate;
#endif

	/* Add private indirection member */
	g_type_class_add_private(klass, sizeof(PInfoAppPrivate));
}

static void
p_info_app_finalize(GObject *self)
{
	P_INFO_APP_USE_PRIVATE;
	g_object_unref(priv->app_actions);
	if(priv->font_dialog)
		gtk_widget_destroy(priv->font_dialog);

	G_OBJECT_CLASS(p_info_app_parent_class)->finalize(self);
}

PInfoApp *
p_info_app_get(void)
{
	static PInfoApp *theapp = NULL;

	if(G_UNLIKELY(theapp == NULL))
		theapp = P_INFO_APP(g_object_new(P_TYPE_INFO_APP,
#ifndef MAC_INTEGRATION
			"application-id", "org.ptomato.infoapp",
#endif
			NULL));

	return theapp;
}

static void
p_info_app_activate(PInfoApp *self)
{
	g_print("Activate\n");
}

/* Signal handlers */

void
action_about(GtkAction *action, PInfoApp *self)
{
	gtk_show_about_dialog(NULL,
		"program-name", _("InfoApp"),
		"title", _("About InfoApp"),
		"logo-icon-name", "info-app",
		"comments", _("A sample application"),
		"copyright", _("(c) 2012 Philip Chimento"),
		"website", "http://ptomato.github.com/advanced-gtk-techniques",
		NULL);
}

void
action_open(GtkAction *action, PInfoApp *self)
{
	P_INFO_APP_USE_PRIVATE;

	if(priv->font_dialog == NULL) {
		priv->font_dialog = gtk_font_selection_dialog_new(_("Choose a font"));
	}

	gtk_dialog_run(GTK_DIALOG(priv->font_dialog));
	gtk_widget_hide(priv->font_dialog);
}

void
action_preferences(GtkAction *action, PInfoApp *self)
{
}

void
action_quit(GtkAction *action, PInfoApp *self)
{
	gtk_main_quit();
}

/* Public API */

GtkActionGroup *
p_info_app_get_action_group(PInfoApp *self)
{
	P_INFO_APP_USE_PRIVATE;
	return priv->app_actions;
}
