#include <glib/gi18n.h>
#include <gtk/gtk.h>
#include <gio/gio.h>
#include "pinfoapp.h"

#ifdef MAC_INTEGRATION
#include "pinfowindow.h"
#endif

int
main(int argc, char **argv)
{
	/* Set up internationalization */
	setlocale (LC_ALL, "");
	bindtextdomain (PACKAGE, LOCALEDIR);
	textdomain (PACKAGE);
	
	gtk_init(&argc, &argv);
#ifndef MAC_INTEGRATION
	return g_application_run(G_APPLICATION(p_info_app_get()), argc, argv);
#else
	PInfoApp *theapp = p_info_app_get();
	GtkWidget *mainwin = p_info_window_new();
	gtk_window_present(GTK_WINDOW(mainwin));
	gtk_main();
	g_object_unref(theapp);
	return 0;
#endif
}
