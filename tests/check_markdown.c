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

#include <glib.h>
#include <balde.h>
#include <baldepress/markdown-private.h>


void
test_parse_markdown(void)
{
    balde_app_t *app = balde_app_init();
    gchar *mkd = parse_markdown(app, "### Foo ###\n");
    g_assert_cmpstr(mkd, ==, "<h3>Foo</h3>");
    g_free(mkd);
    balde_app_free(app);
}


int
main(int argc, char** argv)
{
    g_test_init(&argc, &argv, NULL);
    g_test_add_func("/markdown/parse_markdown", test_parse_markdown);
    return g_test_run();
}
