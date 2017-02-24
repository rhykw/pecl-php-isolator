/* __header_here__ */

#ifndef PHP_ISOLATOR_H
#define PHP_ISOLATOR_H

extern zend_module_entry isolator_module_entry;
#define phpext_isolator_ptr &isolator_module_entry

#define PHP_ISOLATOR_VERSION "0.1.0"

#ifdef PHP_WIN32
#	define PHP_ISOLATOR_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_ISOLATOR_API __attribute__ ((visibility("default")))
#else
#	define PHP_ISOLATOR_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

/* 
  	Declare any global variables you may need between the BEGIN
	and END macros here:     

ZEND_BEGIN_MODULE_GLOBALS(isolator)
	long  global_value;
	char *global_string;
ZEND_END_MODULE_GLOBALS(isolator)
*/

/* In every utility function you add that needs to use variables 
   in php_isolator_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as ISOLATOR_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define ISOLATOR_G(v) TSRMG(isolator_globals_id, zend_isolator_globals *, v)
#else
#define ISOLATOR_G(v) (isolator_globals.v)
#endif

#endif	/* PHP_ISOLATOR_H */

/* __footer_here__ */
