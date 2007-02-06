/*
 * This file is included by eval.c
 */

/* safe-level:
   0 - strings from streams/environment/ARGV are tainted (default)
   1 - no dangerous operation by tainted value
   2 - process/file operations prohibited
   3 - all generated objects are tainted
   4 - no global (non-tainted) variable modification/no direct output
*/

int
rb_safe_level(void)
{
    return GET_THREAD()->safe_level;
}

void
rb_set_safe_level_force(int safe)
{
    GET_THREAD()->safe_level = safe;
}

static VALUE safe_getter _((void));
static void safe_setter _((VALUE val));

#define PROC_TSHIFT (FL_USHIFT+1)
#define PROC_TMASK  (FL_USER1|FL_USER2|FL_USER3)
#define PROC_TMAX   (PROC_TMASK >> PROC_TSHIFT)
#define PROC_NOSAFE FL_USER4

#define SAFE_LEVEL_MAX PROC_TMASK

/* $SAFE accessor */
void
rb_set_safe_level(int level)
{
    rb_thead_t *th = GET_THREAD();

    if (level > th->safe_level) {
	if (level > SAFE_LEVEL_MAX) {
	    level = SAFE_LEVEL_MAX;
	}
	th->safe_level = level;
    }
}

static VALUE
safe_getter(void)
{
    return INT2NUM(rb_safe_level());
}

static void
safe_setter(VALUE val)
{
    int level = NUM2INT(val);
    rb_thead_t *th = GET_THREAD();

    if (level < th->safe_level) {
	rb_raise(rb_eSecurityError,
		 "tried to downgrade safe level from %d to %d",
		 th->safe_level, level);
    }
    if (level > SAFE_LEVEL_MAX) {
	level = SAFE_LEVEL_MAX;
    }
    th->safe_level = level;
}

void
rb_secure(int level)
{
    if (level <= rb_safe_level()) {
	if (rb_frame_callee()) {
	    rb_raise(rb_eSecurityError, "Insecure operation `%s' at level %d",
		     rb_id2name(rb_frame_callee()), rb_safe_level());
	}
	else {
	    rb_raise(rb_eSecurityError, "Insecure operation at level %d",
		     rb_safe_level());
	}
    }
}

void
rb_secure_update(VALUE obj)
{
    if (!OBJ_TAINTED(obj))
	rb_secure(4);
}

void
rb_check_safe_obj(VALUE x)
{
    if (rb_safe_level() > 0 && OBJ_TAINTED(x)) {
	if (rb_frame_callee()) {
	    rb_raise(rb_eSecurityError, "Insecure operation - %s",
		     rb_id2name(rb_frame_callee()));
	}
	else {
	    rb_raise(rb_eSecurityError, "Insecure operation: -r");
	}
    }
    rb_secure(4);
}

void
rb_check_safe_str(VALUE x)
{
    rb_check_safe_obj(x);
    if (TYPE(x) != T_STRING) {
	rb_raise(rb_eTypeError, "wrong argument type %s (expected String)",
		 rb_obj_classname(x));
    }
}
