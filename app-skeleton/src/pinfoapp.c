#include <glib.h>
#include <glib-object.h>
#include <gio/gio.h>
#include "pinfoapp.h"

G_DEFINE_TYPE(PInfoApp, p_info_app, G_TYPE_APPLICATION);

static void
p_info_app_init(PInfoApp *self)
{
}

static void
p_info_app_class_init(PInfoAppClass *klass)
{
}

PInfoApp *
p_info_app_get(void)
{
	static PInfoApp *theapp = NULL;

	if(G_UNLIKELY(theapp == NULL))
		theapp = P_INFO_APP(g_object_new(P_TYPE_INFO_APP,
			"application-id", "org.ptomato.app-skeleton",
			NULL));

	return theapp;
}

