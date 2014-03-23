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
} bp_git_fixture_t;


void
bp_git_test_setup(bp_git_fixture_t *f, gconstpointer  data)
{
    f->git_dir = g_build_filename(g_get_tmp_dir(), "test.git.XXXXXX", NULL);
    f->git_dir = g_mkdtemp(f->git_dir);
}


void
bp_git_test_teardown(bp_git_fixture_t *f, gconstpointer  data)
{
    gchar *cmd = g_strdup_printf("rm -rf '%s'", f->git_dir);
    gint ret = system(cmd);
    g_assert(ret == 0);
    g_free(cmd);
    g_free(f->git_dir);
}


void
bp_git_test_runner(bp_git_fixture_t *f, gconstpointer  data)
{
    ((void (*) (const gchar *git_dir)) data) (f->git_dir);
}


#define bp_git_test_add(path, ftest) \
    g_test_add("/git/" path, bp_git_fixture_t, (gpointer) test_git_##ftest, \
        bp_git_test_setup, bp_git_test_runner, bp_git_test_teardown)


void
bp_git_add_file(git_repository *repo, const gchar *file_path,
    const gchar *contents)
{
    gchar *dirname = g_path_get_dirname(file_path);
    g_mkdir_with_parents(dirname, 0755);
    g_free(dirname);
    g_file_set_contents(file_path, contents, -1, NULL);
}


git_repository*
bp_git_create_repository(const gchar *git_dir, gboolean bare)
{
    git_repository *repo = NULL;
    git_repository_init(&repo, git_dir, bare);
    return repo;
}


git_repository*
bp_git_create_repository_with_files(const gchar *git_dir)
{
    git_repository *repo = bp_git_create_repository(git_dir, FALSE);
    gchar *f = g_build_filename(git_dir, "balde.txt", NULL);
    bp_git_add_file(repo, f, "balde");
    g_free(f);
    f = g_build_filename(git_dir, "chunda.txt", NULL);
    bp_git_add_file(repo, f, "chunda");
    g_free(f);
    f = g_build_filename(git_dir, "bola", "moises.txt", NULL);
    bp_git_add_file(repo, f, "bola moises");
    g_free(f);
    f = g_build_filename(git_dir, "bola", "guda", "arcoiro.txt", NULL);
    bp_git_add_file(repo, f, "bola guda arcoiro");
    g_free(f);
    return repo;
}


void
bp_git_assert_filectx(bp_git_filectx_t *fctx, const gchar *path,
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
test_git_open_repository(const gchar *git_dir)
{
    git_repository *repo_created = bp_git_create_repository(git_dir, FALSE);
    g_assert(repo_created != NULL);
    balde_app_t *app = balde_app_init();
    git_repository *repo = bp_git_open_repository(app, git_dir);
    g_assert(repo != NULL);
    g_assert(app->error == NULL);
    g_assert_cmpstr(git_repository_path(repo_created), ==,
        git_repository_path(repo));
    git_repository_free(repo_created);
    git_repository_free(repo);
    balde_app_free(app);
}


int
main(int argc, char** argv)
{
    g_test_init(&argc, &argv, NULL);
    bp_git_test_add("open_repository", open_repository);
    return g_test_run();
}
