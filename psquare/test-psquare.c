#include <gtk/gtk.h>
#include "psquare.h"

/* When the user clicks the Add button, add a random widget to the PSquare */
static void
add_clicked(GtkToolButton *button, GtkWidget *square)
{
    GList *children;
    GtkWidget *widget;
    gint count;
    gchar *text;
    
    /* Get the number of this new widget and print it in a string */
    children = gtk_container_get_children(GTK_CONTAINER(square));
    count = g_list_position(children, g_list_last(children)) + 2;
    g_list_free(children);
    text = g_strdup_printf("Widget #%d", count);
    
    /* Pick a widget and put the text in it (unless it's a GtkImage) */
    switch(g_random_int_range(0, 7)) {
    case 0:
        widget = gtk_image_new_from_stock(GTK_STOCK_NEW, g_random_int_range(1, 6));
        break;
    case 1:
        widget = gtk_label_new(text);
        break;
    case 2:
        widget = gtk_button_new_with_label(text);
        break;
    case 3:
        widget = gtk_check_button_new_with_label(text);
        break;
    case 4:
        widget = gtk_entry_new();
        gtk_entry_set_text(GTK_ENTRY(widget), text);
        break;
    case 5:
    {
        GtkObject *adjustment = gtk_adjustment_new(count, 0, count, 1, 1, 0);
        widget = gtk_spin_button_new(GTK_ADJUSTMENT(adjustment), 1, 0);
    }
        break;
    case 6:
        widget = gtk_combo_box_new_text();
        gtk_combo_box_append_text(GTK_COMBO_BOX(widget), text);
        gtk_combo_box_append_text(GTK_COMBO_BOX(widget), text);
        gtk_combo_box_set_active(GTK_COMBO_BOX(widget), 0);
        break;
    }
    g_free(text);
    
    /* Show the widget and add it to our container */
    gtk_widget_show(widget);
    gtk_container_add(GTK_CONTAINER(square), widget);
}

/* When the user clicks the Remove button, simply remove the widget that was last added to the PSquare (if there was one) */
static void
remove_clicked(GtkToolButton *button, GtkWidget *square)
{
    GList *children = gtk_container_get_children(GTK_CONTAINER(square));
    GList *last = g_list_last(children);
    
    if(last)
        gtk_container_remove(GTK_CONTAINER(square), last->data);
    
    g_list_free(children);
}

int
main(int argc, char *argv[])
{
    GtkWidget *window, *vbox, *toolbar, *square;
    GtkToolItem *add, *remove;
    
    gtk_init(&argc, &argv);
    
    /* Build the main window */
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "PSquare Test Program");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    gtk_widget_set_size_request(window, 400, 400);
    
    /* Build the various other widgets */
    vbox = gtk_vbox_new(FALSE, 10);
    toolbar = gtk_toolbar_new();
    add = gtk_tool_button_new_from_stock(GTK_STOCK_ADD);
    remove = gtk_tool_button_new_from_stock(GTK_STOCK_REMOVE);
    square = p_square_new();
    
    /* Connect signals */
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(add, "clicked", G_CALLBACK(add_clicked), square);
    g_signal_connect(remove, "clicked", G_CALLBACK(remove_clicked), square);
    
    /* Put all the widgets together */
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), add, -1);
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), remove, -1);
    gtk_box_pack_start(GTK_BOX(vbox), toolbar, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), square, TRUE, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);
    gtk_widget_show_all(window);
    
    /* Run the program */
    gtk_main();
    return 0;
}
