/* unibenchmark-window.h
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

#pragma once

#include <gtk/gtk.h>
#include "info-unibenchmark.h"

G_BEGIN_DECLS

#define UNIBENCHMARK_TYPE_WINDOW (unibenchmark_window_get_type())

GType unibenchmark_window_get_type(void);

G_GNUC_BEGIN_IGNORE_DEPRECATIONS
typedef struct _UnibenchmarkWindow UnibenchmarkWindow;

typedef struct {
    GtkApplicationWindowClass parent_class;

    // signals
    // void (*_dialog) (GtkWidget *, gpointer);
} UnibenchmarkWindowClass;

_GLIB_DEFINE_AUTOPTR_CHAINUP(UnibenchmarkWindow, GtkApplicationWindow)
G_DEFINE_AUTOPTR_CLEANUP_FUNC(UnibenchmarkWindowClass, g_type_class_unref)

G_GNUC_UNUSED static inline UnibenchmarkWindow * UNIBENCHMARK_WINDOW(gpointer ptr) {
    return G_TYPE_CHECK_INSTANCE_CAST(ptr, unibenchmark_window_get_type(), UnibenchmarkWindow);
}

G_GNUC_UNUSED static inline gboolean UNIBENCHMARK_IS_WINDOW(gpointer ptr) {
    return G_TYPE_CHECK_INSTANCE_TYPE(ptr, unibenchmark_window_get_type());
}
G_GNUC_END_IGNORE_DEPRECATIONS

G_END_DECLS
