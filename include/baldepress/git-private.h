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
} bp_git_filectx_t;

typedef struct {
    git_oid *revision_id;
    GSList *files;
} bp_git_changectx_t;

typedef struct {
    gchar *path;
    gchar *branch;
    bp_git_changectx_t *changectx;
} bp_git_repository_t;

void bp_git_check_error(balde_app_t *app, int error_code, const gchar *message);
git_repository* bp_git_open_repository(balde_app_t *app, const gchar *path);
git_reference* bp_git_get_reference_from_branch(balde_app_t *app,
    git_repository *repo, const gchar *branch);

#endif /* _BALDEPRESS_GIT_PRIVATE_H */
