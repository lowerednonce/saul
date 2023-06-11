#include <ctime>
#include <string>
#include <iostream>
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

#include "../lib/debug.hpp"
#include "../lib/saul-base.hpp"
#include "../lib/memory-track.hpp"
#ifndef linux
  #undef main // Windows stuff, sloppy solution
#endif

AllocationMetrics s_AllocationMetrics;

void printMetrics(){
    std::cout << s_AllocationMetrics.current_usage() << " bytes \n";
   	std::cout << s_AllocationMetrics.totalAllocated << " allocated  bytes\n";
    std::cout << s_AllocationMetrics.totalFreed << " freed bytes\n";
}

int main(){
  const char * logfile = "out.log";

	int success;
	saul::Window* main_w = new saul::Window(success,"Testing", SDL_WINDOW_SHOWN, 500, 500); //SDL_WINDOWPOS_UNDEFINED
	main_w->setDflagE(SAUL_DEGUG_ALL, true);
  main_w->logfname = logfile;
	TTF_Init();
	if(!success){
		saul::Log(logfile, "INFO", "main", "Window created successfully");
	}
	//setting up TTF
	TTF_Font* font = TTF_OpenFont("../src/fonts/DejaVuSans.ttf", 25);
	if(font == NULL){
		saul::Log(logfile, "WARNING", "main", "font is NULL");
		font = TTF_OpenFont("./src/fonts/DejaVuSans.ttf", 25);
		if(font == NULL){
			std::string logmsg = "Unable to load font DejaVuSans.ttf, TTF_GetError(): ";
			logmsg += TTF_GetError();
			saul::Log(logfile, "ERROR", "main", logmsg.c_str());
			return -1;
		}
        }

	// sample saul::TextU to render
	struct saul::TextU* T_text = new saul::TextU;
	//T_text.t_surface  = saul::textToSurface(font,"test text", (SDL_Color){255,0,0}); // TODO: ERROR HERE, segmentation fault
	T_text->t_surface  = TTF_RenderText_Solid(font, "TEST", {255,255,0});
	T_text->boundaries = {0,0,500,500};
	main_w->updateTextU(*T_text);

	// sample saul::Texture to render
	struct saul::Texture* T_tex = new saul::Texture;
	T_tex->texture    = main_w->LoadTexture((char *)"../src/textures/500x500.png");
	if(T_tex->texture == NULL){
		saul::Log(logfile, "WARNING", "main", "T_tex.texture is NULL");
		T_tex->texture = main_w->LoadTexture((char *)"./src/textures/500x500.png");
		if(T_tex->texture == NULL){
			std::string logmsg = "Unable to load image 500x500.png, IMG_GetError(): ";
			logmsg += IMG_GetError();
			saul::Log(logfile, "ERROR", "main", logmsg.c_str());
			return -1;
		}
    }
	T_tex->boundaries = {0,0,500,500};

	// sample saul::TexturePrimitive to render
	const struct SDL_Color ptex_col = {0,255,0};
	const struct SDL_Rect ptex_rec = {245,245,10,10};
	struct saul::TexturePrimitive* T_ptex = new saul::TexturePrimitive {
		ptex_col,
		ptex_rec,
	};

	//sample saul::Button
	const struct SDL_Color btn_col = {255, 0, 100};
	// Make a helper TextU stuct, that will not be rendered
	struct saul::TextU T_htext;
	T_htext.t_surface  = TTF_RenderText_Solid(font, "BUTTON", {0,255,255});
	main_w->updateTextU(T_htext);
	const struct SDL_Rect btn_bgboundaries = {100,100,300,50};
	const struct SDL_Rect btn_txboundaries = {100,100,300,50};
	struct saul::Button* T_btn = new saul::Button {
		btn_col,
		NULL,
		T_htext.t_texture,
		btn_bgboundaries,
		btn_txboundaries,
                0,
                true,
	};

        // create the objects
        saul::UIObj btn_object     = {"sample button",    saul::TYPE::BUTTON,           (void*)T_btn,     true};
        saul::UIObj text_object    = {"test text",        saul::TYPE::TEXTU,            (void*)T_text,    true};
        saul::UIObj tex_object     = {"500x500 texture",  saul::TYPE::TEXTURE,          (void*)T_tex,     true};
        saul::UIObj primtex_object = {"blue, 10x10 box",  saul::TYPE::TEXTUREPRIMITIVE, (void*)T_ptex,    true};

        // the objects put into list and put into a scene
        std::list<saul::UIObj> objlist;
        objlist.push_back(btn_object);
        objlist.push_back(text_object);
        objlist.push_back(tex_object);
        objlist.push_back(primtex_object);

	struct saul::Scene sc1 = {
		objlist,
                "default scene"
	};
        {
                std::string fullText = "Scene list size: " +     std::to_string(main_w->addScene(sc1));
    	        saul::Log(logfile, "INFO", "main", fullText.c_str());
        }
	main_w->setActiveScene(0);

	if(main_w->render()==-1){};
	SDL_Delay(2000); // cannot close for 2 seconds, only here to see the first frame

	for(int i = 0; i<10000; ++i){
		if(main_w->render()==-1){break;};
        /*
        sc1 = main_w->getScene(0);
        auto f_btn = sc1.buttons.begin();
        {
            std::string logmsg = "Button clicked ";
            logmsg += std::to_string(f_btn->upclicks);
            logmsg += " times";
            saul::Log("INFO", "main", logmsg.c_str());
        }
        */
		//SDL_Delay(5);
	}

	/*while (1){
	 	if(main_w->render()==-1){break;};
	}*/

	//clean up
	delete main_w; // requires that handleEvents does not delete the window
	SDL_Quit();
	TTF_CloseFont(font);
	TTF_Quit();
	return 0;
}
