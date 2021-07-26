
#ifndef UASSERT_H
#define UASSERT_H

/* ! */
#ifdef DISABLE_ASSERT
/* ! */

#define assert( comp )

/* ! */
#elif defined( USE_SOFT_ASSERT )
/* ! */

#  define assert( comp ) if( !(comp) ) { g_assert_failed_len = dio_snprintf( g_assert_failed, 255, __FILE__ ": %d: ASSERT FAILED", __LINE__ ); }

#elif !defined( assert )

#include <assert.h>

/* ! */
#endif /* USE_SOFT_ASSERT */
/* ! */

#ifndef MAIN_C
extern char g_assert_failed[];
extern int g_assert_failed_len;
#endif /* !MAIN_C */

#endif /* UASSERT_H */

