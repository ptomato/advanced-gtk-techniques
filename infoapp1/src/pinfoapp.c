#include <glib.h>
#include <glib-object.h>
#include <gio/gio.h>
#include "pinfoapp.h"

#ifdef MAC_INTEGRATION
#include <gtkmacintegration/gtkosxapplication.h>
#endif

G_DEFINE_TYPE(PInfoApp, p_info_app,
#ifndef MAC_INTEGRATION
              G_TYPE_APPLICATION);
#else
              GTK_TYPE_OSX_APPLICATION);
#endif

static void p_info_app_activate(PInfoApp *theapp);

static void
p_info_app_init(PInfoApp *self)
{
#ifdef MAC_INTEGRATION
	g_signal_connect(self, "NSApplicationDidBecomeActive", G_CALLBACK(p_info_app_activate), NULL);
#endif
}

static void
p_info_app_class_init(PInfoAppClass *klass)
{
#ifndef MAC_INTEGRATION
	GApplicationClass *application_class = G_APPLICATION_CLASS(klass);
	application_class->activate = p_info_app_activate;
#endif
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
p_info_app_activate(PInfoApp *theapp)
{
	g_print("Activate\n");
}
