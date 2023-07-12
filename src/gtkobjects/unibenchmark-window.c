/* unibenchmark-window.c
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

#include "unibenchmark-config.h"
#include "unibenchmark-window.h"

#include <math.h>
#include "utils/benchmark.h"
#include "utils/system-info.h"

struct _UnibenchmarkWindow {
    GtkApplicationWindow  parent_instance;

    /* Template widgets */
    GtkButton           *btn_test;
    GtkProgressBar      *progress;

    GtkEntry            *entry_tiempo;
    GtkEntry            *entry_productividad;
    GtkEntry            *entry_rendimiento;

    InfoUnibenchmark    *os;
    InfoUnibenchmark    *cpu;
    InfoUnibenchmark    *ram;
    InfoUnibenchmark    *disco;
};

G_DEFINE_TYPE(UnibenchmarkWindow, unibenchmark_window, GTK_TYPE_APPLICATION_WINDOW)

static GMutex mutex;

static void*
test(UnibenchmarkWindow *self) {
    char str[100];
    benchmark_info info = {
        .seg = 1,
        .ciclos = 0,
    };

    g_mutex_lock(&mutex);
    gtk_widget_set_sensitive(GTK_WIDGET(self->btn_test), FALSE);
    gtk_progress_bar_set_fraction(self->progress, 0.0f);

    gtk_entry_set_text(self->entry_tiempo, "");
    gtk_entry_set_text(self->entry_rendimiento, "");
    gtk_entry_set_text(self->entry_productividad, "");

    benchmark(&info, self->progress);

    g_snprintf(str, sizeof(str) / sizeof(char), "%f segundos", info.seg);
    gtk_entry_set_text(self->entry_tiempo, str);

    g_snprintf(str, sizeof(str) / sizeof(char), "%f", 1.0f / info.seg);
    gtk_entry_set_text(self->entry_rendimiento, str);

    g_snprintf(str, sizeof(str) / sizeof(char), "%f MCPS", info.ciclos / (info.seg * pow(10, 6)));
    gtk_entry_set_text(self->entry_productividad, str);

    gtk_widget_set_sensitive(GTK_WIDGET (self->btn_test), TRUE);
    g_mutex_unlock(&mutex);

    return NULL;
}

static void
on_click_btn_test(G_GNUC_UNUSED GtkButton *btn, UnibenchmarkWindow *self) {
    g_thread_new("execute", (GThreadFunc) test, self);
}

/*enum {
  DIALOG,
  N_SIGNAL,
};

static gint unibenchmark_window_signals[N_SIGNAL] = { 0, };*/

static void
unibenchmark_window_class_init(UnibenchmarkWindowClass *klass) {
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
    info_unibenchmark_get_type();

    gtk_widget_class_set_template_from_resource(widget_class, "/org/gnome/unibenchmark/templates/unibenchmark-window.ui");
    gtk_widget_class_bind_template_child(widget_class, UnibenchmarkWindow, btn_test);

    gtk_widget_class_bind_template_child(widget_class, UnibenchmarkWindow, entry_tiempo);
    gtk_widget_class_bind_template_child(widget_class, UnibenchmarkWindow, entry_productividad);
    gtk_widget_class_bind_template_child(widget_class, UnibenchmarkWindow, entry_rendimiento);

    gtk_widget_class_bind_template_child(widget_class, UnibenchmarkWindow, os);
    gtk_widget_class_bind_template_child(widget_class, UnibenchmarkWindow, cpu);
    gtk_widget_class_bind_template_child(widget_class, UnibenchmarkWindow, ram);
    gtk_widget_class_bind_template_child(widget_class, UnibenchmarkWindow, disco);
    gtk_widget_class_bind_template_child(widget_class, UnibenchmarkWindow, progress);

    gtk_widget_class_bind_template_callback(widget_class, on_click_btn_test);

    /*unibenchmark_window_signals[DIALOG] = g_signal_new ("dialog",  //name
    UNIBENCHMARK_TYPE_WINDOW,                                    //class type
    G_SIGNAL_RUN_FIRST,                                          //signal flags
    G_STRUCT_OFFSET (UnibenchmarkWindowClass, _dialog),               //class_offset
    NULL,                                                        //accumulator
    NULL,                                                        //accumulator data
    g_cclosure_marshal_VOID__VOID,                               //marshaller
    G_TYPE_NONE,                                                 //type of return value
    0, NULL);

    klass->_dialog = NULL;*/
}

static void
unibenchmark_window_init(UnibenchmarkWindow *self) {
    system_info info;

    gtk_widget_init_template(GTK_WIDGET(self));

    get_info(&info);
    info_unibenchmark_set_label(self->os, info.osName);
    info_unibenchmark_set_label(self->cpu, info.cpu);
    info_unibenchmark_set_label (self->ram, info.ram);
    info_unibenchmark_set_label(self->disco, info.disc);
    system_info_free(&info);
}
