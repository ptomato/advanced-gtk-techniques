#include <gtk/gtk.h>
#include <gio/gio.h>
#include "pinfoapp.h"

int
main(int argc, char **argv)
{
	gtk_init(&argc, &argv);
	return g_application_run(G_APPLICATION(p_info_app_get()), argc, argv);
}
