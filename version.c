/**********************************************************************

  version.c -

  $Author$
  $Date$
  created at: Thu Sep 30 20:08:01 JST 1993

  Copyright (C) 1993-2003 Yukihiro Matsumoto

**********************************************************************/

#include "ruby.h"
#include "version.h"
#include <stdio.h>

#define PRINT(type) puts(TOKEN_PASTE(ruby_,type))
#ifndef rb_str_new_cstr
#define rb_str_new_cstr(str) rb_str_new(str, strlen(str))
#endif
#define MKSTR(type) rb_obj_freeze(rb_str_new_cstr(TOKEN_PASTE(ruby_,type)))

const char ruby_version[] = RUBY_VERSION;
const char ruby_release_date[] = RUBY_RELEASE_DATE;
const char ruby_platform[] = RUBY_PLATFORM;
const int ruby_patchlevel = RUBY_PATCHLEVEL;
#ifdef RUBY_DESCRIPTION
const char ruby_description[] = RUBY_DESCRIPTION;
#else
char ruby_description[
    sizeof("ruby  () []") +
    sizeof(RUBY_VERSION)-1 + sizeof(RUBY_PATCHLEVEL_STR)-1 +
    sizeof(RUBY_RELEASE_DATE)-1 + sizeof(RUBY_REVISION_STR)-1 +
    sizeof(RUBY_PLATFORM)-1];
#endif
#ifdef RUBY_COPYRIGHT
const char ruby_copyright[] = RUBY_COPYRIGHT;
#else
char ruby_copyright[
    sizeof("ruby - Copyright (C) - ") + 20 + sizeof(RUBY_AUTHOR)-1];
#endif

const struct ruby_initial_loadpath {
#ifdef RUBY_SEARCH_PATH
    char search_path[sizeof(RUBY_SEARCH_PATH)];
#endif
    char site_lib2[sizeof(RUBY_SITE_LIB2)];
#ifdef RUBY_SITE_THIN_ARCHLIB
    char site_thin_archlib[sizeof(RUBY_SITE_THIN_ARCHLIB)];
#endif
    char site_archlib[sizeof(RUBY_SITE_ARCHLIB)];
    char site_lib[sizeof(RUBY_SITE_LIB)];

    char vendor_lib2[sizeof(RUBY_VENDOR_LIB2)];
#ifdef RUBY_VENDOR_THIN_ARCHLIB
    char vendor_thin_archlib[sizeof(RUBY_VENDOR_THIN_ARCHLIB)];
#endif
    char vendor_archlib[sizeof(RUBY_VENDOR_ARCHLIB)];
    char vendor_lib[sizeof(RUBY_VENDOR_LIB)];

    char lib[sizeof(RUBY_LIB)];
#ifdef RUBY_THIN_ARCHLIB
    char thin_archlib[sizeof(RUBY_THIN_ARCHLIB)];
#endif
    char archlib[sizeof(RUBY_ARCHLIB)];
    char terminator[1];
} ruby_initial_load_paths = {
#ifdef RUBY_SEARCH_PATH
    RUBY_SEARCH_PATH,
#endif
    RUBY_SITE_LIB2,
#ifdef RUBY_SITE_THIN_ARCHLIB
    RUBY_SITE_THIN_ARCHLIB,
#endif
    RUBY_SITE_ARCHLIB,
    RUBY_SITE_LIB,

    RUBY_VENDOR_LIB2,
#ifdef RUBY_VENDOR_THIN_ARCHLIB
    RUBY_VENDOR_THIN_ARCHLIB,
#endif
    RUBY_VENDOR_ARCHLIB,
    RUBY_VENDOR_LIB,

    RUBY_LIB,
#ifdef RUBY_THIN_ARCHLIB
    RUBY_THIN_ARCHLIB,
#endif
    RUBY_ARCHLIB,
    ""
};

void
Init_version()
{
    VALUE v = MKSTR(version);
    VALUE d = MKSTR(release_date);
    VALUE p = MKSTR(platform);

#ifndef RUBY_DESCRIPTION
    snprintf(ruby_description, sizeof(ruby_description),
	     "ruby %s%s (%s%s) [%s]",
	     RUBY_VERSION, RUBY_PATCHLEVEL_STR,
	     RUBY_RELEASE_DATE, RUBY_REVISION_STR,
	     RUBY_PLATFORM);
#endif
#ifndef RUBY_COPYRIGHT
    snprintf(ruby_copyright, sizeof(ruby_copyright),
	     "ruby - Copyright (C) %d-%d %s",
	     RUBY_BIRTH_YEAR, RUBY_RELEASE_YEAR,
	     RUBY_AUTHOR);
#endif

    rb_define_global_const("RUBY_VERSION", v);
    rb_define_global_const("RUBY_RELEASE_DATE", d);
    rb_define_global_const("RUBY_PLATFORM", p);
    rb_define_global_const("RUBY_PATCHLEVEL", INT2FIX(RUBY_PATCHLEVEL));
    rb_define_global_const("RUBY_REVISION", INT2FIX(RUBY_REVISION));
    rb_define_global_const("RUBY_DESCRIPTION", MKSTR(description));
    rb_define_global_const("RUBY_COPYRIGHT", MKSTR(copyright));

    /* obsolete constants */
    rb_define_global_const("VERSION", v);
    rb_define_global_const("RELEASE_DATE", d);
    rb_define_global_const("PLATFORM", p);
}

void
ruby_show_version()
{
    PRINT(description);
    fflush(stdout);
}

void
ruby_show_copyright()
{
    PRINT(copyright);
    exit(0);
}
