
#ifndef WINDOW_H
#define WINDOW_H

/**
 * \addtogroup unilayer_gui Unilayer Graphical User Interface
 * \brief In-engine interface for windows and controls.
 *
 * \{
 */

/*! \brief A piece of data attached to CONTROL::data. */
union CONTROL_DATA {
   int32_t scalar;
   MEMORY_HANDLE string;
   int16_t grid[4];
};

/**
 * \brief Defines a graphical window frame/background with a set of image
 *        assets.
 */
struct WINDOW_FRAME {
   /*! \brief Top-right corner. */
   uint16_t tr;
   /*! \brief Top-left corner. */
   uint16_t tl;
   /*! \brief Bottom-right corner. */
   uint16_t br;
   /*! \brief Bottom-left corner. */
   uint16_t bl;
   /*! \brief Top edge. */
   uint16_t t;
   /*! \brief Bottom edge. */
   uint16_t b;
   /*! \brief Right edge. */
   uint16_t r;
   /*! \brief Left edge. */
   uint16_t l;
   /*! \brief Center fill. */
   uint16_t c;
};

/*! \brief Struct representing an on-screen graphical window. */
struct WINDOW {
   /*! \brief Unique identifier used to find this window in the global stack. */
   uint16_t id;
   uint16_t parent_id;
   /*! \brief Type of control defined by a constant. */
   uint8_t type;
   /*! \brief Current drawing status of this window. */
   uint8_t flags;
   /* TODO: Work into flags. */
   uint8_t render_flags;
   /**
    * \brief Window coordinates and dimensions indexed as ::GUI_X, ::GUI_Y,
    *        ::GUI_W, ::GUI_H.
    */
   GRAPHICS_COLOR fg;
   GRAPHICS_COLOR bg;
   int16_t coords[4];
   union CONTROL_DATA data;
};

/*! \} */

/*! \file window.h
 *  \brief Tools for drawing and interacting with graphical windows on-screen.
 */

/**
 * \brief Specifies X coordinate in WINDOW::coords.
 */
#define GUI_X 0
/**
 * \brief Specifies X coordinate in WINDOW::coords.
 */
#define GUI_Y 1
/**
 * \brief Specifies width coordinate in WINDOW::coords.
 */
#define GUI_W 2
/**
 * \brief Specifies height in WINDOW::coords.
 */
#define GUI_H 3

/*! \brief WINDOW::x or WINDOW::y value indicating the system should do its
 *         best to center the WINDOW onscreen.
 */
#define WINDOW_CENTERED -1

/*! \brief Maximum number of windows that can be onscreen at one time. */
#define WINDOW_COUNT_MAX 10

/**
 * \brief Size of internal text buffer used to represent
 *        ::WINDOW_FLAG_TEXT_NUM.
 */
#define WINDOW_NUM_BUFFER_SZ       10

/**
 * \addtogroup unilayer_gui_controls_flags GUI Control Flags
 * \brief Options specifying how a ::WINDOW behaves.
 *
 * The lower 4 bits are general to all controls while the upper 4 bits are
 * specific to each ::WINDOW:type.
 *
 * \{
 */

/**
 * \brief WINDOW::flags indicating control is visible and interactive.
 */
#define WINDOW_FLAG_ACTIVE       0x01
#define WINDOW_FLAG_DIRTY  0x04
/**
 * \brief WINDOW::status indicating window is blocking all input.
 */
#define WINDOW_FLAG_MODAL  0x08
/**
 * \brief WINDOW::flags indicating control option has been selected.
 */
#define WINDOW_FLAG_CHECKED       0x10

/**
 * \brief WINDOW::flags for WINDOW_TYPE_LABEL indicating text is a number.
 */
#define WINDOW_FLAG_TEXT_NUM        0x10

#define WINDOW_FLAG_TEXT_PTR        0x20

#define WINDOW_FLAG_TEXT_MASK       0x30

#define WINDOW_FLAG_SPRITE_DIR_MASK 0x30

#define WINDOW_FLAG_SPRITE_SOUTH    0x00
#define WINDOW_FLAG_SPRITE_NORTH    0x10
#define WINDOW_FLAG_SPRITE_EAST     0x20
#define WINDOW_FLAG_SPRITE_WEST     0x30

#define WINDOW_FLAG_SPRITE_BORDER_SINGLE  0x40

/*! \} */

#define WINDOW_STRING_SZ_MAX 100

/**
 * \addtogroup unilayer_gui_controls_placement GUI Control Placement
 * \brief Options for specifying ::WINDOW placement in a ::WINDOW.
 *
 * \{
 */

/*! \brief Place the control in the center of the window. */
#define WINDOW_PLACEMENT_CENTER       (-1)
/**
 * \brief Alight the control's right side (if specified as X) or bottom side
 *        (if specified as Y) to the window's respective side. */
#define WINDOW_PLACEMENT_RIGHT_BOTTOM (-2)
/**
 * \brief Place the control at the grid X or Y as relevant, and set the grid
 *        width or height respectively at the control's width or height.
 */
#define WINDOW_PLACEMENT_GRID_RIGHT_DOWN   (-3)
/**
 * \brief Place the control at the grid X or Y without modifying the grid.
 */
#define WINDOW_PLACEMENT_GRID         (-4)

#define WINDOW_SIZE_AUTO   (-5)

/*! \} */

#define window_screen_reset_grid() memory_zero_ptr( g_window_screen_grid, 4 * sizeof( int16_t ) );

#ifdef DEPTH_VGA
#  define WINDOW_PREFAB_DEFAULT_FG() GRAPHICS_COLOR_WHITE
#  define WINDOW_PREFAB_DEFAULT_BG() GRAPHICS_COLOR_MAGENTA
#  define WINDOW_PREFAB_DEFAULT_HL() GRAPHICS_COLOR_CYAN
#elif defined( DEPTH_CGA )
#  define WINDOW_PREFAB_DEFAULT_FG() GRAPHICS_COLOR_BLACK
#  define WINDOW_PREFAB_DEFAULT_BG() GRAPHICS_COLOR_MAGENTA
#  define WINDOW_PREFAB_DEFAULT_HL() GRAPHICS_COLOR_CYAN
#elif defined( DEPTH_MONO )
   /* TODO: Verify this looks OK. */
#  define WINDOW_PREFAB_DEFAULT_FG() GRAPHICS_COLOR_WHITE
#  define WINDOW_PREFAB_DEFAULT_BG() GRAPHICS_COLOR_BLACK
#  define WINDOW_PREFAB_DEFAULT_HL() GRAPHICS_COLOR_WHITE
#endif /* DEPTH */

/**
 * \brief Global initialization for the window subsystem. Runs at startup.
 * \param auto_w Limit width of screen area for auto-centering.
 * \param auto_h Limit height of screen area for auto-centering.
 * \return 1 if successful, or 0 otherwise.
 */
int16_t window_init( uint16_t auto_w, uint16_t auto_h );

int16_t window_reload_frames();

/**
 * \brief Global shutdown for the window subsystem. Runs at shutdown.
 */
void window_shutdown();

int16_t window_draw_all();

/**
 * \brief Push a new window onto the global window stack.
 * \param id WINDOW::id for the window being pushed.
 * \param status Initial WINDOW::status for the window being pushed.
 * \param x The left horizontal offset of the window in pixels.
 * \param y The top vertical offset of the window in pixels.
 * \param w The width of the window in pixels.
 * \param h The height of the window in pixels.
 * \param frame_idx The index of the window frame to use.
 * \param windows Locked ::MEMORY_PTR to the global window list.
 */
int16_t window_push(
   uint16_t id, uint16_t parent_id, uint8_t type, uint8_t flags,
   int16_t x, int16_t y, int16_t w, int16_t h,
   GRAPHICS_COLOR fg, GRAPHICS_COLOR bg, uint8_t render_flags,
   int32_t data_scalar, const char* data_string );

/**
 * \brief Destroy the top-most onscreen WINDOW with the given WINDOW::id.
 * \param id WINDOW::id to search for and eliminate.
 * \param windows Locked ::MEMORY_PTR to the global window list.
 */
void window_pop( uint16_t id );

void window_refresh( uint16_t w_id );

#if 0
/**
 * \brief Determine if there is a WINDOW_FLAG_MODAL WINDOW on-screen.
 * \param windows Locked ::MEMORY_PTR to the global window list.
 * \return 0 if no modal windows showing, 1+ otherwise.
 */
int16_t window_modal();
#endif

#define window_modal() (g_window_modals)

#define WINDOW_CB_TABLE( f ) f( 0, LABEL ) f( 1, BUTTON ) f( 2, CHECK ) f( 3, SPRITE ) f( 4, WINDOW )

typedef int16_t (*WINDOW_CB_DRAW)( uint16_t w_id, struct WINDOW* windows );

/**
 * \return 1 if successful and 0 otherwise.
 */
typedef uint8_t (*WINDOW_CB_SZ)(
   uint16_t w_id, struct WINDOW* windows, int16_t r[2] );



#ifdef WINDOW_C

static MEMORY_HANDLE g_frames_handle = (MEMORY_HANDLE)NULL;
static MEMORY_HANDLE g_windows_handle = (MEMORY_HANDLE)NULL;

uint8_t g_window_modals = 0;

static int16_t g_window_screen_coords[4] = {
   0, 0, SCREEN_W, SCREEN_H
};

static int16_t g_window_screen_grid[4] = {
   0, 0, 0, 0
};

#  define WINDOW_CB_DRAW_TABLE_PROTOTYPES( idx, name ) int16_t window_draw_ ## name( uint16_t w_id, struct WINDOW* windows );

WINDOW_CB_TABLE( WINDOW_CB_DRAW_TABLE_PROTOTYPES );

#  define WINDOW_CB_SZ_TABLE_PROTOTYPES( idx, name ) uint8_t window_sz_ ## name( uint16_t w_id, struct WINDOW* windows, int16_t r[2] );

WINDOW_CB_TABLE( WINDOW_CB_SZ_TABLE_PROTOTYPES );

#  define WINDOW_CB_DRAW_TABLE_LIST( idx, name ) window_draw_ ## name,

static const WINDOW_CB_DRAW gc_window_draw_callbacks[] = {
   WINDOW_CB_TABLE( WINDOW_CB_DRAW_TABLE_LIST )
};

#  define WINDOW_CB_SZ_TABLE_LIST( idx, name ) window_sz_ ## name,

static const WINDOW_CB_SZ gc_window_sz_callbacks[] = {
   WINDOW_CB_TABLE( WINDOW_CB_SZ_TABLE_LIST )
};

#  define WINDOW_CB_TABLE_CONSTS( idx, name ) RES_CONST uint16_t WINDOW_TYPE_ ## name = idx;

WINDOW_CB_TABLE( WINDOW_CB_TABLE_CONSTS );

#else

extern uint8_t g_window_modals;

/**
 * \brief Define extern constants that can be used e.g. in spawners.
 */
#  define WINDOW_CB_TABLE_CONSTS( idx, name ) extern RES_CONST uint16_t WINDOW_TYPE_ ## name;

WINDOW_CB_TABLE( WINDOW_CB_TABLE_CONSTS )

#endif /* WINDOW_C */

/*! \} */

#endif /* WINDOW_H */

