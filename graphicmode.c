#include "graphicmode.h"


// definition of some strings: they cannot be changed when the program is executed !
const char myName1[] = "Diogo Costa";
const char myNumber1[] = "ist186971";
const char myName2[] = "Pedro Azevedo";
const char myNumber2[] = "ist425696";

/**
 * graphic mode function: entry point of the program
 * only to invoke other functions !
 */
void graphicmode(tripnode** _triplist, stationnode**_stationlist)
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    TTF_Font *serif = NULL;
    SDL_Surface *imgs[2];
    SDL_Event event;
    int delay = 300;
    int quit = 0;
    int width = (TABLE_SIZE + LEFT_BAR_SIZE);
    int height = TABLE_SIZE;
    int board_pos = 5;
    int x,y;

    // initialize graphics
    InitEverything(width, height, &serif, imgs, &window, &renderer);

    SDL_ShowSimpleMessageBox(0,"Welcome!","Welcome to BikeStation program! You chose graphic mode", window);
    SDL_ShowSimpleMessageBox(0,"Start","Please start by choosing in the terminal a day of the week to show the routes", window);

    criterion2(_triplist);

    SDL_ShowSimpleMessageBox(0,"Rules","Good! It will now appear a circle with all the stations. Click in a station to see the routes from and to those stations\n", window);

    while( quit == 0 )
    {
        // while there's events to handle
        while( SDL_PollEvent( &event ) )
        {
            if( event.type == SDL_QUIT )
            {
                quit=1;
            }

            else if(event.type==SDL_MOUSEBUTTONDOWN)
            {
                switch(event.button.button)
                {
                    case SDL_BUTTON_LEFT:
                    case SDL_BUTTON_RIGHT:
                        SDL_GetMouseState(&x, &y);
                        DrawRoutes(x,y,_stationlist, _triplist, window, renderer);
                        break;
                    default:
                        SDL_ShowSimpleMessageBox(0,"Mouse","Invalid button pressed\n", window);
                        break;

                }
            }
        }
        // render game table
        RenderTable( board_pos, serif, imgs, renderer, _triplist, _stationlist);
        // render in the screen all changes above
        SDL_RenderPresent(renderer);
        // add a delay
        SDL_Delay( delay );
    }

    // free memory allocated for images and textures and closes everything including fonts
    TTF_CloseFont(serif);
    SDL_FreeSurface(imgs[0]);
    SDL_FreeSurface(imgs[1]);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

/**
 * RenderTable: Draws the table where the game will be played, namely:
 * -  some texture for the background
 * -  the right part with the IST logo and the student name and number
 * -  the grid for game board with squares and seperator lines
 * \param _board_pos number of squares in the board
 * \param _font font used to render the text
 * \param _img surfaces with the table background and IST logo (already loaded)
 * \param _renderer renderer to handle all rendering in a window
 */
void RenderTable( int _board_pos, TTF_Font *_font, SDL_Surface *_img[], SDL_Renderer* _renderer, tripnode** _triplist, stationnode** _stationlist)
{
    SDL_Color black = { 0, 0, 0 }; // black
    SDL_Texture *table_texture;
    SDL_Rect tableSrc, tableDest;
    int height;
    int coordx0, coordy0, radius, coordx, coordy, err;
    int numofstations=0;
    stationnode* current;

    // set color of renderer to some color
    SDL_SetRenderDrawColor( _renderer, 255, 255, 255, 255 );

    // clear the window
    SDL_RenderClear( _renderer );

    tableDest.x = tableSrc.x = 0;
    tableDest.y = tableSrc.y = 0;
    tableSrc.w = _img[0]->w;
    tableSrc.h = _img[0]->h;
    tableDest.w = TABLE_SIZE;
    tableDest.h = TABLE_SIZE;

    // draws the table texture
    table_texture = SDL_CreateTextureFromSurface(_renderer, _img[0]);
    SDL_RenderCopy(_renderer, table_texture, &tableSrc, &tableDest);

    // render the IST Logo
    height = RenderLogo(TABLE_SIZE, 0, _img[1], _renderer);

    // render the student name
    height += RenderText(TABLE_SIZE+3*MARGIN, height, myName1, _font, &black, _renderer);

    // this renders the student number
    height += RenderText(TABLE_SIZE+3*MARGIN, height, myNumber1, _font, &black, _renderer);
    height=height+20;
    height += RenderText(TABLE_SIZE+3*MARGIN, height, myName2, _font, &black, _renderer);
    RenderText(TABLE_SIZE+3*MARGIN, height, myNumber2, _font, &black, _renderer);

    radius=300;
    coordx0=325;
    coordy0=325;
    coordx=radius;
    coordy=0;
    err=0;

    DrawCircle(_renderer, radius, coordx0, coordy0, coordx, coordy, err);

    SDL_SetRenderDrawColor(_renderer, 255,0,0,0);

    current=*_stationlist;

    while(current!=NULL)
    {
        numofstations++;
        current=current->next;
    }

    float increment=0.0f;

    increment=(float)(2*M_PI)/numofstations;
    SDL_Rect rectangle;
    int x,y;
    double theta=0;

    current=*_stationlist;

    while(current!=NULL)
    {
        x=(int)coordx0+radius*cos(theta);
        y=(int)coordy0+radius*sin(theta);

        rectangle.x=x;
        rectangle.y=y;
        rectangle.w=10;
        rectangle.h=10;

        SDL_RenderFillRect(_renderer,&rectangle);

        current->station_file.x=x;
        current->station_file.y=y;

        current=current->next;
        theta+=increment;
    }

    // destroy everything
    SDL_DestroyTexture(table_texture);
}


void DrawCircle (SDL_Renderer* _renderer, int radius, int coordx0, int coordy0, int coordx, int coordy, int err)
{
    while(coordx>=coordy)
    {
        SDL_RenderDrawPoint(_renderer, coordx0+coordx, coordy0+coordy);//1st octant
        SDL_RenderDrawPoint(_renderer, coordx0+coordy, coordy0+coordx);//2nd octant
        SDL_RenderDrawPoint(_renderer, coordx0-coordy, coordy0+coordx);//3rd octant
        SDL_RenderDrawPoint(_renderer, coordx0-coordx, coordy0+coordy);//4th octant
        SDL_RenderDrawPoint(_renderer, coordx0-coordx, coordy0-coordy);//5th octant
        SDL_RenderDrawPoint(_renderer, coordx0-coordy, coordy0-coordx);//6th octant
        SDL_RenderDrawPoint(_renderer, coordx0+coordy, coordy0-coordx);//7th octant
        SDL_RenderDrawPoint(_renderer, coordx0+coordx, coordy0-coordy);//8th octant

        coordy++;

        if(err<=0)
        {
            err+=2*coordy+1;
        }

        if(err>0)
        {
            coordx--;
            err-=2*coordx+1;
        }

    }

}

/**
 * RenderLogo function: Renders the IST logo on the app window
 * \param x X coordinate of the Logo
 * \param y Y coordinate of the Logo
 * \param _logoIST surface with the IST logo image to render
 * \param _renderer renderer to handle all rendering in a window
 */
int RenderLogo(int x, int y, SDL_Surface *_logoIST, SDL_Renderer* _renderer)
{
    SDL_Texture *text_IST;
    SDL_Rect boardPos;

    // space occupied by the logo
    boardPos.x = x;
    boardPos.y = y;
    boardPos.w = _logoIST->w;
    boardPos.h = _logoIST->h;

    // render it
    text_IST = SDL_CreateTextureFromSurface(_renderer, _logoIST);
    SDL_RenderCopy(_renderer, text_IST, NULL, &boardPos);

    // destroy associated texture !
    SDL_DestroyTexture(text_IST);
    return _logoIST->h;
}

/**
 * RenderText function: Renders some text on a position inside the app window
 * \param x X coordinate of the text
 * \param y Y coordinate of the text
 * \param text string with the text to be written
 * \param _font TTF font used to render the text
 * \param _color color of the text
 * \param _renderer renderer to handle all rendering in a window
 */
int RenderText(int x, int y, const char *text, TTF_Font *_font, SDL_Color *_color, SDL_Renderer* _renderer)
{
    SDL_Surface *text_surface;
    SDL_Texture *text_texture;
    SDL_Rect solidRect;

    solidRect.x = x;
    solidRect.y = y;
    // create a surface from the string text with a predefined font
    text_surface = TTF_RenderText_Blended(_font,text,*_color);
    if(!text_surface)
    {
        printf("TTF_RenderText_Blended: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
    // create texture
    text_texture = SDL_CreateTextureFromSurface(_renderer, text_surface);
    // obtain size
    SDL_QueryTexture( text_texture, NULL, NULL, &solidRect.w, &solidRect.h );
    // render it !
    SDL_RenderCopy(_renderer, text_texture, NULL, &solidRect);
    // clear memory
    SDL_DestroyTexture(text_texture);
    SDL_FreeSurface(text_surface);
    return solidRect.h;
}

/**
 * InitEverything: Initializes the SDL2 library and all graphical components: font, window, renderer
 * \param width width in px of the window
 * \param height height in px of the window
 * \param _font font that will be used to render the text
 * \param _img surface to be created with the table background and IST logo
 * \param _window represents the window of the application
 * \param _renderer renderer to handle all rendering in a window
 */
void InitEverything(int width, int height, TTF_Font **_font, SDL_Surface *_img[], SDL_Window** _window, SDL_Renderer** _renderer)
{
    InitSDL();
    InitFont();
    *_window = CreateWindow(width, height);
    *_renderer = CreateRenderer(width, height, *_window);

    // load the table texture
    _img[0] = IMG_Load("index.png");
    if (_img[0] == NULL)
    {
        printf("Unable to load image: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // load IST logo
    _img[1] = SDL_LoadBMP("ist_logo.bmp");
    if (_img[1] == NULL)
    {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    // this opens (loads) a font file and sets a size
    *_font = TTF_OpenFont("OpenSans.ttf", 16);
    if(!*_font)
    {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
}

/**
 * InitSDL: Initializes the SDL2 graphic library
 */
void InitSDL()
{
    // init SDL library
    if ( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        printf(" Failed to initialize SDL : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}

/**
 * InitFont: Initializes the SDL2_ttf font library
 */
void InitFont()
{
    // Init font library
    if(TTF_Init()==-1)
    {
        printf("TTF_Init: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
}

/**
 * CreateWindow: Creates a window for the application
 * \param width width in px of the window
 * \param height height in px of the window
 * \return pointer to the window created
 */
SDL_Window* CreateWindow(int width, int height)
{
    SDL_Window *window;
    // init window
    window = SDL_CreateWindow( "Bikestation-Graphic Mode", WINDOW_POSX, WINDOW_POSY, width, height, 0 );
    // check for error !
    if ( window == NULL )
    {
        printf("Failed to create window : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    return window;
}

/**
 * CreateRenderer: Creates a renderer for the application
 * \param width width in px of the window
 * \param height height in px of the window
 * \param _window represents the window for which the renderer is associated
 * \return pointer to the renderer created
 */
SDL_Renderer* CreateRenderer(int width, int height, SDL_Window *_window)
{
    SDL_Renderer *renderer;
    // init renderer
    renderer = SDL_CreateRenderer( _window, -1, 0 );

    if ( renderer == NULL )
    {
        printf("Failed to create renderer : %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // set size of renderer to the same as window
    SDL_RenderSetLogicalSize( renderer, width, height );

    return renderer;
}


void DrawRoutes(int _x, int _y, stationnode** _stationlist, tripnode** _triplist, SDL_Window* window, SDL_Renderer* _renderer)
{
    stationnode* current=NULL;

    current=*_stationlist;

    while(current!=NULL)
    {
        if(_x>=current->station_file.x && _x<=current->station_file.x+10 && _y>=current->station_file.y &&  _y<=current->station_file.y+10)
            break;

        current=current->next;
    }

    if(current==NULL)
    {
        SDL_ShowSimpleMessageBox(0,"Failure","Sorry, you didn't click a station\n", window);
        return;
    }

    graphicroutelisting(_triplist, _stationlist, current->station_file.stationID, _renderer);

}

void graphicroutelisting(tripnode** _triplist, stationnode** _stationlist, int _ID, SDL_Renderer* _renderer)
{
    int num=0; //variables to 1-save the station ID
    int station=0;//variable to save a station ID. Zero if there is no station.
    int stationarrive[STATIONMAX]= {0}, stationdepart[STATIONMAX]= {0};// vectors that contain the number of trips to a station (vector position=stationID)
    int tempstation[STATIONMAX]= {0}, tempstation2[STATIONMAX]= {0}; //temporary vectors that also contain that so that we can manipulate data without losing it
    tripnode* current=NULL; //auxiliar pointer to iterate over the list
    tripnode* prev=NULL; //auxiliar pointer to iterate over the list (one pointer behind the current one)
    tripnode* stopbegin=NULL; //auxiliar pointer that will help us distinguish in the sorted list when the stationID chosen is correspondent to a start station or a stop station
    int i=0; //variable for cycles

    num=_ID;

    //We start by organizing our trip list to make it easier to print data later

    current=*_triplist;
    prev=current;

    while(current!=NULL)
    {
        //If the current trip does not contain the chosen station, the node is removed
        if(current->trip_file.stationstartID!=num && current->trip_file.stationstopID!=num)
        {
            RemoveNode(&current,&prev,_triplist);
        }

        //If the current trip contains the chosen station as start
        else if(current->trip_file.stationstartID==num)
        {
            //If it matched the start station of the current trip, then we
            // (1) Increment the respective position of the stop station in the tempstation vector;
            // (2) Increment the respective position of the stop station in the stationdepart vector;
            tempstation[current->trip_file.stationstopID]++;
            stationdepart[current->trip_file.stationstopID]++;

            //If the station has already been incremented (already exists, the trip is removed from the list)
            //This will avoid repetitions in printing
            if(stationdepart[current->trip_file.stationstopID] > 1)
                RemoveNode(&current,&prev,_triplist);


            else
            {
                prev=current; //We save the current node
                current=current->next; //We go to the next node
            }

        }

        else
        {
            //If it matched the stop station of the current trip, then we
            // (1) Increment the respective position of the start station in the tempstation2 vector;
            // (2) Increment the respective position of the start stationin the stationarrive vector;
            tempstation2[current->trip_file.stationstartID]++;
            stationarrive[current->trip_file.stationstartID]++;

            //if the station has already been incremented (already exists, the trip is removed from the list)
            if(stationarrive[current->trip_file.stationstartID] > 1)
                RemoveNode(&current,&prev,_triplist);

            //if the station has not been analyzed yet
            else
            {
                prev=current; //We save the current node
                current=current->next; //We use the pointer to the next
            }

        }

    }


    //What remains of our trip list will now be sorted to differentiate the trips that our station as start station or as stop station
    stopbegin=SortTripList(_triplist,num);


    //Now we will sort the list again, ordering it by number of trips to the stop station
    for(i=0; i<STATIONMAX; i++)
    {

        station=order_bynumber(_triplist,stopbegin,tempstation);
        //the station value in the temporary vector is erased so it is not re-ordered
        tempstation[station]=0;

        //if the station value is 0 there are no more stations to be ordered
        if(station==0)
            break;
    }

    //We do the same now ordering the start stations by number of trips
    for(i=0; i<STATIONMAX; i++)
    {
        station=order_bynumber(&stopbegin,NULL,tempstation2);
        tempstation2[station]=0;

        if(station==0)
            break;
    }

    graph_routelist(_triplist,_stationlist, stopbegin,num,stationdepart,stationarrive, _renderer);
}


void graph_routelist(tripnode** _triplist, stationnode** _stationlist, tripnode* end, int num, int stationdepart[STATIONMAX], int stationarrive[STATIONMAX], SDL_Renderer* _renderer)
{
    tripnode* current=NULL; //auxiliar pointer to iterate over the list
    stationnode* search=NULL;
    int xini=0, yini=0, xfin=0, yfin=0;
    int stationbegID=0;
    int stationendID=0;

    current=*_triplist;
    search=*_stationlist;

    stationbegID=current->trip_file.stationstartID;

    while(search!=NULL)
    {
        if(search->station_file.stationID==stationbegID)
            break;

        search=search->next;
    }

    if(search==NULL)
        return;

    xini=search->station_file.x;
    yini=search->station_file.y;

    while(current!=end)
    {
        stationendID=current->trip_file.stationstopID;
        search=*_stationlist;
        while(search!=NULL)
        {
            if(search->station_file.stationID==stationendID)
                break;

            search=search->next;
        }

        if(search==NULL)
            return;

        xfin=search->station_file.x;
        yfin=search->station_file.y;

        SDL_RenderDrawLine(_renderer, xini, yini, xfin, yfin);

        if(stationdepart[current->trip_file.stationstopID]>30)
            SDL_RenderDrawLine(_renderer, xini, yini+1, xfin, yfin+1);

        if(stationdepart[current->trip_file.stationstopID]>60)
            SDL_RenderDrawLine(_renderer, xini, yini+2, xfin, yfin+2);

        if(stationdepart[current->trip_file.stationstopID]>90)
            SDL_RenderDrawLine(_renderer, xini, yini+3, xfin, yfin+3);

        if(stationdepart[current->trip_file.stationstopID]>120)
            SDL_RenderDrawLine(_renderer, xini, yini+4, xfin, yfin+4);

        if(stationdepart[current->trip_file.stationstopID]>150)
            SDL_RenderDrawLine(_renderer, xini, yini+5, xfin, yfin+5);

        if(stationdepart[current->trip_file.stationstopID]>180)
            SDL_RenderDrawLine(_renderer, xini, yini+6, xfin, yfin+6);

        if(stationdepart[current->trip_file.stationstopID]>210)
            SDL_RenderDrawLine(_renderer, xini, yini+7, xfin, yfin+7);

        if(stationdepart[current->trip_file.stationstopID]>240)
            SDL_RenderDrawLine(_renderer, xini, yini+8, xfin, yfin+8);


        current=current->next;//iterate over the list
    }

    stationendID=current->trip_file.stationstopID;

    while(search!=NULL)
    {
        if(search->station_file.stationID==stationendID)
            break;

        search=search->next;
    }

    if(search==NULL)
        return;

    xfin=search->station_file.x;
    yfin=search->station_file.y;

    while(current!=NULL)
    {
        stationbegID=current->trip_file.stationstartID;
        search=*_stationlist;
        while(search!=NULL)
        {
            if(search->station_file.stationID==stationbegID)
                break;

            search=search->next;
        }

        if(search==NULL)
            return;

        xini=search->station_file.x;
        yini=search->station_file.y;

        SDL_RenderDrawLine(_renderer, xini, yini, xfin, yfin);

        if(stationarrive[current->trip_file.stationstartID]>30)
            SDL_RenderDrawLine(_renderer, xini, yini+1, xfin, yfin+1);

        if(stationarrive[current->trip_file.stationstartID]>60)
            SDL_RenderDrawLine(_renderer, xini, yini+2, xfin, yfin+2);

        if(stationarrive[current->trip_file.stationstartID]>90)
            SDL_RenderDrawLine(_renderer, xini, yini+3, xfin, yfin+3);

        if(stationarrive[current->trip_file.stationstartID]>120)
            SDL_RenderDrawLine(_renderer, xini, yini+4, xfin, yfin+4);

        if(stationarrive[current->trip_file.stationstartID]>150)
            SDL_RenderDrawLine(_renderer, xini, yini+5, xfin, yfin+5);

        if(stationarrive[current->trip_file.stationstartID]>180)
            SDL_RenderDrawLine(_renderer, xini, yini+6, xfin, yfin+6);

        if(stationarrive[current->trip_file.stationstartID]>210)
            SDL_RenderDrawLine(_renderer, xini, yini+7, xfin, yfin+7);

        if(stationarrive[current->trip_file.stationstartID]>240)
            SDL_RenderDrawLine(_renderer, xini, yini+8, xfin, yfin+8);

        current=current->next;//iterate over the list
    }

}
