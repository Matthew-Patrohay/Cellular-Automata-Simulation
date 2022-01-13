#ifndef Header_h
#define Header_h
//--------- Global Variables ---------
int SIMULATION_WIDTH = 100; // Width of Game Window
int SIMULATION_HEIGHT = 100; // Height of Game Window
float SIMULATION_GRID_RESOLUTION = 50; // Width and Height (Square) of Simulation Grid (Minimum of 5 because of walls)

//--------- Debugging Setting Variables ---------
float slowspeed = 0; // Slow down each frame by this many seconds
bool countWaterON = false; // Count number of water particles being rendered and print value;

int waterCount = 0;
int frameCount = 0;

//--------- Structures ---------
// A single point for triangle generation
typedef struct triangle_point
{
    float x_value;
    float y_value;
    float z_value;
} triangle_point;

// The defining color for a pixel
typedef struct rgb_color_value
{
    float red;
    float green;
    float blue;
} rgb_color_value;

// The element of the pixel
typedef enum pixel_element
{
    WATER = 'w',
    AIR = '#',
    WALL = 'x',
    STONE = 's'
} pixel_element;

// The velocity Data for a pixel
typedef struct velocity_vector
{
    int x;
    int y;
} velocity_vector;

// All the data stored in a pixel
typedef struct simulation_pixel
{
    bool updated_this_cycle;
    velocity_vector velocity;
    pixel_element element;
} simulation_pixel;

#endif
