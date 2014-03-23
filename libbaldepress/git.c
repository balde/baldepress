/*
 * baldepress: A Git-based blogging engine built with balde.
 * Copyright (C) 2014 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 *
 * This program can be distributed under the terms of the LGPL-2 License.
 * See the file COPYING.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <balde.h>
#include <git2.h>
#include <glib.h>

#include <baldepress/git-private.h>


void
bp_git_check_error(balde_app_t *app, int error_code, const gchar *message)
{
    if (error_code == 0)
        return;
    const git_error *g_error = giterr_last();
    gchar *tmp = g_strdup_printf("Git: %s - %s", message,
        (g_error != NULL && g_error->message != NULL) ?
        g_error->message : "Unknown error");
    balde_abort_set_error_with_description(app, 500, tmp);
    g_free(tmp);
}


git_repository*
bp_git_open_repository(balde_app_t *app, const gchar *path)
{
    g_return_val_if_fail(app->error == NULL, NULL);
    git_repository *repo = NULL;
    bp_git_check_error(app, git_repository_open_ext(&repo, path, 0, NULL),
        "Open repository");
    return repo;
}
