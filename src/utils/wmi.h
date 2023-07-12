/* wmi.h
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

#include <glib/gi18n.h>
#include <math.h>
#include <wchar.h>

#ifdef _WIN32
#include <windows.h>
#include <wbemidl.h>
#include <stdlib.h>
#endif

typedef void (*QueryFunc) (IEnumWbemClassObject *, char *, gsize);

void wmi_query(wchar_t *query, QueryFunc func, char *str, gsize size);
void wmi_os(IEnumWbemClassObject *results, char *str, gsize size);
void wmi_cpu(IEnumWbemClassObject *results, char *str, gsize size);
void wmi_ram(IEnumWbemClassObject *results, char *str, gsize size);
void wmi_disc(IEnumWbemClassObject *results, char *str, gsize size);
