
#ifndef INPUT_H
#define INPUT_H

/**
 * \addtogroup unilayer_input Unilayer Input Layer
 * \brief Simple abstract unified remappable user input layer designed
 *        for resource-constrained systems.
 * \{
 *
 * \file input.h
 * \brief Platform-general abstractions for user input.
 */

#ifdef INPUT_C

#define INPUT_KEYS_VALS( name, val ) val,
RES_CONST INPUT_VAL gc_input_keys_vals[] = {
   INPUT_KEYS( INPUT_KEYS_VALS )
};

#define INPUT_KEYS_NAMES( name, val ) #name,
RES_CONST char* gc_input_keys_names[] = {
   INPUT_KEYS( INPUT_KEYS_NAMES )
   NULL
};

INPUT_VAL g_input_key_left;
INPUT_VAL g_input_key_right;
INPUT_VAL g_input_key_up;
INPUT_VAL g_input_key_down;
INPUT_VAL g_input_key_quit;
INPUT_VAL g_input_key_ok;
INPUT_VAL g_input_key_menu;

uint8_t input_platform_init();

#else

/*! \brief All valid key values for the compiled platform. */
extern RES_CONST INPUT_VAL g_keys_vals[];

/*! \brief Mapping names for key values for the compiled platform. */
extern RES_CONST char* g_keys_names[];
 
extern INPUT_VAL g_input_key_left;
extern INPUT_VAL g_input_key_right;
extern INPUT_VAL g_input_key_up;
extern INPUT_VAL g_input_key_down;
extern INPUT_VAL g_input_key_quit;
extern INPUT_VAL g_input_key_ok;
extern INPUT_VAL g_input_key_menu;

#endif /* INPUT_C */

uint8_t input_init();

/**
 * \brief Assign the given key to the given input function.
 * \param key Pointer to the input function key slot to assign to.
 * \param name String indicating the name of the key to assign.
 * \return 1 if successful or 0 if key not found.
 */
uint8_t input_assign_key( INPUT_VAL* key, const char* name, uint8_t name_sz )
   SECTION_SETUP;

INPUT_VAL input_poll( int16_t* x, int16_t* y );

void input_shutdown();

/*! \} */ /* unilayer_input */

#endif /* INPUT_H */

