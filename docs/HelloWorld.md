# A basic SAUL program

This will not cover compilation and building, please refer to the README inside the main folder for instructions on those.

For starters we need to import the nessesary libraries: SDL2 and SAUL.
```c++
// SDL2, might vary by platfrom
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

// SAUL 
#include "debug.hpp"
#include "saul-base.hpp"
#include "memory-track.hpp"
```

Then we create the window straight ahead, as SAUL already takes care of the initialization of SDL2. If it wasn't successful, we return.

```c++
const char* logfile = "out.log" // name of the file SAUL will log to
int fail; // this will be storing whether the creating the window was successful
saul::Window* main_w = new saul::Window(fail,"Testing", SDL_WINDOW_SHOWN, 500, 500);
if(fail){
        saul::Log(logfile, "INFO", "main", "Window creation failed");
}
```

There we used saul::Log(), SAUL's built in logging function. The function signature for saul::Log is `bool Log(const char* logf, const char* level, const char* caller, const char* text)` the logging file can be different for different parts of the code, but for the sake of being simple we'll keep it the same for the Window's logging and our logging.

Now we can go ahead and set up a simple square. In SAUL, simple color - no texture objects are stored inside a struct called saul::TexturePrimitive.

```c++
// sample saul::TexturePrimitive to render
const struct SDL_Color ptex_col = {0,255,0};  // the color blue
const struct SDL_Rect ptex_rec = {245,245,10,10}; // coordinates 255, 255 and 10x10 pixels

struct saul::TexturePrimitive* T_ptex = new saul::TexturePrimitive {
        ptex_col,
        ptex_rec,
};

```

In the above code, we set up `T_ptex` which will be of a color blue and will be placed at 245, 245 (x,y) and will be 10x10 pixels (width, height). Now this is a specific structure, in order to fit into a nice list of UI objects it is put into a saul::UIObj.
```c++
saul::UIObj primtex_object = {"blue, 10x10 box",  saul::TYPE::TEXTUREPRIMITIVE, (void*)T_ptex,    true};
```

The first element of the struct tells the name, the second the type of the object we are storing, the third a type-less void* pointer to the object, and the fourth one is whether it is enabled or not (will not render if set to false). Now the saul::UIObj can be put into a list.

```c++
std::list<saul::UIObj> objlist; // create std::list
objlist.push_back(primtex_object); // push our object, primtex_object into that std::list
```
Now the objectlist is put into a scene and set the active scene.
```c++
struct saul::Scene sc1 = {
		objlist,
                "our first SAUL Scene" // name for the Scene
};
main_w->setActiveScene(0); // set active scene to render.
```

Now the main render loop. Exit by closing the window.
```c++
while(True){
	if(main_w->render()==-1){
	    break;
	};
}
```
