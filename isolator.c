
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_isolator.h"

#include "sched.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/mount.h>

static void isolator_register_classes(void);
zend_class_entry *isolator_ce_isolator;

/* {{{ proto int isolator_unshare(int arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(isolator_unshare)
{
	long arg = 0;
	long unshare_flags = 0;
	int ret = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &arg) == FAILURE) {
		return;
	}

	unshare_flags = arg;
	ret = unshare(unshare_flags);

	RETURN_LONG(ret);

}
/* }}} */


/* {{{ proto bool isolator_chroot(string directory)
   Change root directory */
PHP_FUNCTION(isolator_chroot)
{
	char *str;
	int ret;
	size_t str_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "p", &str, &str_len) == FAILURE) {
		RETURN_FALSE;
	}

	ret = chroot(str);
	if (ret != 0) {
		php_error_docref(NULL, E_WARNING, "%s (errno %d)", strerror(errno), errno);
		RETURN_FALSE;
	}

	php_clear_stat_cache(1, NULL, 0);

	ret = chdir("/");

	if (ret != 0) {
		php_error_docref(NULL, E_WARNING, "%s (errno %d)", strerror(errno), errno);
		RETURN_FALSE;
	}

	RETURN_TRUE;
}
/* }}} */


/* {{{ proto bool isolator_mount(string source_path,string target_path,string filesystemtype,long mountflags)
   Mount */
PHP_FUNCTION(isolator_mount)
{

	char *source_path, *target_path, *filesystemtype;
	size_t source_path_len, target_path_len, filesystemtype_len;
    zend_long mountflags = 0;

	ZEND_PARSE_PARAMETERS_START(3, 4)
		Z_PARAM_PATH(source_path, source_path_len)
		Z_PARAM_PATH(target_path, target_path_len)
        Z_PARAM_STRING(filesystemtype, filesystemtype_len)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG(mountflags)
	ZEND_PARSE_PARAMETERS_END_EX(RETURN_FALSE);

    if (mount(source_path, target_path, filesystemtype, (unsigned long)mountflags ,NULL) ){
      RETURN_FALSE;
    }

	RETURN_TRUE;
}
/* }}} */

#define REGISTER_ISOLATOR_CONSTANT(__c) REGISTER_LONG_CONSTANT(#__c, __c, CONST_CS | CONST_PERSISTENT)

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(isolator)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/

	isolator_register_classes();

	REGISTER_ISOLATOR_CONSTANT(CLONE_VM);
	REGISTER_ISOLATOR_CONSTANT(CLONE_FS);
	REGISTER_ISOLATOR_CONSTANT(CLONE_FILES);
	REGISTER_ISOLATOR_CONSTANT(CLONE_SIGHAND);
	REGISTER_ISOLATOR_CONSTANT(CLONE_PTRACE);
	REGISTER_ISOLATOR_CONSTANT(CLONE_VFORK);
	REGISTER_ISOLATOR_CONSTANT(CLONE_PARENT);
	REGISTER_ISOLATOR_CONSTANT(CLONE_THREAD);
	REGISTER_ISOLATOR_CONSTANT(CLONE_NEWNS);
	REGISTER_ISOLATOR_CONSTANT(CLONE_SYSVSEM);
	REGISTER_ISOLATOR_CONSTANT(CLONE_SETTLS);
	REGISTER_ISOLATOR_CONSTANT(CLONE_PARENT_SETTID);
	REGISTER_ISOLATOR_CONSTANT(CLONE_CHILD_CLEARTID);
	REGISTER_ISOLATOR_CONSTANT(CLONE_DETACHED);
	REGISTER_ISOLATOR_CONSTANT(CLONE_UNTRACED);
	REGISTER_ISOLATOR_CONSTANT(CLONE_CHILD_SETTID);
	REGISTER_ISOLATOR_CONSTANT(CLONE_NEWUTS);
	REGISTER_ISOLATOR_CONSTANT(CLONE_NEWIPC);
	REGISTER_ISOLATOR_CONSTANT(CLONE_NEWUSER);
	REGISTER_ISOLATOR_CONSTANT(CLONE_NEWPID);
	REGISTER_ISOLATOR_CONSTANT(CLONE_NEWNET);
	REGISTER_ISOLATOR_CONSTANT(CLONE_IO);

	REGISTER_ISOLATOR_CONSTANT(MS_RDONLY);
	REGISTER_ISOLATOR_CONSTANT(MS_NOSUID);
	REGISTER_ISOLATOR_CONSTANT(MS_NODEV);
	REGISTER_ISOLATOR_CONSTANT(MS_NOEXEC);
	REGISTER_ISOLATOR_CONSTANT(MS_SYNCHRONOUS);
	REGISTER_ISOLATOR_CONSTANT(MS_REMOUNT);
	REGISTER_ISOLATOR_CONSTANT(MS_MANDLOCK);
	REGISTER_ISOLATOR_CONSTANT(MS_DIRSYNC);
	REGISTER_ISOLATOR_CONSTANT(MS_NOATIME);
	REGISTER_ISOLATOR_CONSTANT(MS_NODIRATIME);
	REGISTER_ISOLATOR_CONSTANT(MS_BIND);
	REGISTER_ISOLATOR_CONSTANT(MS_MOVE);
	REGISTER_ISOLATOR_CONSTANT(MS_REC);
	REGISTER_ISOLATOR_CONSTANT(MS_SILENT);
	REGISTER_ISOLATOR_CONSTANT(MS_POSIXACL);
	REGISTER_ISOLATOR_CONSTANT(MS_UNBINDABLE);
	REGISTER_ISOLATOR_CONSTANT(MS_PRIVATE);
	REGISTER_ISOLATOR_CONSTANT(MS_SLAVE);
	REGISTER_ISOLATOR_CONSTANT(MS_SHARED);
	REGISTER_ISOLATOR_CONSTANT(MS_RELATIME);
	REGISTER_ISOLATOR_CONSTANT(MS_KERNMOUNT);
	REGISTER_ISOLATOR_CONSTANT(MS_I_VERSION);
	REGISTER_ISOLATOR_CONSTANT(MS_STRICTATIME);
	REGISTER_ISOLATOR_CONSTANT(MS_ACTIVE);
	REGISTER_ISOLATOR_CONSTANT(MS_NOUSER);
	REGISTER_ISOLATOR_CONSTANT(MS_RMT_MASK);
	REGISTER_ISOLATOR_CONSTANT(MS_MGC_VAL);
	REGISTER_ISOLATOR_CONSTANT(MS_MGC_MSK);

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(isolator)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(isolator)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(isolator)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(isolator)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "isolator support", "enabled");
	php_info_print_table_end();

	DISPLAY_INI_ENTRIES();
}
/* }}} */

/* {{{ isolator_functions[]
 *
 * Every user visible function must have an entry in isolator_functions[].
 */
const zend_function_entry isolator_functions[] = {
	PHP_FE(isolator_unshare,	NULL)
	PHP_FE(isolator_chroot,	NULL)
	PHP_FE(isolator_mount,	NULL)
	/* __function_entries_here__ */
	PHP_FE_END	/* Must be the last line in isolator_functions[] */
};
/* }}} */

#define arginfo_isolator_unshare NULL
#define arginfo_isolator_chroot NULL
#define arginfo_isolator_mount NULL

const zend_function_entry isolator_funcs_isolator[] = {
	PHP_ME_MAPPING(unshare,	isolator_unshare,	arginfo_isolator_unshare,	ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME_MAPPING(chroot,	isolator_chroot,	arginfo_isolator_chroot,	ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME_MAPPING(mount,	isolator_mount,	arginfo_isolator_mount,	ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};

static void isolator_register_classes(void) /* {{{ */
{
	zend_class_entry ce_isolator;

	INIT_CLASS_ENTRY(ce_isolator, "Isolator", isolator_funcs_isolator);
	isolator_ce_isolator = zend_register_internal_class_ex(&ce_isolator, NULL);
} /* }}} */

/* {{{ isolator_module_entry
 */
zend_module_entry isolator_module_entry = {
	STANDARD_MODULE_HEADER,
	"isolator",
	isolator_functions,
	PHP_MINIT(isolator),
	PHP_MSHUTDOWN(isolator),
	PHP_RINIT(isolator),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(isolator),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(isolator),
	PHP_ISOLATOR_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_ISOLATOR
ZEND_GET_MODULE(isolator)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
