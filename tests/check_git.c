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

#include <git2.h>
#include <glib.h>
#include <balde.h>
#include <baldepress/git-private.h>


typedef struct {
    gchar *git_dir;
} baldepress_git_fixture_t;


void
baldepress_git_test_setup(baldepress_git_fixture_t *f, gconstpointer  data)
{
    f->git_dir = g_build_filename(g_get_tmp_dir(), "test.git.XXXXXX", NULL);
    f->git_dir = g_mkdtemp(f->git_dir);
}


void
baldepress_git_test_teardown(baldepress_git_fixture_t *f, gconstpointer  data)
{
    gchar *cmd = g_strdup_printf("rm -rf '%s'", f->git_dir);
    gint ret = system(cmd);
    g_assert(ret == 0);
    g_free(cmd);
    g_free(f->git_dir);
}


void
baldepress_git_test_runner(baldepress_git_fixture_t *f, gconstpointer  data)
{
    ((void (*) (const gchar *git_dir)) data) (f->git_dir);
}


git_repository*
baldepress_git_create_repository(const gchar *git_dir, gboolean bare)
{
    git_repository *repo = NULL;
    git_repository_init(&repo, git_dir, bare);
    return repo;
}


void
baldepress_git_add_file(git_repository *repo, const gchar *file_path,
    const gchar *contents)
{
    gchar *dirname = g_path_get_dirname(file_path);
    g_mkdir_with_parents(dirname, 0755);
    g_free(dirname);
    g_file_set_contents(file_path, contents, -1, NULL);
}


git_repository*
baldepress_git_create_repository_with_files(const gchar *git_dir)
{
    git_repository *repo = baldepress_git_create_repository(git_dir, FALSE);
    gchar *f = g_build_filename(git_dir, "balde.txt", NULL);
    baldepress_git_add_file(repo, f, "balde");
    g_free(f);
    f = g_build_filename(git_dir, "chunda.txt", NULL);
    baldepress_git_add_file(repo, f, "chunda");
    g_free(f);
    f = g_build_filename(git_dir, "bola", "moises.txt", NULL);
    baldepress_git_add_file(repo, f, "bola moises");
    g_free(f);
    f = g_build_filename(git_dir, "bola", "guda", "arcoiro.txt", NULL);
    baldepress_git_add_file(repo, f, "bola guda arcoiro");
    g_free(f);
    return repo;
}


void
baldepress_git_assert_filectx(baldepress_git_filectx_t *fctx, const gchar *path,
    const gchar *content, gchar *author, GDateTime *date, GDateTime *mdate)
{
    g_assert_cmpstr(fctx->path, ==, path);
    g_assert_cmpstr(fctx->content->str, ==, content);
    g_assert_cmpstr(fctx->author, ==, author);
    if (date == NULL)
        g_assert(fctx->date == NULL);
    else
        g_assert(g_date_time_compare(fctx->date, date) == 0);
    if (mdate == NULL)
        g_assert(fctx->mdate == NULL);
    else
        g_assert(g_date_time_compare(fctx->mdate, mdate) == 0);
}


void
test_git_load_filectx_from_index_not_added(const gchar *git_dir)
{
    git_repository *repo = baldepress_git_create_repository_with_files(git_dir);
    balde_app_t *app = balde_app_init();
    GSList *l = baldepress_git_load_filectx_from_index(app, repo);
    g_assert(l == NULL);
    g_assert(app->error == NULL);
    balde_app_free(app);
    git_repository_free(repo);
}


void
test_git_load_filectx_from_index(const gchar *git_dir)
{
    git_repository *repo = baldepress_git_create_repository_with_files(git_dir);
    const gchar *paths[] = {"*"};
    git_strarray arr = {(char**) paths, 1};
    git_index *index = NULL;
    git_repository_index(&index, repo);
    git_index_add_all(index, &arr, GIT_INDEX_ADD_DEFAULT, NULL, NULL);
    balde_app_t *app = balde_app_init();
    GSList *l = baldepress_git_load_filectx_from_index(app, repo);
    baldepress_git_assert_filectx(l->data, "balde.txt", "balde", NULL, NULL, NULL);
    baldepress_git_assert_filectx(l->next->data, "bola/guda/arcoiro.txt",
        "bola guda arcoiro", NULL, NULL, NULL);
    baldepress_git_assert_filectx(l->next->next->data, "bola/moises.txt",
        "bola moises", NULL, NULL, NULL);
    baldepress_git_assert_filectx(l->next->next->next->data, "chunda.txt",
        "chunda", NULL, NULL, NULL);
    g_assert(l->next->next->next->next == NULL);
    g_assert(app->error == NULL);
    balde_app_free(app);
    git_index_free(index);
    git_repository_free(repo);
}


int
main(int argc, char** argv)
{
    g_test_init(&argc, &argv, NULL);
    g_test_add("/git/load_filectx_from_index_not_added", baldepress_git_fixture_t,
        test_git_load_filectx_from_index_not_added, baldepress_git_test_setup,
        baldepress_git_test_runner, baldepress_git_test_teardown);
    g_test_add("/git/load_filectx_from_index", baldepress_git_fixture_t,
        test_git_load_filectx_from_index, baldepress_git_test_setup,
        baldepress_git_test_runner, baldepress_git_test_teardown);
    return g_test_run();
}
