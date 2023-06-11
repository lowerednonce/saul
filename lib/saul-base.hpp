#pragma once
#include <list>
#include <string>
#include <vector>
#ifndef linux
        #include <SDL.h>
        #include <SDL_ttf.h>
        #include <SDL_image.h>
#endif
#ifdef linux
        #include <SDL2/SDL.h>
        #include <SDL2/SDL_ttf.h>
        #include <SDL2/SDL_image.h>
#endif

#include "debug.hpp"

#ifndef IMGFLAGS
        #define IMGFLAGS IMG_INIT_JPG|IMG_INIT_PNG
#endif

#ifndef __SAUL_BASE__

#define __SAUL_BASE__

namespace saul{
        enum TYPE {TEXTU, TEXTURE, TEXTUREPRIMITIVE, BUTTON, CHECKBOX};

        struct Button{
              SDL_Color bgcolor;
              SDL_Texture* bgtexture;
              SDL_Texture* txtexture;
              SDL_Rect bgboundaries;
              SDL_Rect txboundaries;
              unsigned int upclicks;
              bool clickable;
        };
        struct TextS{
                SDL_Surface* t_surface;
                SDL_Rect boundaries;
        };
        struct TextU{
                SDL_Texture* t_texture;
                SDL_Surface* t_surface;
                SDL_Rect boundaries;
        };
        struct Texture{
                SDL_Texture* texture;
                SDL_Rect boundaries;
        };
        struct TexturePrimitive{
                //Like textures, but just a simple monotone color
                SDL_Color fill_color;
                SDL_Rect boundaries;
        };
        struct CheckBox{
                SDL_Texture* toggledtexture;
                SDL_Texture* offtexture;
                SDL_Rect boundaries;
                bool toggled;
        };
        struct UIObj{
                const char* name;
                saul::TYPE type;
                void* objectp;
                bool enabled;
        };

        struct Scene{
                std::list<saul::UIObj> objects;
                const char* name;
        };

        // function here, so they can use saul's types
        void deleteObject(struct saul::UIObj obj);
        bool Log(const char* logf, const char* level, const char* caller, const char* text);
        SDL_Surface* textToSurface(TTF_Font * font, const char* text, SDL_Color color);

	class Window{
                private:
                        const char* wname;
                        SDL_Window* window;
                        SDL_Renderer* renderer;
                        int x;int y; int w; int h;
                        unsigned int activeScene;
                        std::list<saul::Scene> scenes;
                        bool debugflags[SAUL_DEBUG_LEN] = {false};

                        void renderObject(saul::UIObj obj);

                        bool CheckMouseOverlap(float MouseX, float MouseY, float OtherX, float OtherY, float OtherWidth, float OtherHeight);
                        bool handleEvents();

                public:
                        const char * logfname = NULL; // default no logfile
                        bool quitOnSignal;
                        int quitKeysLen;
                        SDL_Keycode quitKeys[];

                        Window(int& success, const char* name, Uint32 flags, int w, int h, int x=SDL_WINDOWPOS_UNDEFINED, int y=SDL_WINDOWPOS_UNDEFINED);
                        ~Window();

                        void setDflags(bool value[SAUL_DEBUG_LEN]);
                        void setDflagE(int element, bool value);
                        bool* getDflags();

                        void setActiveScene(int value);
                        unsigned int getActiveScene();

                        int addScene(struct saul::Scene scene);
                        struct saul::Scene getScene(int position);
                        int get_scene_size();

                        void setQuitOnSignal(bool s);
                        bool getQuitOnSignal();

                        int render();

                        void updateTextU(saul::TextU& textu);
                        SDL_Texture* LoadTexture(char* str);
                        SDL_Texture* surfaceToTexture(SDL_Surface* surface);

                        void updateStruct(unsigned int scene, unsigned int element_index, void* new_element);
        };
}

#endif
