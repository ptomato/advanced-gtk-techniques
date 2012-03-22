#include <glib.h>
#include <glib-object.h>
#include <glib/gi18n.h>
#include <gio/gio.h>
#include <gtk/gtk.h>
#include "pinfoapp.h"
#include "pinfowindow.h"

#ifdef MAC_INTEGRATION
#include <gtkmacintegration/gtkosxapplication.h>
#endif

#define P_INFO_APP_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), \
	P_TYPE_INFO_APP, PInfoAppPrivate))
#define P_INFO_APP_USE_PRIVATE \
	PInfoAppPrivate *priv = P_INFO_APP_PRIVATE(self)

typedef struct {
	GSList *document_list;
	GtkActionGroup *app_actions;
	GtkWidget *file_dialog;
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

	/* Private properties */
	priv->document_list = NULL; /* Empty list */
	priv->file_dialog = NULL; /* Created when needed */

	/* Build objects */
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
	g_slist_free(priv->document_list);
	g_object_unref(priv->app_actions);
	if(priv->file_dialog)
		gtk_widget_destroy(priv->file_dialog);

	G_OBJECT_CLASS(p_info_app_parent_class)->finalize(self);
}

static void
p_info_app_activate(PInfoApp *self)
{
	g_print("Activate\n");
	GtkWidget *mainwin = p_info_app_open_new_window(self, NULL);
	gtk_window_present(GTK_WINDOW(mainwin));
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

	if(priv->file_dialog == NULL) {
		priv->file_dialog = gtk_file_chooser_dialog_new(_("Choose a file"),
			NULL, GTK_FILE_CHOOSER_ACTION_OPEN,
			GTK_STOCK_CANCEL, GTK_RESPONSE_REJECT,
			GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
			NULL);
	}

	int response = gtk_dialog_run(GTK_DIALOG(priv->file_dialog));
	gtk_widget_hide(priv->file_dialog);
 
	if(response == GTK_RESPONSE_ACCEPT) {
		GFile *file_to_open = gtk_file_chooser_get_file(GTK_FILE_CHOOSER(priv->file_dialog));
		GtkWidget *new_window = p_info_app_open_new_window(self, file_to_open);
		gtk_window_present(GTK_WINDOW(new_window));
	}
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

GtkActionGroup *
p_info_app_get_action_group(PInfoApp *self)
{
	P_INFO_APP_USE_PRIVATE;
	return priv->app_actions;
}

GtkWidget *
p_info_app_open_new_window(PInfoApp *self, GFile *file)
{
	P_INFO_APP_USE_PRIVATE;
	GtkWidget *retval;

	/* First check if there is a window that is already displaying the file */
	retval = p_info_app_get_open_window(self, file);
	if(retval != NULL)
		return retval;

	/* Next check if there is a window that is not displaying any file */
	retval = p_info_app_get_open_window(self, NULL);
	if(retval != NULL) {
		p_info_window_set_displayed_file(P_INFO_WINDOW(retval), file);
		return retval;
	}

	GtkWidget *window = GTK_WIDGET(g_object_new(p_info_window_get_type(),
		"displayed-file", file,
		NULL));
	priv->document_list = g_slist_prepend(priv->document_list, window);
	return window;
}

static int
window_has_file_open(PInfoWindow *window, GFile *file)
{
	GFile *file_that_is_open = p_info_window_get_displayed_file(window);
	if(file_that_is_open == NULL && file == NULL)
		return 0;
	if(file_that_is_open == NULL || file == NULL)
		return -1;
	
	int retval = -1;
	if(g_file_equal(file, file_that_is_open))
		retval = 0;
	g_object_unref(file_that_is_open);
	return retval;
}

GtkWidget *
p_info_app_get_open_window(PInfoApp *self, GFile *file)
{
	P_INFO_APP_USE_PRIVATE;

	GSList *iter = g_slist_find_custom(priv->document_list, file, (GCompareFunc)window_has_file_open);
	if(iter)
		return GTK_WIDGET(iter->data);
	return NULL;
}
