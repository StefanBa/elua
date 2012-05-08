/*>> Begin of file*/

/* Not sure if all they are all necessary, but it works like this. */
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "common.h"
#include "auxmods.h"
#include "lrotable.h"
#include "platform_conf.h"

#include "devman.h"


/* Our example idiotic function */
static int file_dostuff( lua_State *L ){
        lua_pushinteger( L, 43 );
        return 1;
}

static int file_getsize( lua_State *L )
{
  const DM_DEVICE *pdev;
  unsigned dev, i;
  DM_DIR *d;
  struct dm_dirent *ent;
  u32 total;

  const char *buf;
  size_t len;

  luaL_checktype( L, 1, LUA_TSTRING );
  buf = lua_tolstring( L, 1, &len );

  // Iterate through all devices, looking for the ones that can do "opendir"
  for( dev = 0; dev < dm_get_num_devices(); dev ++ )
  {
    pdev = dm_get_device_at( dev );
    if( pdev->p_opendir_r == NULL || pdev->p_readdir_r == NULL || pdev->p_closedir_r == NULL )
      continue;
    d = dm_opendir( pdev->name );
    if( d )
    {
      while( ( ent = dm_readdir( d ) ) != NULL )
      {
    	  if ( strcmp( ent->fname , buf ) == 0 ){
    	        lua_pushinteger( L, ( unsigned )ent->fsize );
    	        return 1;
    	  }
      }
      dm_closedir( d );
    }
  }
  lua_pushnil( L );
  return 1;
}



/* Our magic constant */
#define IMAG_MAGICNUMBER 43

/* Here is the definition of the Romtable. Note the macros that we don't have
 * in normal Lua. They are features introduced in eLua to make possible the
 * registration not only of functions but constants as well. These macros are
 * important, remember that we're not using a luaL_Reg as in normal Lua.
 * So remember to use them.
 * For more about this go to 'src/lua/lrodefs.h' & 'src/lua/lrotable.h'.
 */
#define MIN_OPT_LEVEL 2
#include "lrodefs.h"
const LUA_REG_TYPE file_map[] =
{
  { LSTRKEY( "getsize" ), LFUNCVAL( file_getsize ) },
  { LSTRKEY( "dostuff" ), LFUNCVAL( file_dostuff ) },
  { LSTRKEY( "magic_number" ), LNUMVAL( IMAG_MAGICNUMBER ) },
  { LNILKEY, LNILVAL }
};

/* Now we have another tricky part, here you have 2 possible registrations of
 * your module. The first case, the '#if LUA_OPTIMIZE_MEMORY > 0', we get
 * all of this on the romtable, so... Nothing to register.
 * The '#else' part we register it like in Lua and the function and constant will live in RAM.
 */
LUALIB_API int luaopen_file( lua_State *L ){
#if LUA_OPTIMIZE_MEMORY > 0
        return 0;
#else
        LREGISTER( L, AUXLIB_IMAG, file_map );
        return 1;
#endif
}

/* >> End of file */
