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
baldepress_git_check_error(balde_app_t *app, int error_code, const gchar *message)
{
    g_return_if_fail(error_code != 0);
    const git_error *g_error = giterr_last();
    gchar *tmp = g_strdup_printf("Git: %s - %s", message,
        (g_error != NULL && g_error->message != NULL) ?
        g_error->message : "Unknown error");
    balde_abort_set_error_with_description(app, 500, tmp);
    g_free(tmp);
}


git_repository*
baldepress_git_open_repository(balde_app_t *app, const gchar *path)
{
    g_return_val_if_fail(app->error == NULL, NULL);
    git_repository *repo = NULL;
    baldepress_git_check_error(app, git_repository_open_ext(&repo, path, 0, NULL),
        "Open repository");
    g_return_val_if_fail(app->error == NULL, NULL);
    return repo;
}


GSList*
baldepress_git_load_filectx_from_index(balde_app_t *app, git_repository *repo)
{
    g_return_val_if_fail(app->error == NULL, NULL);
    GSList *files = NULL;
    git_index *index = NULL;
    baldepress_git_check_error(app, git_repository_index(&index, repo),
        "Get repository index");
    g_return_val_if_fail(app->error == NULL, NULL);
    baldepress_git_check_error(app, git_index_read(index, 0),
        "Read repository index");
    if (app->error != NULL)
        goto err1;
    for (guint i = 0; i < git_index_entrycount(index); i++) {
        const git_index_entry* tmp = git_index_get_byindex(index, i);
        git_blob *blob = NULL;
        baldepress_git_check_error(app, git_blob_lookup(&blob, repo, &(tmp->oid)),
            "Object lookup");
        if (app->error != NULL)
            goto err1;
        baldepress_git_filectx_t *fctx = g_new(baldepress_git_filectx_t, 1);
        fctx->path = g_strdup(tmp->path);
        fctx->content = g_string_new_len((gchar*) git_blob_rawcontent(blob),
            (gssize) git_blob_rawsize(blob));
        files = g_slist_append(files, fctx);
        git_blob_free(blob);
    }

err1:
    if (index != NULL)
        git_index_free(index);
    return files;
}
