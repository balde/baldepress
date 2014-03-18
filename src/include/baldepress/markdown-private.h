/*
 * baldepress: A Git-based blogging engine built with balde.
 * Copyright (C) 2014 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 *
 * This program can be distributed under the terms of the LGPL-2 License.
 * See the file COPYING.
 */

#ifndef _BALDEPRESS_MARKDOWN
#define _BALDEPRESS_MARKDOWN

#include <glib.h>
#include <balde.h>

gchar* parse_markdown(balde_app_t *app, const gchar *mkd_source);

#endif
