/* info-unibenchmark.c
 *
 * Copyright 2023 OrbitalMk
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "info-unibenchmark.h"

struct _InfoUnibenchmark {
    GtkGrid  parent_instance;

    gchar               *title;
    gchar               *icon;
    gchar               *label;

    /* Template widgets */
    GtkLabel            *label_info;
    GtkImage            *icon_info;
    GtkLabel            *title_info;
};

G_DEFINE_TYPE(InfoUnibenchmark, info_unibenchmark, GTK_TYPE_GRID)

enum {
    PROP_ICON = 1,
    PROP_TITLE,
    PROP_LABEL,
    N_PROPERTIES,
};

static GParamSpec *properties[N_PROPERTIES] = { NULL, };

static void
info_unibenchmark_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec) {
    InfoUnibenchmark *self = INFO_UNIBENCHMARK(object);

    switch (property_id)
    {
        case PROP_ICON:
            g_free(self->icon);
            self->icon = g_value_dup_string(value);
            gtk_image_set_from_resource(self->icon_info, self->icon);
            break;

        case PROP_TITLE:
            g_free(self->title);
            self->title = g_value_dup_string(value);
            gtk_label_set_label(GTK_LABEL(self->title_info), self->title);
            break;

        case PROP_LABEL:
            g_free(self->label);
            self->label = g_value_dup_string(value);
            gtk_label_set_label(GTK_LABEL(self->label_info), self->label);
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
            break;
    }
}

static void
info_unibenchmark_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec) {
    InfoUnibenchmark *self = INFO_UNIBENCHMARK(object);
    
    switch (property_id)
    {
        case PROP_ICON:
            g_value_set_string(value, self->icon);
            break;

        case PROP_TITLE:
            g_value_set_string(value, self->title);
            break;

        case PROP_LABEL:
            g_value_set_string(value, self->label);
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
            break;
    }
}

static void
info_unibenchmark_class_init(InfoUnibenchmarkClass *klass) {
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
    GObjectClass *object_class = G_OBJECT_CLASS(klass);

    gtk_widget_class_set_template_from_resource(widget_class, "/org/gnome/unibenchmark/templates/info-unibenchmark.ui");
    gtk_widget_class_bind_template_child(widget_class, InfoUnibenchmark, label_info);
    gtk_widget_class_bind_template_child(widget_class, InfoUnibenchmark, title_info);
    gtk_widget_class_bind_template_child(widget_class, InfoUnibenchmark, icon_info);

    object_class->set_property = info_unibenchmark_set_property;
    object_class->get_property = info_unibenchmark_get_property;

    properties[PROP_ICON] = g_param_spec_string("icon",
        "Icon", "Icon of the info", NULL, G_PARAM_READWRITE);

    properties[PROP_TITLE] = g_param_spec_string("title",
        "Title", "Title of the info", NULL, G_PARAM_READWRITE);

    properties[PROP_LABEL] = g_param_spec_string("label",
        "Label", "Label of the info", NULL, G_PARAM_READWRITE);

    g_object_class_install_properties(object_class, N_PROPERTIES, properties);
}

static void
info_unibenchmark_init(InfoUnibenchmark *self) {
    gtk_widget_init_template (GTK_WIDGET (self));
}

void
info_unibenchmark_set_label(InfoUnibenchmark *self, const gchar *label) {
    GValue value = G_VALUE_INIT;

    g_value_init(&value, G_TYPE_STRING);
    g_value_set_string(&value, label);

    g_object_set_property(G_OBJECT(self), "label", &value);

    g_value_unset(&value);
}
