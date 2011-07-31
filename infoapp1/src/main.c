#include <glib/gi18n.h>
#include <gtk/gtk.h>
#include <gio/gio.h>
#include "pinfoapp.h"

int
main(int argc, char **argv)
{
	/* Set up internationalization */
	setlocale (LC_ALL, "");
	bindtextdomain (PACKAGE, LOCALEDIR);
	textdomain (PACKAGE);
	
	gtk_init(&argc, &argv);
	return g_application_run(G_APPLICATION(p_info_app_get()), argc, argv);
}
