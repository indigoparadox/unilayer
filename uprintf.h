
#ifndef UPRINTF_H
#define UPRINTF_H

#ifndef PLATFORM_NEWLINE
#define PLATFORM_NEWLINE "\n"
#endif /* !PLATFORM_NEWLINE */

#ifdef LOG_TO_FILE
#ifndef DEBUG_LOG
#define DEBUG_LOG
#endif /* !DEBUG_LOG */
#define LOG_ERR_TARGET g_log_file
#define LOG_STD_TARGET g_log_file
#else
#define LOG_ERR_TARGET stderr
#define LOG_STD_TARGET stdout
#endif /* LOG_TO_FILE */

/* ! */
#ifdef ANCIENT_C
/* ! */

#  define debug_printf
#  define error_printf

/* ! */
#elif defined( DEBUG_LOG )
/* ! */

#  include <stdio.h>

#  define internal_debug_printf( lvl, ... ) if( NULL != LOG_ERR_TARGET && lvl >= DEBUG_THRESHOLD ) { platform_fprintf( LOG_STD_TARGET, "(%d) " __FILE__ ": %d: ", lvl, __LINE__ ); platform_fprintf( LOG_STD_TARGET, __VA_ARGS__ ); platform_fprintf( LOG_STD_TARGET, PLATFORM_NEWLINE ); platform_fflush( LOG_STD_TARGET ); }

#  define internal_error_printf( ... ) if( NULL != LOG_ERR_TARGET ) { platform_fprintf( LOG_ERR_TARGET, "(E) " __FILE__ ": %d: ", __LINE__ ); platform_fprintf( LOG_ERR_TARGET, __VA_ARGS__ ); platform_fprintf( LOG_ERR_TARGET, PLATFORM_NEWLINE ); platform_fflush( LOG_ERR_TARGET ); }

#  define debug_printf( lvl, ... ) internal_debug_printf( lvl, __VA_ARGS__ )

#  define error_printf( ... ) internal_error_printf( __VA_ARGS__ )

/* ! */
#else /* !DEBUG_LOG, !ANCIENT_C */
/* ! */

#  define debug_printf( ... )
#  define error_printf( ... )

/* ! */
#endif /* DEBUG_LOG, ANCIENT_C */
/* ! */

#ifdef LOG_TO_FILE
#ifdef MAIN_C
platform_file g_log_file = NULL;
#else /* !MAIN_C */
extern platform_file g_log_file;
#endif /* MAIN_C */
#endif /* LOG_TO_FILE */

#endif /* !UPRINTF_H */

