#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>

#include "game.h"
#include "rendering.h"
#include "logic.h"
#include "menu-items.h"

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Window *window = SDL_CreateWindow("Millionaire",
                                          700, 100,
                                          SCREEN_WIDTH, SCREEN_HEIGHT,
                                          SDL_WINDOW_SHOWN);

    if (window == NULL) {
        printf("error creating window: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1,
                                                SDL_RENDERER_ACCELERATED |
                                                SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        SDL_DestroyWindow(window);
        printf("error creating renderer: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }
    
    // Root user initialisation
    user_t root = {
        .id = "root",
        .password = "root",
        .admin = 1
    };

    // Current user
    user_t current_user; 

    node head = {&root, NULL};
    //load_users(&head);

    // Menu initialisation
    char* start = "START";
    char* login = "LOGIN";
    char* singup = "SIGNUP";
    char* stats = "STATS";
    char* instructions = "INSTRUCTIONS";
    char* logout = "LOGOUT";
    char* del = "DELETE ACCOUNT";
    char* grant = "GRANT ADMIN";
    char* add = "ADD QUESTION";
    char* back = "BACK";

    m_question init_menu = {
        .m_ans_a = login,
        .m_ans_b = singup,
        .m_ans_c = instructions,
        .m_ans_d = stats
    };

    m_question user_menu = {
        .m_ans_a = start,
        .m_ans_b = logout,
        .m_ans_c = instructions,
        .m_ans_d = stats
    };

    m_question admin_menu = {
        .m_ans_a = start,
        .m_ans_b = logout,
        .m_ans_c = instructions,
        .m_ans_d = stats
    };

    m_question admin_ops = {
        .m_ans_a = del,
        .m_ans_b = grant,
        .m_ans_c = add,
        .m_ans_d = back
    };

    menu_t menu = {
        .question = {init_menu,user_menu,admin_menu,admin_ops},
        .state = RUNNING,
        .selection = NO_SELECTION,
        .type = INIT_MENU,
        .user_id = ""
    };

    game_t* game = game_init(&menu);

    int x, y;

    SDL_Event e;
    while (menu.state != QUIT) {
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
            case SDL_QUIT:
                menu.state = QUIT;
                break;
            case SDL_KEYDOWN:
                switch (e.key.keysym.scancode){
                case SDL_SCANCODE_A:
                     if (menu.selection < 5)
                        menu.selection = A_SELECTED;
                    break;
                case SDL_SCANCODE_B:
                     if (menu.selection < 5)    
                        menu.selection = B_SELECTED;
                    break;
                case SDL_SCANCODE_C:
                     if (menu.selection < 5)    
                        menu.selection = C_SELECTED;
                    break;
                case SDL_SCANCODE_D:
                     if (menu.selection < 5)    
                        menu.selection = D_SELECTED;
                    break;
                case SDL_SCANCODE_R:
                    // Admin ops menu
                    if (menu.type == ADMIN_MENU){
                        menu.type = ADMIN_OPS;
                        menu.selection = NO_SELECTION;
                    }
                    break;
                case SDL_SCANCODE_RETURN:
                case SDL_SCANCODE_KP_ENTER:
                case SDL_SCANCODE_SPACE:
                    // confirm option
                    if (menu.selection > 0)
                        menu.selection += 4;
                    break;
                }
                break; 
           // Mouse selection 
            case SDL_MOUSEMOTION:
                SDL_GetMouseState(&x,&y);
                menu_hover_select(&menu, x, y);
                break;
            case SDL_MOUSEBUTTONDOWN:
                // confirm option
                if (menu.selection > 0)
                    menu.selection += 4;
                break;

            default: {}
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        render_menu(renderer, &menu);
        SDL_RenderPresent(renderer);

        check_menu_selection(renderer,game,&menu);
        // FPS
        SDL_Delay(1000/30);
        SDL_FlushEvent(SDL_KEYDOWN);
        SDL_FlushEvent(SDL_MOUSEBUTTONDOWN);
        SDL_FlushEvent(SDL_MOUSEMOTION);
    }    
    //game_t* game = game_init();
    //game_loop(renderer, game);

    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
