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
void graphicmode(char** argv, tripnode** _triplist, stationnode**_stationlist)
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
    int weekday;

    // initialize graphics
    InitEverything(width, height, &serif, imgs, &window, &renderer);
    //Render the initial window
    RenderTable( board_pos, serif, imgs, renderer, _triplist, _stationlist);

    //Initial messages
    SDL_ShowSimpleMessageBox(0,"Welcome!","Welcome to BikeStation program! You chose graphic mode", window);
    SDL_ShowSimpleMessageBox(0,"Start","Please start by choosing in the terminal a day of the week to show the routes", window);

    //Register the weekday
    weekday=criterion2(_triplist);

    SDL_ShowSimpleMessageBox(0,"Rules","Good! It will now appear a circle with all the stations. Click in a station to see the routes from (green) and to (blue) those stations\n", window);

    while( quit == 0)
    {
        //On all the cycles except the first we need to re-eliminate the nodes, because we always reset the list after a click
        *_triplist=RemoveUsingWeekday(weekday,*_triplist);
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
                        RenderTable( board_pos, serif, imgs, renderer, _triplist, _stationlist);
                        SDL_GetMouseState(&x, &y);
                        DrawRoutes(x,y,_stationlist, _triplist, window, renderer);
                        break;
                    default:
                        SDL_ShowSimpleMessageBox(0,"Mouse","Invalid button pressed\n", window);
                        break;

                }
            }
        }
        //Reset the lists
        ClearData(argv, _triplist, _stationlist, 0);
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
    float increment=0.0f;
    SDL_Rect rectangle;
    int x,y;
    double theta=0;

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

    // render the students names
    height += RenderText(TABLE_SIZE+3*MARGIN, height, myName1, _font, &black, _renderer);

    // this renders the students numbers
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

    //Draw the circle
    DrawCircle(_renderer, radius, coordx0, coordy0, coordx, coordy, err);

    //Set the red color
    SDL_SetRenderDrawColor(_renderer, 255,0,0,0);

    current=*_stationlist;

    //Count yhe number of stations
    while(current!=NULL)
    {
        numofstations++;
        current=current->next;
    }


    //This increment variable will set the space between the rectangles showing the stations
    increment=(float)(2*M_PI)/numofstations;

    current=*_stationlist;

    //For all the stations we start at initial points (with theta=0)
    //THen we increment the theta using the increment value
    while(current!=NULL)
    {
        x=(int)coordx0+radius*cos(theta);
        y=(int)coordy0+radius*sin(theta);

        rectangle.x=x;
        rectangle.y=y;
        rectangle.w=10;
        rectangle.h=10;

        SDL_RenderFillRect(_renderer,&rectangle);

        //Save the coordinates for the station, for later access
        current->station_file.x=x;
        current->station_file.y=y;

        current=current->next;
        theta+=increment;
    }

    // destroy everything
    SDL_DestroyTexture(table_texture);
}


/**
 * DrawCircle: Draws the circle to display the stations
 * \param _renderer renderer to handle all rendering in a window
 * \param radius: the radius of the circle we're rendering
 * \param coordx0: x coordinate of the center of the circle
 * \param coordy0: y coordinate of the center of the circle
 * \param coordx: auxiliary x coordinate to draw the circle
 * \param coordy: auxiliary y coordinate to draw the circle
 * \param err: parameter to control the rendering of the circle
 */

void DrawCircle (SDL_Renderer* _renderer, int radius, int coordx0, int coordy0, int coordx, int coordy, int err)
{

    while(coordx>=coordy)
    {
        //The algorithm chosen finds a path through the pixel grid pixels which are as close as possible to solutions of
        //x² +y² =r²:
        //At each step, the path is extended by choosing the adjacent pixel which satisfies the equation of the circle
        // but also maximizes x² + y²

        //The algorithm starts with the equation of the circle centered in x0, y0.
        //FOr simplicity let's assume the circle is centered in (0,0) (not our case).

        //We first consider only the first octant and draw a curve that starts in (r,0) and proceeds
        // counterclockwise until the degree of 45

        //The fast direction is the y direction so the algorithm always takes a step in the positive y direction
        //and occasionally it goes in the slow direction (negative x direction)

        //The equation of the circle is equivalent to x²+y²-r²=0
        //We let the points in the circle to be a sequence of coordinates of the vector to the points
        //The points are numbered according to the order in which are drawn (n=1 is the first point (r,0)

        //For each point; xn² +yn²=r² -> xn² =r²-yn² which means xn+1² = r²-yn+1²
        //Since the next point will always be at least 1 pixel higher than the last it's true that
        // yn+1²=(yn+1)²=yn²+2yn+1
        // xn+1²=r²-yn²-2yn-1
        // xn+1²=xn²-2yn-1

        //Because of the continuity of a circle and because the maxima along both axes is the same, we'll not be skipping x points
        //as it advances in the sequence.
        //Usually it stays on the same x coordinate, and sometimes advances by one.
        //The resulting coordinate is then translated by adding midpoint coordinates.
        //The zero in the transformed circle equation is replaced by the error term.
        //The initialization of the error term is derived from an offset of 1/2 pixel at the start.

        //Until the intersection with the perpendicular line, this leads to an accumulated value of r in the error term,
        //so that this value is used for initialization

        SDL_RenderDrawPoint(_renderer, coordx0+coordx, coordy0+coordy);
        SDL_RenderDrawPoint(_renderer, coordx0+coordy, coordy0+coordx);
        SDL_RenderDrawPoint(_renderer, coordx0-coordy, coordy0+coordx);
        SDL_RenderDrawPoint(_renderer, coordx0-coordx, coordy0+coordy);
        SDL_RenderDrawPoint(_renderer, coordx0-coordx, coordy0-coordy);
        SDL_RenderDrawPoint(_renderer, coordx0-coordy, coordy0-coordx);
        SDL_RenderDrawPoint(_renderer, coordx0+coordy, coordy0-coordx);
        SDL_RenderDrawPoint(_renderer, coordx0+coordx, coordy0-coordy);

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

/**
 * DrawRoutes: It identifies the station clicked and sends us to other function to draw the routes
 * \param _x x coordinate clicked
 * \param _y y coordinate clicked
 * \param _stationlist (list of stations)
 * \param _triplist (list of trips)
 * \param window represents the window of the application
 * \param _renderer renderer to handle all rendering in a window
 */

void DrawRoutes(int _x, int _y, stationnode** _stationlist, tripnode** _triplist, SDL_Window* window, SDL_Renderer* _renderer)
{
    stationnode* current=NULL;//auxiliary pointer to iterate over the station list

    current=*_stationlist;

    while(current!=NULL)
    {
        //We look if the user clicked in any coordinates of a station
        //Since the stations are represented by squares, we consider a small range (the size of the square)
        //for the click
        //When we find a station that matches we break the cycle
        if(_x>=current->station_file.x && _x<=current->station_file.x+10 && _y>=current->station_file.y &&  _y<=current->station_file.y+10)
            break;

        current=current->next;
    }

    //If there wasn't a station found, we display a message on the screen and return immediately
    if(current==NULL)
    {
        SDL_ShowSimpleMessageBox(0,"Failure","Sorry, you didn't click a station\n", window);
        return;
    }

    //If it's not we print on the terminal the station chosen
    printf("You chose the station: %d -> %s\n", current->station_file.stationID, current->station_file.station);

    graphicroutelisting(_triplist, _stationlist, current->station_file.stationID, _renderer);

}

/**
 * graphicroutelisting: It eliminates the unnecessary nodes and sorts the list depending on the station chosen
 * \param _stationlist (list of stations)
 * \param _triplist (list of trips)
 * \param _ID the ID of the station chosen
 * \param _renderer renderer to handle all rendering in a window
 */

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

    num=_ID; //we attribute the ID chosen

    //We start by organizing our trip list to make it easier to print data later

    current=*_triplist;
    prev=current;

    if(current==NULL)
        return;

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

    if(stopbegin==NULL)
        return;

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

/**
 * graph_routelist function: draws the routes
 * _triplist --> the head node of the trip list
 * _stationlist -> the head node of the station list
 * end --> the head node of the "sublist" of trips (part of the list that contains the stationID as destination)
 * num --> station ID number
 * stationdepart --> vector with the number of trips between station ID and the respective value pos (as destination)
 * stationdepart --> vector with the number of trips between station ID and the respective value pos (as origin)
 */

void graph_routelist(tripnode** _triplist, stationnode** _stationlist, tripnode* end, int num, int stationdepart[STATIONMAX], int stationarrive[STATIONMAX], SDL_Renderer* _renderer)
{
    tripnode* current=NULL; //auxiliar pointer to iterate over the trip list
    stationnode* search=NULL; //auxiliar pointer to iterate over the station list
    int xini=0, yini=0, xfin=0, yfin=0; //extreme points to draw a line
    int stationbegID=0; //variable to store the start station
    int stationendID=0;//variable to store the stop station
    int i=0,j=0; //variables to use in cycles
    int thickness=0; //variable to store the thickness of the line to draw

    current=*_triplist;

    if(current==NULL)
    {
        printf("Empty list\n");
        return;
    }

    search=*_stationlist;

    //We store the station start ID
    stationbegID=current->trip_file.stationstartID;

    //We look for this station in the station list so that we can determinate the coordinates of the station
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

    //We set green
    SDL_SetRenderDrawColor(_renderer, 0,255,0,0);
    //We print the routes
    while(current!=end)
    {
        if(current==NULL)
            return;

        //Store the stop station ID
        stationendID=current->trip_file.stationstopID;
        search=*_stationlist;
        //Look for the station to get the respective coordinates
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

        //If the number of trips is different from zero, we draw lines
        if(stationdepart[current->trip_file.stationstopID]!=0)
        {
            //we apply the linear correlation defined
            //thickness = 0.16*number of trips + 0.84
            thickness=(int)(0.16*stationdepart[current->trip_file.stationstopID]+0.84);
            for(i=-1; i<=thickness; i++)
            {
                for(j=1; j<=thickness; j++)
                    SDL_RenderDrawLine(_renderer, xini+i, yini+j, xfin+i, yfin+j);
            }
        }

        current=current->next;//iterate over the list
    }

    //We do the same but now considering the chosen station as end point
    //The algorithm is analogous
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

    //We set up blue
    SDL_SetRenderDrawColor(_renderer, 0,0,255,0);

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

        if(stationarrive[end->trip_file.stationstartID]!=0)
        {
            thickness=(int)(0.16*stationarrive[end->trip_file.stationstartID]+0.84);
            for(i=-1; i<=thickness; i++)
            {
                for(j=1; j<=thickness; j++)
                    SDL_RenderDrawLine(_renderer, xini+i, yini+j, xfin+i, yfin+j);
            }
        }


        current=current->next;//iterate over the list
    }

}
