#include "Pixel.cpp"

int main(int argsc, char *argsv[]) {
    SDL_Init( SDL_INIT_EVERYTHING );

    SDL_Window *window = SDL_CreateWindow("Procedural Fish \u00A9DecCatBurner 2024", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);

    SDL_Renderer *rend = SDL_CreateRenderer(window, -1, 1);

    Draw::SetRend(rend);

    if (window == NULL) {
        std::cout << "Could not create window: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Event windowEvent;
    // End SDL init

    // Variable init
    float itime = 0.0f;
    int frameRate = 50;
    int frame = frameRate;
    bool pause = false;

    Pixel_ID placeID = Pixel_ID::sand;
    
    // Initialize board
    Board board = Board(10);
    std::cout << "Declared as " << board << std::endl;
    board.Initialize();
    std::cout << "Initialized" << std::endl;

    for (int x = 0; x < board.size.x; x++) {
        board.pixels[x + board.size.x * 5].id = Pixel_ID::sand;
    }
    std::cout << "Land" << std::endl;

    Draw::CircleOutline(origin, 20.0f);

    SDL_RenderPresent(rend);

    while (true) {
        SDL_Delay(1); //Wait each frame for consitancy

        itime += 0.1f;
        frame++;

        // Initial Mouse State
        int mx, my;
        SDL_GetMouseState(&mx, &my);
        Vector2 mouse = Vector2(mx, my);
        if (!pause) {
            if (frame <= frameRate) { goto Button; }
            frame = 0;
            // Clear Screen each frame
            SDL_SetRenderDrawColor(rend, 0, 60, 180, 255);
            SDL_RenderClear(rend);

            // Act on each pixel
            board.Act();

            // Draw each pixel
            board.Draw();

            // Render scene
            SDL_RenderPresent( rend );   
        }

        Button:

        if (SDL_PollEvent( &windowEvent )) {
            switch(windowEvent.type ){
                /* Keyboard event */
                case SDL_KEYDOWN:
                    switch(windowEvent.key.keysym.sym) {
                        case SDLK_p:
                            pause = !pause;
                            break;
                        case SDLK_c:
                            Draw::TakeScreenShot(window, itime);
                            std::cout << "Snap taken at: " << itime << std::endl;
                            break;
                        case SDLK_0:
                            placeID = Pixel_ID::empty;
                            break;
                        case SDLK_1:
                            placeID = Pixel_ID::sand;
                            break;
                        case SDLK_2:
                            placeID = Pixel_ID::stone;
                            break;
                        case SDLK_3:
                            placeID = Pixel_ID::water;
                            break;
                        default:
                            break;
                    }
                    break;
                case SDL_KEYUP:
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    board.SetPixel(IntVector2(mouse) / board.pixelSize, placeID);
                    break;
                /* SDL_QUIT event (window close) */
                case SDL_QUIT:
                    std::cout << "Window Falure" << std::endl;
                    goto Exit;

                default:
                    break;
            }
        }
    }
    Exit:

    SDL_DestroyWindow( window );
    SDL_DestroyRenderer( rend );
    SDL_Quit();

    return EXIT_SUCCESS;
} // mingw32-make