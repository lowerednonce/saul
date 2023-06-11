#include <ctime>
#include <string>
#include <fstream>
#include <iostream>
#ifndef linux
        #include <SDL_ttf.h>
        #include <SDL_render.h>
#endif
#ifdef linux
        #include <SDL2/SDL_ttf.h>
        #include <SDL2/SDL_render.h>
#endif

#include "debug.hpp"
#include "saul-base.hpp"

//for details about debugging, see lib/debug.hpp

bool saul::Log(const char* logf, const char* level, const char* caller, const char* text){
	std::time_t t = std::time(nullptr);
	char mbstr[100];
	if (std::strftime(mbstr, sizeof(mbstr), "%d/%m/%Y %T ", std::localtime(&t))) {
		std::cout << mbstr << " - " << level << "(in "<< caller << "): "<< text <<'\n';
		if(logf!=NULL){
			// only do file operations when the filename is not NULL
			std::ofstream logfile;
			logfile.open(logf, std::ios_base::ate | std::ios_base::app);
			logfile << mbstr << " - " << level << "(in "<< caller << "): "<< text <<'\n';
			logfile.close();
		}
		return true;
	}
	return false;
        // the return is mostly useless, but can be used to check for errors getting the time
}

void saul::deleteObject(struct saul::UIObj obj){
        {
                std::string logmsg = "deleting object with name: \"";
                logmsg += obj.name;
                logmsg += "\"";
                saul::Log("out.log", "DEBUG", "saul::deleteObject", logmsg.c_str());
        }

        switch (obj.type) {
                case saul::TYPE::TEXTU :{
                        saul::TextU text = *(saul::TextU*)obj.objectp;
                        // So we create a duplicate saul::TextU on the stack, which
                        // will have the same pointers, that we can then free.
                        // the struct itself that objectp is pointing to is freed after the switch
                        SDL_DestroyTexture(text.t_texture);
                        SDL_FreeSurface(text.t_surface);
                        break;
                }
                case saul::TYPE::TEXTURE :{
                        saul::Texture tex = *(saul::Texture*)obj.objectp;
                        SDL_DestroyTexture(tex.texture);
                        break;
                }
                case saul::TYPE::TEXTUREPRIMITIVE :
                        // there is no surface of texture or stuff like that
                        break;
                case saul::TYPE::BUTTON :{
                        saul::Button btn = *(saul::Button*)obj.objectp;

                        SDL_DestroyTexture(btn.txtexture);
                        SDL_DestroyTexture(btn.bgtexture); // TODO: test whether we should put and if here
                        break;
                }
                case saul::TYPE::CHECKBOX :{
                        saul::CheckBox cbox = *(saul::CheckBox*)obj.objectp;

                        SDL_DestroyTexture(cbox.toggledtexture);
                        SDL_DestroyTexture(cbox.offtexture);
                        break;
                }
                default:
                        break;

                // finally free what the object is linking to
                free(obj.objectp);
        }
}

SDL_Surface* saul::textToSurface(TTF_Font * font, const char* text, SDL_Color color){
	return TTF_RenderText_Solid(font, text, color);
}

saul::Window::Window(int& success, const char* name, Uint32 flags, int w, int h, int x, int y){
        // setting simple variables
        wname = name;
        // debugflags initialized in class
        activeScene=0;
        quitOnSignal=true;
        // initing SDL
        SDL_Init(SDL_INIT_VIDEO);
        IMG_Init(IMGFLAGS);
        TTF_Init();
        window = SDL_CreateWindow(name, x, y, w, h, flags);
        if(window==NULL){
                success = 1; //unsuccessful creation of a window
        }else{
                renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
                if(renderer == NULL){
                        success = 2; //unsuccessful creation of a renderer
                }else{
                        success = 0; //no error
                }
        }
        // debugging done through the "success" variable
}

saul::Window::~Window(){
        saul::Log(logfname, "DEBUG", "Window::~Window", "destructor called");
        //TODO SDL_DestroyTexture() and SDL_FreeSurface()
        auto s_current = scenes.begin();
        for(unsigned int j = 0; j<scenes.size(); j++){
                std::list<saul::UIObj>::iterator o_current = s_current->objects.begin();
                for(unsigned int i = 0; i<s_current->objects.size(); i++){
                        saul::deleteObject(*o_current);
                        std::advance(o_current, 1);
                }
                std::advance(s_current, 1);
        }
        //library related stuff
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        TTF_Quit();
        //self explanitory, quit SDL, might cause more issues if you work with multiple windows at once
        if(debugflags[SAUL_DEGUG_ALL] || debugflags[SAUL_DEBUG_WINDOW_DESTROY]){
                saul::Log(logfname, "INFO", "saul::Window::~Window", "Window destructor finished");
        }
}

void saul::Window::setDflags(bool value[SAUL_DEBUG_LEN]){
        // value is actually a bool* pointing to the beggining of an array
        for(int i=0;i<SAUL_DEBUG_LEN;++i){
                debugflags[i]=*(value+i);
        }
        if(debugflags[SAUL_DEGUG_ALL] || debugflags[SAUL_DEBUG_DEBUG_SET]){
                saul::Log(logfname, "INFO", "saul::Window::setDflags", "debug flags set");
        }
}

bool* saul::Window::getDflags(){
        return debugflags;
}

void saul::Window::setDflagE(int element, bool value){
        debugflags[element] = value;
}

void saul::Window::setActiveScene(int value){
    activeScene = value;
    if(debugflags[SAUL_DEGUG_ALL] || debugflags[SAUL_DEBUG_SCENE_SET]){
        std::string fullText  = "Active scene set to ";
        fullText+=std::to_string(activeScene);
        saul::Log(logfname, "INFO", "saul::Window::setActiveScene", fullText.c_str());
    }
}

int saul::Window::addScene(struct saul::Scene scene){
        scenes.push_back(scene);
        return scenes.size();
}

struct saul::Scene saul::Window::getScene(int position){
        auto s_current = scenes.begin();
        std::advance(s_current, position);
        return *s_current;
}

unsigned int saul::Window::getActiveScene(){
    return activeScene;
}

int saul::Window::get_scene_size(){
        return scenes.size();
}

void saul::Window::setQuitOnSignal(bool s){
        quitOnSignal=s;
}

bool saul::Window::getQuitOnSignal(){
        return quitOnSignal;
}

int saul::Window::render(){

        if(saul::Window::handleEvents()){
                return -1;
        }
        // handleEvents() might quit and call the destructor, so to not waste resources, it's called here
        if(scenes.size()<activeScene){
                if(debugflags[SAUL_DEGUG_ALL] || debugflags[SAUL_DEBUG_RENDER_ERROR]){
                        std::string fullText = "the selected scene to render by activeScene is out of bounds, activeScreem currently is ";
                        fullText += std::to_string(activeScene);
                        fullText += ", and the current size of the scene stucture in this window class is ";
                        fullText += std::to_string(scenes.size());
                        saul::Log(logfname, "ERROR", "saul::Window::render", fullText.c_str());
                        // We make a string object where we add the number in a string form then we convert the string object to a char*, so saul::Log() accepts it
                }
                return 1; //Error no1: the selected scene to render is out of bounds
        }
        // saul::Window::renderTexturePrimitive sets renderer draw color, so set it back, then clear the scene
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        std::list<saul::Scene>::iterator s_current = scenes.begin();
        std::advance(s_current, activeScene);

        std::list<saul::UIObj>::iterator o_current = s_current->objects.begin();
        for(unsigned int i = 0; i<s_current->objects.size(); i++){
                renderObject(*o_current);
                std::advance(o_current, 1);
        }

        SDL_RenderPresent(renderer);
        return 0;
}

void saul::Window::updateTextU(saul::TextU& textu) {
    textu.t_texture = SDL_CreateTextureFromSurface(renderer, textu.t_surface);
}

SDL_Texture* saul::Window::LoadTexture(char* str){
        SDL_Surface* surface = IMG_Load(str);
        if(surface == NULL){
                if(debugflags[SAUL_DEGUG_ALL] || debugflags[SAUL_DEBUG_LOAD_TEXTURE_ERROR]){
                        std::string logmsg = "could not load image ";
                        logmsg += str;
                        logmsg += ", IMG_GetError(): ";
                        logmsg += IMG_GetError();
                        saul::Log(logfname, "ERROR", "saul::Window::LoadTexture", logmsg.c_str());
                }
                return NULL;
        }
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        if(debugflags[SAUL_DEGUG_ALL] || debugflags[SAUL_DEBUG_LOAD_TEXTURE_INFO]){
                std::string logmsg = "loaded texture with the path ";
                logmsg+=str;
                saul::Log(logfname, "INFO", "saul::Window::LoadTexture", logmsg.c_str());
        }
        return texture;
}

SDL_Texture* saul::Window::surfaceToTexture(SDL_Surface* surface){
        return SDL_CreateTextureFromSurface(renderer, surface);
}

void saul::Window::updateStruct(unsigned int scene, unsigned int element_index, void* new_element){
        // advance to the specified scene
        std::list<saul::Scene>::iterator s_current = scenes.begin();
        std::advance(s_current, scene);
        // advance to the specified object
        std::list<saul::UIObj>::iterator o_current = s_current->objects.begin();
        std::advance(o_current, element_index);

        // actually set the property
        free(o_current->objectp); // removing this will cause memory leaks
        o_current->objectp = new_element;

}

// Private functions

void saul::Window::renderObject(saul::UIObj obj){
        if(!obj.enabled){return;}; // if the object is not enabled, then do nothing with it

        if(obj.enabled){
                switch (obj.type) {
                        case saul::TYPE::TEXTU: {
                                saul::TextU textu = *(saul::TextU*)obj.objectp;

                                SDL_RenderCopy(renderer, textu.t_texture, NULL, &textu.boundaries);
                                break;
                        }
                        case saul::TYPE::TEXTURE:{
                                saul::Texture texture = *(saul::Texture*)obj.objectp;

                                SDL_RenderCopy(renderer, texture.texture, NULL, &texture.boundaries);
                                break;
                        }
                        case saul::TYPE::TEXTUREPRIMITIVE:{
                                saul::TexturePrimitive primtexture = *(saul::TexturePrimitive*)obj.objectp;

                                int retu  = SDL_SetRenderDrawColor(renderer, primtexture.fill_color.r, primtexture.fill_color.g, primtexture.fill_color.b, primtexture.fill_color.a);
                                int retu2 = SDL_RenderFillRect(renderer, &primtexture.boundaries);
                                break;
                        }
                        case saul::TYPE::BUTTON:{
                                saul::Button button = *(saul::Button*)obj.objectp;

                                // rendering the background
                                if(button.bgtexture==NULL || button.bgtexture==nullptr){
                                        // the background is a color
                                        int retu  = SDL_SetRenderDrawColor(renderer, button.bgcolor.r, button.bgcolor.g, button.bgcolor.b, button.bgcolor.a);
                                        int retu2 = SDL_RenderFillRect(renderer, &button.bgboundaries);
                                }else{
                                        SDL_RenderCopy(renderer, button.bgtexture, NULL, &button.bgboundaries);
                                }
                                SDL_RenderCopy(renderer, button.txtexture, NULL, &button.txboundaries);
                                break;
                        }
                        case saul::TYPE::CHECKBOX: {
                                saul::CheckBox chkbox = *(saul::CheckBox*)obj.objectp;

                                if(chkbox.toggled){
                                        SDL_RenderCopy(renderer, chkbox.toggledtexture, NULL, &chkbox.boundaries);
                                }else{ // off
                                        SDL_RenderCopy(renderer, chkbox.offtexture, NULL, &chkbox.boundaries);
                                }
                                break;
                        }
                        default:
                                // Unkown type
                                // TODO: error handling here
                                break;
                }
        }
}

bool saul::Window::CheckMouseOverlap(float MouseX, float MouseY, float OtherX, float OtherY, float OtherWidth, float OtherHeight){
    return (MouseX > OtherX) && (MouseX < OtherX + OtherWidth) && (MouseY > OtherY) && (MouseY < OtherY + OtherHeight);
}

bool saul::Window::handleEvents(){
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        switch(e.type){
            case SDL_QUIT:
                if(quitOnSignal){
                    // delete this;
                    return 1;
                }
                break;

            case SDL_KEYDOWN :{ // enclose so i doesn't escape
                for (int i = 0; i < quitKeysLen; i++) {
                    if (quitKeys[i] == e.key.keysym.sym) {
                        // delete this;
                        return 1;
                    }
                }
            }

            case SDL_MOUSEBUTTONDOWN :{
                std::list<saul::Scene>::iterator s_current = scenes.begin();
                std::advance(s_current, activeScene);
                // go to active scene

                // loop throught the objects
                std::list<saul::UIObj>::iterator o_current = s_current->objects.begin();
                for(unsigned int i = 0; i<s_current->objects.size(); i++){
                        // create iterator for the current object's properties
                        if(o_current->type==saul::TYPE::BUTTON){
                                saul::Button* btn = (saul::Button*)o_current->objectp;
                                if(btn->clickable && CheckMouseOverlap(e.button.x, e.button.y, btn->bgboundaries.x, btn->bgboundaries.y, btn->bgboundaries.w, btn->bgboundaries.h)){
                                        btn->upclicks++;
                                }
                        }

                        std::advance(o_current, 1);
                }
            }
            default:
                break;
        }
    }
    return 0;
}
