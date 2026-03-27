/*
 * libosinfo: A collection of utilities used for medias & trees
 *
 * Copyright (C) 2019-2020 Red Hat, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include "osinfo_util_private.h"

gboolean
osinfo_util_requires_soup(const gchar *location)
{
    const gchar *prefixes[] = { "http://", "https://", NULL };
    gsize i;

    for (i = 0; prefixes[i] != NULL; i++) {
        if (g_str_has_prefix(location, prefixes[i]))
            return TRUE;
    }

    return FALSE;
}

#if SOUP_MAJOR_VERSION < 3
static void
soup_session_send_and_read_ready(SoupSession *session,
                                 GAsyncResult *result,
                                 GTask *task)
{
    GOutputStream *ostream = g_task_get_task_data(task);
    GInputStream *istream = NULL;
    GError *error = NULL;
    gssize size = 0;

    istream = soup_session_send_finish(session, result, &error);
    if (!istream) {
        g_task_return_error(task, error);
        g_object_unref(task);
        return;
    }

    size = g_output_stream_splice(ostream,
                                  istream,
                                  G_OUTPUT_STREAM_SPLICE_CLOSE_SOURCE |
                                  G_OUTPUT_STREAM_SPLICE_CLOSE_TARGET,
                                  g_task_get_cancellable(task),
                                  &error);
    if (size < 0) {
        g_task_return_error(task, error);
        g_object_unref(task);
        return;
    }

    g_task_return_pointer(task,
                          g_memory_output_stream_steal_as_bytes(G_MEMORY_OUTPUT_STREAM(ostream)),
                          (GDestroyNotify)g_bytes_unref);
    g_object_unref(task);
}

void
soup_session_send_and_read_async(SoupSession *session,
                                 SoupMessage *message,
                                 int priority,
                                 GCancellable *cancellable,
                                 GAsyncReadyCallback callback,
                                 gpointer user_data)
{
    GTask *task = NULL;
    GOutputStream *ostream = NULL;

    g_return_if_fail(SOUP_IS_SESSION(session));
    g_return_if_fail(SOUP_IS_MESSAGE(message));

    ostream = g_memory_output_stream_new_resizable();
    task = g_task_new(session, cancellable, callback, user_data);
    g_task_set_priority(task, priority);
    g_task_set_task_data(task, ostream, g_object_unref);

    soup_session_send_async(session,
                            message,
                            g_task_get_cancellable(task),
                            (GAsyncReadyCallback)soup_session_send_and_read_ready,
                            task);
}

GBytes *
soup_session_send_and_read_finish(SoupSession *session,
                                  GAsyncResult *result,
                                  GError **error)
{
    g_return_val_if_fail(SOUP_IS_SESSION(session), NULL);
    g_return_val_if_fail(g_task_is_valid(result, session), NULL);

    return g_task_propagate_pointer(G_TASK(result), error);
}
#endif
