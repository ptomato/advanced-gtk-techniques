#include <gtk/gtk.h>
#include <math.h>
#include "psquare.h"

/* Private class member */
#define P_SQUARE_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), \
	P_SQUARE_TYPE, PSquarePrivate))
                                    
typedef struct _PSquarePrivate PSquarePrivate;

struct _PSquarePrivate
{
    GList *children;
};

/* Forward declarations */
static void p_square_size_request(GtkWidget *widget, GtkRequisition *requisition);
static void p_square_size_allocate(GtkWidget *widget, GtkAllocation *allocation);
static GType p_square_child_type(GtkContainer *container);
static void p_square_add(GtkContainer *container, GtkWidget *widget);
static void p_square_remove(GtkContainer *container, GtkWidget *widget);
static void p_square_forall(GtkContainer *container, gboolean include_internals, GtkCallback callback, gpointer callback_data);

/* Define the PSquare type and inherit from GtkContainer */
G_DEFINE_TYPE(PSquare, p_square, GTK_TYPE_CONTAINER);

/* Initialize the PSquare class */
static void
p_square_class_init(PSquareClass *klass)
{    
    /* Override GtkWidget methods */
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
    widget_class->size_request = p_square_size_request;
    widget_class->size_allocate = p_square_size_allocate;
    
    /* Override GtkContainer methods */
    GtkContainerClass *container_class = GTK_CONTAINER_CLASS(klass);
    container_class->child_type = p_square_child_type;
    container_class->add = p_square_add;
    container_class->remove = p_square_remove;
    container_class->forall = p_square_forall;
    
    /* Add private indirection member */
    g_type_class_add_private(klass, sizeof(PSquarePrivate));
}

/* Initialize a new PSquare instance */
static void
p_square_init(PSquare *square)
{
    /* This means that PSquare doesn't supply its own GdkWindow */
    GTK_WIDGET_SET_FLAGS(GTK_WIDGET(square), GTK_NO_WINDOW);
    /* Set redraw on allocate to FALSE if the top left corner of your widget doesn't change when it's resized; this saves time */
    /*gtk_widget_set_redraw_on_allocate(GTK_WIDGET(square), FALSE);*/

    /* Initialize private members */
    PSquarePrivate *priv = P_SQUARE_PRIVATE(square);
    priv->children = NULL;
}

/* Return a new PSquare cast to a GtkWidget */
GtkWidget *
p_square_new()
{
    return GTK_WIDGET(g_object_new(p_square_get_type(), NULL));
}

/* Get the total size request of the container */
static void
p_square_size_request(GtkWidget *widget, GtkRequisition *requisition)
{
    g_return_if_fail(widget != NULL || requisition != NULL);
    g_return_if_fail(P_IS_SQUARE(widget));
    
    PSquarePrivate *priv = P_SQUARE_PRIVATE(widget);
        
    /* Start with the container's border width */
    requisition->width = GTK_CONTAINER(widget)->border_width * 2;
    requisition->height = GTK_CONTAINER(widget)->border_width * 2;
    
    /* Count the visible children */
    GList *iter;
    unsigned n_visible_children = 0;
    for(iter = priv->children; iter; iter = g_list_next(iter))
        if(GTK_WIDGET_VISIBLE(iter->data))
            n_visible_children++;
    
    if(n_visible_children == 0)
        return;
    
    /* Calculate the number of columns (and rows) */
    int n_columns = (int)ceil(sqrt((double)n_visible_children));
    
    /* Allocate arrays for the width of each column 
     * and the height of each row */
    int *width = g_new0(int, n_columns);
    int *height = g_new0(int, n_columns);
    
    /* Get each child's size request; set the width of each column
     * to the maximum width of each child in that column,
     * and the height of each row to the maximum height of each
     * child in that row */
    int count = 0;
    for(iter = priv->children; iter; iter = g_list_next(iter)) {
        if(!GTK_WIDGET_VISIBLE(iter->data))
            continue;
        
        GtkRequisition child_requisition;
        gtk_widget_size_request(iter->data, &child_requisition);
        
        width[count % n_columns] = 
            MAX(child_requisition.width, width[count % n_columns]);
        height[count / n_columns] = 
            MAX(child_requisition.height, height[count / n_columns]);
        
        count++;
    }    

    /* Add the widths and heights and pass that
     * as the container's size request */
    for(count = 0; count < n_columns; count++) {
        requisition->width += width[count];
        requisition->height += height[count];
    }
    
    g_free(width);
    g_free(height);
}

/* Allocate the sizes of the container's children */
static void
p_square_size_allocate(GtkWidget *widget, GtkAllocation *allocation)
{
    g_return_if_fail(widget != NULL || allocation != NULL);
    g_return_if_fail(P_IS_SQUARE(widget));
 
    PSquarePrivate *priv = P_SQUARE_PRIVATE(widget);
    
    widget->allocation = *allocation;
    
    /* Count the visible children */
    GList *iter;
    unsigned n_visible_children = 0;
    for(iter = priv->children; iter; iter = g_list_next(iter))
        if(GTK_WIDGET_VISIBLE(iter->data))
            n_visible_children++;
    
    if(n_visible_children == 0)
        return;
    
    /* Calculate the number of columns (and rows) */
    int n_columns = (gint)ceil(sqrt((double)n_visible_children));
    
    /* Allocate arrays for the width of each column 
     * and the height of each row */
    int *width = g_new0(int, n_columns);
    int *height = g_new0(int, n_columns);
    
    /* Follow the same procedure as in the size request to get 
     * the ideal sizes of each row and column */
    int count = 0;
    for(iter = priv->children; iter; iter = g_list_next(iter)) {
        if(!GTK_WIDGET_VISIBLE(iter->data))
            continue;
        
        GtkRequisition child_requisition;
        gtk_widget_get_child_requisition(iter->data, &child_requisition);
        
        width[count % n_columns] = 
            MAX(child_requisition.width, width[count % n_columns]);
        height[count / n_columns] =
            MAX(child_requisition.height, height[count / n_columns]);
        
        count++;
    }
    
    /* Calculate the extra space per column and row (can be negative) */
    int extra_width = 
        (allocation->width - widget->requisition.width) / n_columns;
    int extra_height = 
        (allocation->height - widget->requisition.height) / n_columns;
    
    /* Distribute the surplus or shortage of space
     * equally between columns */
    for(count = 0; count < n_columns; count++) {
        width[count] += extra_width;
        /* If this results in a negative width, redistribute
         * pixels from other nonzero-width columns to this one */
        if(width[count] < 0) {
            int count2;
            for(count2 = (count + 1) % n_columns;
                width[count] < 0;
                count2++, count2 %= n_columns) 
            {
                if(count2 == count || width[count2] < 0)
                    continue;
                width[count2]--;
                width[count]++;
            }
        }
        /* Do the same for the rows */
        height[count] += extra_height;
        if(height[count] < 0) {
            int count2;
            for(count2 = (count + 1) % n_columns;
                height[count] < 0;
                count2++, count2 %= n_columns)
            {
                if(count2 == count || height[count2] < 0)
                    continue;
                height[count2]--;
                height[count]++;
            }
        }
    }
    
    /* Start positioning the items at the container's origin,
     * less the border width */
    int x = allocation->x + GTK_CONTAINER(widget)->border_width;
    int y = allocation->y + GTK_CONTAINER(widget)->border_width;

    count = 0;
    for(iter = priv->children; iter; iter = g_list_next(iter)) {
        if(!GTK_WIDGET_VISIBLE(iter->data))
            continue;
        
        /* Give the child its allocation */
        GtkAllocation child_allocation;
        child_allocation.x = x;
        child_allocation.y = y;
        child_allocation.width = width[count % n_columns];
        child_allocation.height = height[count / n_columns];
        gtk_widget_size_allocate(iter->data, &child_allocation);
        
        /* Advance the x coordinate */
        x += child_allocation.width;
        count++;
        /* If we've moved to the next row, return the x coordinate 
         * to the left, and advance the y coordinate */
        if(count % n_columns == 0) {
            x = allocation->x + GTK_CONTAINER(widget)->border_width;
            y += child_allocation.height;
        }
    }
    
    g_free(width);
    g_free(height);
}

/* Return the type of children this container accepts */
static GType
p_square_child_type(GtkContainer *container)
{
    return GTK_TYPE_WIDGET;
}

/* Add a child to the container */
static void
p_square_add(GtkContainer *container, GtkWidget *widget)
{
    g_return_if_fail(container || P_IS_SQUARE(container));
    g_return_if_fail(widget || GTK_IS_WIDGET(widget));
    g_return_if_fail(widget->parent == NULL);
    
    PSquarePrivate *priv = P_SQUARE_PRIVATE(container);
    
    /* Add the child to our list of children. 
     * All the real work is done in gtk_widget_set_parent(). */
    priv->children = g_list_append(priv->children, widget);
    gtk_widget_set_parent(widget, GTK_WIDGET(container));
    
    /* Queue redraw */
    if(GTK_WIDGET_VISIBLE(widget))
        gtk_widget_queue_resize(GTK_WIDGET(container));
}

/* Remove a child from the container */
static void
p_square_remove(GtkContainer *container, GtkWidget *widget)
{
    g_return_if_fail(container || P_IS_SQUARE(container));
    g_return_if_fail(widget || GTK_IS_WIDGET(widget));
    
    PSquarePrivate *priv = P_SQUARE_PRIVATE(container);
    
    /* Remove the child from our list of children. 
     * Again, all the real work is done in gtk_widget_unparent(). */
    GList *link = g_list_find(priv->children, widget);
    if(link) {
        gboolean was_visible = GTK_WIDGET_VISIBLE(widget);
        gtk_widget_unparent(widget);

        priv->children = g_list_delete_link(priv->children, link);
            
        /* Queue redraw */
        if(was_visible)
            gtk_widget_queue_resize(GTK_WIDGET(container));
    }
}

/* Call the function for all the container's children. This function
 * ignores the include_internals argument, because there are no
 * "internal" children. */
static void 
p_square_forall(GtkContainer *container, gboolean include_internals,
                GtkCallback callback, gpointer callback_data)
{
    PSquarePrivate *priv = P_SQUARE_PRIVATE(container);
	g_list_foreach(priv->children, (GFunc)callback, callback_data);
}
