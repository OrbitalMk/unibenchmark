/* system-info.c
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

#include "system-info.h"

#include <glib/gi18n.h>
#include <math.h>
#include <wchar.h>

#ifdef _WIN32
#include <windows.h>
#include <wbemidl.h>
#include <stdlib.h>
#include "wmi.h"
#else
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <sys/statvfs.h>
#endif

static char*
get_os() {
    size_t size = 1024;
    char* str = g_malloc0(sizeof(char) * size);

    #ifdef _WIN32
    wmi_query(L"SELECT * FROM Win32_OperatingSystem", wmi_os, str, 1024);
    #else
    #warning Function is only implemented on Windows
    #endif

    return str;
}

static char*
get_cpu() {
    size_t size = 1024;
    char *str = g_malloc0 (sizeof(char) * size);

    #ifdef _WIN32
    wmi_query(L"SELECT * FROM Win32_Processor", wmi_cpu, str, 1024);
    #else
    #warning Function is only implemented on Windows
    #endif

    return str;
}

static char*
get_ram() {
    size_t size = 1024;
    char* str = g_malloc0 (sizeof(char) * size);

    #ifdef _WIN32
    wmi_query(L"SELECT * FROM Win32_PhysicalMemory", wmi_ram, str, 1024);
    #else
    #warning Function is only implemented on Windows
    #endif

    return str;
}

static char*
get_disc() {
    size_t size = 1024;
    char* str = g_malloc0 (sizeof(char) * size);

    #ifdef _WIN32
    wmi_query(L"SELECT * FROM Win32_LogicalDisk WHERE DeviceId='C:'", wmi_disc, str, 1024);
    #else
    #warning Function is only implemented on Windows
    #endif

    return str;
}

void
get_info(system_info* info) {
    info->osName = get_os();
    info->cpu = get_cpu();
    info->ram = get_ram();
    info->disc = get_disc();
}

void
system_info_free(system_info* info) {
  g_free(info->osName);
  g_free(info->cpu);
  g_free(info->ram);
  g_free(info->disc);
}