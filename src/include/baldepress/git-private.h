/*
 * baldepress: A Git-based blogging engine built with balde.
 * Copyright (C) 2014 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 *
 * This program can be distributed under the terms of the LGPL-2 License.
 * See the file COPYING.
 */

#ifndef _BALDEPRESS_GIT_PRIVATE_H
#define _BALDEPRESS_GIT_PRIVATE_H

#include <git2.h>
#include <glib.h>

typedef struct {
    gchar *path;
    GString *content;
    GDateTime *date;
    GDateTime *mdate;
    gchar *author;
} baldepress_git_filectx_t;

typedef struct {
    git_oid *revision_id;
    GSList *files;
} baldepress_git_changectx_t;

typedef struct {
    gchar *path;
    gchar *branch;
    balde_press_git_changectx_t *changectx;
} baldepress_git_repository_t;

#endif /* _BALDEPRESS_GIT_PRIVATE_H */
