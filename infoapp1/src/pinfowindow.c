#include <gtk/gtk.h>
#include "pinfoapp.h"
#include "pinfowindow.h"

#define P_INFO_WINDOW_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), \
	P_TYPE_INFO_WINDOW, PInfoWindowPrivate))
#define P_INFO_WINDOW_USE_PRIVATE \
	PInfoWindowPrivate *priv = P_INFO_WINDOW_PRIVATE(self)

typedef struct {
	GtkActionGroup *file_actions;
} PInfoWindowPrivate;

G_DEFINE_TYPE(PInfoWindow, p_info_window, GTK_TYPE_WINDOW);

static void p_info_window_finalize(GObject *self);

static void
p_info_window_init(PInfoWindow *self)
{
	P_INFO_WINDOW_USE_PRIVATE;
	GError *error = NULL;
	PInfoApp *theapp = p_info_app_get();

	/* Build widgets */
	GtkBuilder *builder = gtk_builder_new();
	char *objects[] = { "vbox1", "file-actions", NULL };
	if(gtk_builder_add_objects_from_file(builder, "../data/info-app.ui", objects, &error) == 0)
	{
		g_error("Problem loading user interface file: %s", error->message);
	}

	GtkWidget *window_contents = GTK_WIDGET(gtk_builder_get_object(builder, "vbox1"));
	gtk_container_add(GTK_CONTAINER(self), window_contents);

	priv->file_actions = g_object_ref(GTK_ACTION_GROUP(gtk_builder_get_object(builder, "file-actions")));

	gtk_builder_connect_signals(builder, self);
	g_signal_connect(self, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	g_object_unref(builder);

	/* Build menus and toolbar */
	GtkUIManager *ui_manager = gtk_ui_manager_new();
	gtk_ui_manager_insert_action_group(ui_manager, priv->file_actions, 0);
	gtk_ui_manager_insert_action_group(ui_manager, p_info_app_get_action_group(theapp), 1);
	if(gtk_ui_manager_add_ui_from_file(ui_manager, "../data/info-app.menus.xml", &error) == 0)
	{
		g_error("Problem loading menu file: %s", error->message);
	}

	GtkWidget *menu_bar = gtk_ui_manager_get_widget(ui_manager, "/menubar");
	GtkWidget *tool_bar = gtk_ui_manager_get_widget(ui_manager, "/toolbar");
	gtk_box_pack_start(GTK_BOX(window_contents), menu_bar, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(window_contents), tool_bar, FALSE, FALSE, 0);

	g_object_unref(ui_manager);

	/* Configure this window */
	gtk_window_set_icon_name(GTK_WINDOW(self), "info-app");
}

static void
p_info_window_class_init(PInfoWindowClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->finalize = p_info_window_finalize;

	/* Add private indirection member */
	g_type_class_add_private(klass, sizeof(PInfoWindowPrivate));
}

static void
p_info_window_finalize(GObject *self)
{
	P_INFO_WINDOW_USE_PRIVATE;
	g_object_unref(priv->file_actions);
	G_OBJECT_CLASS(p_info_window_parent_class)->finalize(self);
}

GtkWidget *
p_info_window_new(void)
{
	return GTK_WIDGET(g_object_new(p_info_window_get_type(), NULL));
}

/* Signal handlers */

void
action_info(GtkAction *action, PInfoWindow *self)
{
}

