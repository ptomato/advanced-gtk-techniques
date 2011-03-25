#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

static void
action_visible_toggle(GtkAction *action, GtkActionGroup *beatle_action_group)
{
	gtk_action_group_set_visible(beatle_action_group, gtk_toggle_action_get_active(GTK_TOGGLE_ACTION(action)));
}

static void
action_active_toggle(GtkAction *action, GtkActionGroup *beatle_action_group)
{
	gtk_action_group_set_sensitive(beatle_action_group, gtk_toggle_action_get_active(GTK_TOGGLE_ACTION(action)));
}

static void
action_exit_program(GtkAction *action, GtkWidget *about)
{
	gtk_main_quit();
}

static void
action_show_about(GtkAction *action, GtkWidget *about)
{
	gtk_widget_show(about);
}

static void
action_lil(GtkAction *action, GtkWindow *window)
{
	/* Change the path for the relevant action in the global accelerator map, not in the accelerator group! */
	gtk_accel_map_change_entry("<Actions>/BeatleActions/PlayMusic",
		GDK_0 + g_random_int_range(0, 9),
		GDK_CONTROL_MASK | (g_random_boolean()? GDK_MOD1_MASK : 0),
		FALSE);
}

static void
action_play_music(GtkAction *action, GtkWindow *window)
{
	GtkWidget *dialog = gtk_message_dialog_new(window, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
		"Now somewhere in the black mountain hills of Dakota\n"
		"There lived a young boy named Rocky Raccoon\n"
		"And one day his woman ran off with another guy\n"
		"Hit young Rocky in the eye\n"
		"Rocky didn't like that, he said \"I'm gonna get that boy\"");
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}

static void
on_menu_item_selected(GtkItem *item, GtkStatusbar *statusbar)
{
	guint id = gtk_statusbar_get_context_id(statusbar, "MenuItemHints");
	gchar *hint;
	g_object_get(gtk_widget_get_action(GTK_WIDGET(item)), "tooltip", &hint, NULL);
	gtk_statusbar_push(statusbar, id, hint);
	g_free(hint);
}

static void
on_menu_item_deselected(GtkItem *item, GtkStatusbar *statusbar)
{
	guint id = gtk_statusbar_get_context_id(statusbar, "MenuItemHints");
	gtk_statusbar_pop(statusbar, id);
}

int
main(int argc, char *argv[])
{
	GError *error = NULL;

	gtk_init(&argc, &argv);

	/* Build the main window from the GtkBuilder file */
   	GtkBuilder *builder = gtk_builder_new();
	if(!gtk_builder_add_from_file(builder, "rocky.ui", &error))
		g_error("Error while building interface: %s", error->message);	
	GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder, "mainwindow"));
	GtkBox *box = GTK_BOX(gtk_builder_get_object(builder, "vbox"));
	GtkWidget *about = GTK_WIDGET(gtk_builder_get_object(builder, "about"));
	GtkStatusbar *statusbar = GTK_STATUSBAR(gtk_builder_get_object(builder, "statusbar"));
	if(!window)
		g_error("Error while getting main window object");
	gtk_builder_connect_signals(builder, NULL);
	g_object_unref(builder);
	
	/* Make the action groups */	
	static GtkActionEntry regularactions[] = {
		{ "StuffMenuAction", NULL, "_Stuff" },
		{ "HelpMenuAction", NULL, "_Help" },
		{ "ExitProgram", GTK_STOCK_QUIT, "_Quit", "<control>Q", "Exit the program and return to DOS", G_CALLBACK(action_exit_program) },
		{ "ShowAbout", GTK_STOCK_ABOUT, "_About", NULL, "Show some information about this program", G_CALLBACK(action_show_about) }
	};
	static GtkToggleActionEntry toggleactions[] = {
		{ "VisibleToggle", GTK_STOCK_YES, "_Visible", "<alt>V", "Make the Beatle actions visible", G_CALLBACK(action_visible_toggle), TRUE },
		{ "ActiveToggle", GTK_STOCK_NO, "_Active", "<alt>A", "Make the Beatle actions active", G_CALLBACK(action_active_toggle), TRUE }
	};
	static GtkActionEntry beatleactions[] = {
		{ "Magill", GTK_STOCK_REFRESH, "_Nancy", "<control>F1", "Her name was Magill, and she called herself Lil", G_CALLBACK(action_lil) },
		{ "PlayMusic", GTK_STOCK_MEDIA_PLAY, "_Beatles", "<control><alt>4", "Help me if you can, I'm feeling doooooown", G_CALLBACK(action_play_music) }
	};
	
	GtkActionGroup *regular_action_group = gtk_action_group_new("RegularActions");
	GtkActionGroup *beatle_action_group = gtk_action_group_new("BeatleActions");
	gtk_action_group_add_actions(regular_action_group, regularactions, G_N_ELEMENTS(regularactions), about);
	gtk_action_group_add_toggle_actions(regular_action_group, toggleactions, G_N_ELEMENTS(toggleactions), beatle_action_group);
	gtk_action_group_add_actions(beatle_action_group, beatleactions, G_N_ELEMENTS(beatleactions), GTK_WINDOW(window));

	/* Build the menu and toolbar from the GtkUIManager file */
	GtkUIManager *ui_manager = gtk_ui_manager_new();
	gtk_ui_manager_insert_action_group(ui_manager, regular_action_group, 0);
	gtk_ui_manager_insert_action_group(ui_manager, beatle_action_group, 0);
	gtk_ui_manager_add_ui_from_file(ui_manager, "rocky-menus.xml", &error);
	if(error)
		g_error("Building menus failed: %s", error->message);
	GtkWidget *menubar = gtk_ui_manager_get_widget(ui_manager, "/Menubar");
	gtk_box_pack_start(box, menubar, FALSE, FALSE, 0);
	GtkWidget *toolbar = gtk_ui_manager_get_widget(ui_manager, "/Toolbar");
	gtk_box_pack_start(box, toolbar, FALSE, FALSE, 0);		

	/* Set up the signals to do the menu hints in the statusbar */
	static gchar *menupaths[] = {
		"/Menubar/Stuff/Visible", "/Menubar/Stuff/Active", "/Menubar/Stuff/Lil",
		"/Menubar/Stuff/Quit", "/Menubar/Help/About", "/Menubar/Help/Beatles"
	};
	int i;
	for(i = 0; i < G_N_ELEMENTS(menupaths); i++) {
		GtkWidget *item = gtk_ui_manager_get_widget(ui_manager, menupaths[i]);
		if(!item) 
			continue;
		g_signal_connect(item, "select", G_CALLBACK(on_menu_item_selected), statusbar);
		g_signal_connect(item, "deselect", G_CALLBACK(on_menu_item_deselected), statusbar);
	}

	/* Connect the accelerators */
	GtkAccelGroup *accels = gtk_ui_manager_get_accel_group(ui_manager);
	gtk_window_add_accel_group(GTK_WINDOW(window), accels);

	gtk_widget_show_all(window);
	gtk_main();

	g_object_unref(ui_manager);
	
	return 0;
}

