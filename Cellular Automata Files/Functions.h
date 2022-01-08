#ifndef Functions_h
#define Functions_h
//--------- Declare functions ---------
void makeSquare(float x_cord, float y_cord, float z_cord, float red_value, float green_value, float blue_value, float width);
static void error_callback(int error, const char* description);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
float randomZeroOne(void);

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
#endif
