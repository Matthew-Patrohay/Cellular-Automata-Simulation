#ifndef Functions_h
#define Functions_h
//--------- Declare functions ---------
void makeSquare(float x_cord, float y_cord, float z_cord, float red_value, float green_value, float blue_value, float width);
static void error_callback(int error, const char* description);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
float randomZeroOne(void);
void render_simulation_frame(simulation_pixel pixel_data_array[(int)SIMULATION_GRID_RESOLUTION][(int)SIMULATION_GRID_RESOLUTION]);

//--------- Define functions ---------
// Create a square with the input of the center of the square, rgb color of the square, and the width
void makeSquare(float x_cord, float y_cord, float z_cord, float red_value, float green_value, float blue_value, float width)
{
    float half_width = width / 2;
    // Point A Generation
    triangle_point point_A;
    point_A.x_value = x_cord - half_width;
    point_A.y_value = y_cord - half_width;
    point_A.z_value = 0;
    
    // Point B Generation
    triangle_point point_B;
    point_B.x_value = x_cord - half_width;
    point_B.y_value = y_cord + half_width;
    point_B.z_value = 0;
    
    // Point C Generation
    triangle_point point_C;
    point_C.x_value = x_cord + half_width;
    point_C.y_value = y_cord - half_width;
    point_C.z_value = 0;
    
    // Point D Generation
    triangle_point point_D;
    point_D.x_value = x_cord + half_width;
    point_D.y_value = y_cord + half_width;
    point_D.z_value = 0;
    
    // Generate a triangle strip which connects four vertices
    glBegin(GL_TRIANGLE_STRIP);
    glColor3f(red_value, green_value, blue_value);
    glVertex3f(point_A.x_value, point_A.y_value, point_A.z_value);
    glVertex3f(point_B.x_value, point_B.y_value, point_B.z_value);
    glVertex3f(point_C.x_value, point_C.y_value, point_C.z_value);
    glVertex3f(point_D.x_value, point_D.y_value, point_D.z_value);
    glEnd();
}
static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

// Return a quasi-random float value between 0 and 1
float randomZeroOne(void)
{
    return((float) rand() / RAND_MAX);
}

void render_simulation_frame(simulation_pixel pixel_data_array[(int)SIMULATION_GRID_RESOLUTION][(int)SIMULATION_GRID_RESOLUTION])
{
    // Render Pixels
    for (int i = 0; i < SIMULATION_GRID_RESOLUTION; i++)
    {
        for (int j = 0; j < SIMULATION_GRID_RESOLUTION; j++) {
            // Wall Rendering
            if (pixel_data_array[i][j].element == WALL)
            {
                makeSquare(-1 + ((1/SIMULATION_GRID_RESOLUTION) * i * 2) + (1/SIMULATION_GRID_RESOLUTION), -1 + ((1/SIMULATION_GRID_RESOLUTION) * j * 2) + (1/SIMULATION_GRID_RESOLUTION), 0, 0.2, 0.2, 0.2, 2/SIMULATION_GRID_RESOLUTION);
            }
            // Water Rendering
            if (pixel_data_array[i][j].element == WATER)
            {
                makeSquare(-1 + ((1/SIMULATION_GRID_RESOLUTION) * i * 2) + (1/SIMULATION_GRID_RESOLUTION), -1 + ((1/SIMULATION_GRID_RESOLUTION) * j * 2) + (1/SIMULATION_GRID_RESOLUTION), 0, 0, 0, 1, 2/SIMULATION_GRID_RESOLUTION);
            }
            // Air Rendering
            if (pixel_data_array[i][j].element == AIR)
            {
                makeSquare(-1 + ((1/SIMULATION_GRID_RESOLUTION) * i * 2) + (1/SIMULATION_GRID_RESOLUTION), -1 + ((1/SIMULATION_GRID_RESOLUTION) * j * 2) + (1/SIMULATION_GRID_RESOLUTION), 0, 0.8, 0.8, 1, 2/SIMULATION_GRID_RESOLUTION);
            }
        }
    }
}
#endif
