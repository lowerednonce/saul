/* on the note of debugging
* Debugging works the following way: there is a unsigned int, in which each bit is a boolean debug flag
* If you want to get the status of a given flag, you can use the predefined makros in here and do an AND operation to exclude to the single targeted bit, then shift it right
* This might seem like a too complicated solution, but it is not that hard to use
* However I might switch in the future to a model, where there is a bool array in the Window object to store the flags, and there would be macros for the indexes
*/

#ifndef __DEBUG_FLAGS__

#define SAUL_DEGUG_ALL                0
#define SAUL_DEBUG_RENDER_ERROR       1
#define SAUL_DEBUG_RENDER_WARNING     2
#define SAUL_DEBUG_RENDER_INFO        3
#define SAUL_DEBUG_LOAD_TEXTURE_ERROR 4
#define SAUL_DEBUG_LOAD_TEXTURE_INFO  5
#define SAUL_DEBUG_SCENE_ADD_ERROR   6
#define SAUL_DEBUG_SCENE_ADD_INFO    7
#define SAUL_DEBUG_SCENE_SET         8
#define SAUL_DEBUG_WINDOW_CREATE      9
#define SAUL_DEBUG_WINDOW_DESTROY    10
#define SAUL_DEBUG_DEBUG_SET         11
#define SAUL_DEBUG_EVENTS_WARNING    12
#define SAUL_DEBUG_EVENTS_INFO       13


#define SAUL_DEBUG_LEN 14

#define __DEBUG_FLAGS__
#endif
