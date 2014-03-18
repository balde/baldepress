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
#include <mkdio.h>
#include <string.h>

#include <baldepress/markdown-private.h>


gchar*
parse_markdown(balde_app_t *app, const gchar *mkd_source)
{
    gchar *rv = NULL;
    gchar *text;
    MMIOT *doc = mkd_string(mkd_source, strlen(mkd_source), MKD_TABSTOP);
    if (doc == NULL) {
        balde_abort_set_error(app, 500);
        goto point0;
    }
    mkd_compile(doc, MKD_TABSTOP);
    mkd_document(doc, &text);
    rv = g_strdup(text);
point0:
    mkd_cleanup(doc);
    return rv;
}
