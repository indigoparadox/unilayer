
#ifndef JSON_H
#define JSON_H

/**
 * \addtogroup unilayer_json JSON
 * \brief Simple lightweight general-purpose JSON parser based on jsmn.
 *
 * \{
 */

#ifndef JSON_C
#define JSMN_HEADER
#endif /* JSON_C */
#define JSMN_PARENT_LINKS
#include "jsmn.h"

int16_t json_load(
   char* json_buffer, uint16_t json_buffer_sz,
   struct jsmntok* tokens, uint16_t tokens_sz );
int16_t json_get_token_idx(
   const char*, uint16_t, jsmntok_t*, uint16_t, const char*, uint16_t );
int16_t json_token_id_from_path(
   const char*, uint16_t, jsmntok_t*, uint16_t, const char* );
int16_t json_int_from_path(
   const char*, uint16_t, jsmntok_t*, uint16_t, const char* );
int16_t json_bool_from_path(
   const char* path, uint16_t path_sz,
   struct jsmntok* tokens, uint16_t tokens_sz, const char* buf );
int16_t json_str_from_path(
   const char*, uint16_t, char*, uint16_t, jsmntok_t*, uint16_t, const char* );

/**
 * \page json_paths JSON Paths
 **/

/*! \} */

#endif /* JSON_H */

