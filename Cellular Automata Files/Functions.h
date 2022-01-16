#ifndef Functions_h
#define Functions_h
//--------- Declare functions ---------
void makeSquare(float x_cord, float y_cord, float z_cord, float red_value, float green_value, float blue_value, float width);
static void error_callback(int error, const char* description);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
float randomZeroOne(void);
void render_frame(simulation_pixel pixel_data_array[(int)SIMULATION_GRID_RESOLUTION][(int)SIMULATION_GRID_RESOLUTION]);
void master_update(simulation_pixel pixel_data_array[(int)SIMULATION_GRID_RESOLUTION][(int)SIMULATION_GRID_RESOLUTION]);
void water_update(int x, int y, simulation_pixel pixel_data_array[(int)SIMULATION_GRID_RESOLUTION][(int)SIMULATION_GRID_RESOLUTION]);
void sand_update(int x, int y, simulation_pixel pixel_data_array[(int)SIMULATION_GRID_RESOLUTION][(int)SIMULATION_GRID_RESOLUTION]);

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
float randomZeroOne(void) {
    return((float) rand() / RAND_MAX);
}

// Render thr current state of the pixel array input
void render_frame(simulation_pixel pixel_data_array[(int)SIMULATION_GRID_RESOLUTION][(int)SIMULATION_GRID_RESOLUTION]) {
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
                if (countWaterON == true) {
                    waterCount++;
                }
            }
            
            // Air Rendering
            if (pixel_data_array[i][j].element == AIR)
            {
                makeSquare(-1 + ((1/SIMULATION_GRID_RESOLUTION) * i * 2) + (1/SIMULATION_GRID_RESOLUTION), -1 + ((1/SIMULATION_GRID_RESOLUTION) * j * 2) + (1/SIMULATION_GRID_RESOLUTION), 0, 0.8, 0.8, 1, 2/SIMULATION_GRID_RESOLUTION);
            }
            
            // Sand Rendering
            if (pixel_data_array[i][j].element == SAND)
            {
                makeSquare(-1 + ((1/SIMULATION_GRID_RESOLUTION) * i * 2) + (1/SIMULATION_GRID_RESOLUTION), -1 + ((1/SIMULATION_GRID_RESOLUTION) * j * 2) + (1/SIMULATION_GRID_RESOLUTION), 0, 1, 0.8, 0, 2/SIMULATION_GRID_RESOLUTION);
            }
            pixel_data_array[i][j].updated_this_cycle = false;
        }
    }
}

// Update the current state of the entire pixel array inputted
void master_update(simulation_pixel pixel_data_array[(int)SIMULATION_GRID_RESOLUTION][(int)SIMULATION_GRID_RESOLUTION]) {
    for (int x = 0; x < SIMULATION_GRID_RESOLUTION; x++)
    {
        for (int y = 0; y < SIMULATION_GRID_RESOLUTION; y++) {
            if (pixel_data_array[x][y].updated_this_cycle == false)
            {
                // Update Water Pixels when detected
                if (pixel_data_array[x][y].element == WATER) {
                    water_update(x, y, pixel_data_array);
                }
                
                // Update Sand Pixels when detected
                if (pixel_data_array[x][y].element == SAND) {
                    sand_update(x, y, pixel_data_array);
                }
            }
            
        }
    }
}

// Update a water particle when detected. (x, y) is location of detected water particle
void water_update(int x, int y, simulation_pixel pixel_data_array[(int)SIMULATION_GRID_RESOLUTION][(int)SIMULATION_GRID_RESOLUTION]) {
    // Case 1 : There is air under the water particle
    if (pixel_data_array[x][y-1].element == AIR) {
        // Decrement velocity as it falls
        pixel_data_array[x][y].velocity.y--;
        int fall_distance = -1;
        // Increment down to the desired position, go as far as possible
        while (fall_distance > pixel_data_array[x][y].velocity.y && pixel_data_array[x][y + (fall_distance - 1)].element == AIR)
        {
            fall_distance--;
        }
        // Switch air and water particle
        simulation_pixel temp = pixel_data_array[x][y+fall_distance];
        pixel_data_array[x][y+fall_distance] = pixel_data_array[x][y];
        pixel_data_array[x][y] = temp;
        
        // Label particle as rendered
        pixel_data_array[x][y+fall_distance].updated_this_cycle = true;
    }
    
    // Case 2 : There is NOT Air under the water particle
    if (pixel_data_array[x][y-1].element != AIR) {
        
        
        // Bottom Left and Right are both available
        if (pixel_data_array[x-1][y-1].element == AIR && pixel_data_array[x+1][y-1].element == AIR) {
            // Give the particle a velocity
            if (pixel_data_array[x][y].velocity.x == 0)
            {
                if (randomZeroOne() > 0.5) {
                    pixel_data_array[x][y].velocity.x = 1;
                    //pixel_data_array[x][y].velocity.y = 0;
                }
                else
                {
                    pixel_data_array[x][y].velocity.x = -1;
                    //pixel_data_array[x][y].velocity.y = 0;
                }
            }
            if (pixel_data_array[x][y].velocity.x > 0) {
                // Update the particle
                pixel_data_array[x][y].updated_this_cycle = true;
                
                // Switch air and water particle
                simulation_pixel temp = pixel_data_array[x+1][y-1];
                pixel_data_array[x+1][y-1] = pixel_data_array[x][y];
                pixel_data_array[x][y] = temp;
            }
            if (pixel_data_array[x][y].velocity.x < 0) {
                // Update the particle
                pixel_data_array[x][y].updated_this_cycle = true;
                
                // Switch air and water particle
                simulation_pixel temp = pixel_data_array[x-1][y-1];
                pixel_data_array[x-1][y-1] = pixel_data_array[x][y];
                pixel_data_array[x][y] = temp;
            }
            
        }
        
        if (pixel_data_array[x-1][y-1].element == AIR && pixel_data_array[x+1][y-1].element != AIR && pixel_data_array[x][y].updated_this_cycle == false) {
            // Update the particle
            pixel_data_array[x][y].updated_this_cycle = true;
            pixel_data_array[x][y].velocity.x = -1;
            
            // Switch air and water particle
            simulation_pixel temp = pixel_data_array[x-1][y-1];
            pixel_data_array[x-1][y-1] = pixel_data_array[x][y];
            pixel_data_array[x][y] = temp;
        }
        
        if (pixel_data_array[x-1][y-1].element != AIR && pixel_data_array[x+1][y-1].element == AIR && pixel_data_array[x][y].updated_this_cycle == false) {
            // Update the particle
            pixel_data_array[x][y].updated_this_cycle = true;
            pixel_data_array[x][y].velocity.x = 1;
            // Switch air and water particle
            simulation_pixel temp = pixel_data_array[x+1][y-1];
            pixel_data_array[x+1][y-1] = pixel_data_array[x][y];
            pixel_data_array[x][y] = temp;
        }
        
        
        // Case two working
//        // Give the particle a velocity
//        if (pixel_data_array[x][y].velocity.x == 0)
//        {
//            if (randomZeroOne() > 0.5) {
//                pixel_data_array[x][y].velocity.x = 1;
//                //pixel_data_array[x][y].velocity.y = 0;
//            }
//            else
//            {
//                pixel_data_array[x][y].velocity.x = -1;
//                //pixel_data_array[x][y].velocity.y = 0;
//            }
//        }
//        if (pixel_data_array[x][y].velocity.x > 0 && pixel_data_array[x+1][y-1].element == AIR) {
//            // Switch air and water particle
//            simulation_pixel temp = pixel_data_array[x+1][y-1];
//            pixel_data_array[x+1][y-1] = pixel_data_array[x][y];
//            pixel_data_array[x][y] = temp;
//
//            // Label particle as rendered
//            pixel_data_array[x+1][y-1].updated_this_cycle = true;
//        }
//        if (pixel_data_array[x][y].velocity.x < 0 && pixel_data_array[x-1][y-1].element == AIR) {
//            // Switch air and water particle
//            simulation_pixel temp = pixel_data_array[x-1][y-1];
//            pixel_data_array[x-1][y-1] = pixel_data_array[x][y];
//            pixel_data_array[x][y] = temp;
//
//            // Label particle as rendered
//            pixel_data_array[x-1][y-1].updated_this_cycle = true;
//        }
//        if (pixel_data_array[x][y].velocity.x >= 0 && pixel_data_array[x+1][y-1].element != AIR) {
//            pixel_data_array[x][y].velocity.x--;
//        }
//        if (pixel_data_array[x][y].velocity.x <= 0 && pixel_data_array[x-1][y-1].element != AIR) {
//            pixel_data_array[x][y].velocity.x++;
//        }
    }
    
    // Case 3 : There is NOT air under the bottom 3
    if (pixel_data_array[x-1][y-1].element != AIR && pixel_data_array[x][y-1].element != AIR && pixel_data_array[x+1][y-1].element != AIR) {
        
        // Attempt at horizontal acceleration
//        if (pixel_data_array[x][y].velocity.x > 0) {
//            // Increment as it spreads
//            pixel_data_array[x][y].velocity.x++;
//            int horiz_velocity_pos = 0;
//            while (horiz_velocity_pos < pixel_data_array[x][y].velocity.x && pixel_data_array[x + (horiz_velocity_pos + 1)][y].element == AIR) {
//                horiz_velocity_pos++;
//            }
//            if (horiz_velocity_pos != 0) {
//                // Switch air and water particle
//                simulation_pixel temp = pixel_data_array[x + horiz_velocity_pos][y];
//                pixel_data_array[x + horiz_velocity_pos][y] = pixel_data_array[x][y];
//                pixel_data_array[x][y] = temp;
//
//                // Label particle as rendered
//                pixel_data_array[x + horiz_velocity_pos][y].velocity.x = horiz_velocity_pos;
//                pixel_data_array[x + horiz_velocity_pos][y].updated_this_cycle = true;
//            } else {
//                pixel_data_array[x][y].velocity.x = -pixel_data_array[x][y].velocity.x;
//            }
//
//        }
//        if (pixel_data_array[x][y].velocity.x < 0) {
//            // Increment as it spreads
//            pixel_data_array[x][y].velocity.x--;
//            int horiz_velocity_pos = 0;
//            while (horiz_velocity_pos > pixel_data_array[x][y].velocity.x && pixel_data_array[x - (horiz_velocity_pos + 1)][y].element == AIR) {
//                horiz_velocity_pos--;
//            }
//            if (horiz_velocity_pos != 0) {
//                // Switch air and water particle
//                simulation_pixel temp = pixel_data_array[x + horiz_velocity_pos][y];
//                pixel_data_array[x + horiz_velocity_pos][y] = pixel_data_array[x][y];
//                pixel_data_array[x][y] = temp;
//
//                // Label particle as rendered
//                pixel_data_array[x + horiz_velocity_pos][y].velocity.x = horiz_velocity_pos;
//                pixel_data_array[x + horiz_velocity_pos][y].updated_this_cycle = true;
//            } else {
//                pixel_data_array[x][y].velocity.x = -pixel_data_array[x][y].velocity.x;
//            }
//
//        }
//        if (pixel_data_array[x][y].velocity.x == 0) {
//            if (randomZeroOne() > 0.5) {
//                pixel_data_array[x][y].velocity.x = -1;
//            }
//            else
//            {
//                pixel_data_array[x][y].velocity.x = 1;
//            }
//        }
        
        
        // OLD WORKING CODE
        if (pixel_data_array[x][y].velocity.x < 0 && pixel_data_array[x-1][y].element == AIR) {
            // Switch air and water particle
            simulation_pixel temp = pixel_data_array[x-1][y];
            pixel_data_array[x-1][y] = pixel_data_array[x][y];
            pixel_data_array[x][y] = temp;

            // Label particle as rendered
            pixel_data_array[x-1][y].updated_this_cycle = true;
        } else if (pixel_data_array[x][y].velocity.x > 0 && pixel_data_array[x+1][y].element == AIR)
        {
            // Switch air and water particle
            simulation_pixel temp = pixel_data_array[x+1][y];
            pixel_data_array[x+1][y] = pixel_data_array[x][y];
            pixel_data_array[x][y] = temp;

            // Label particle as rendered
            pixel_data_array[x+1][y].updated_this_cycle = true;
        } else if (pixel_data_array[x-1][y].element == AIR)
        {
            // Switch air and water particle
            simulation_pixel temp = pixel_data_array[x-1][y];
            pixel_data_array[x-1][y] = pixel_data_array[x][y];
            pixel_data_array[x][y] = temp;

            // Label particle as rendered
            pixel_data_array[x-1][y].updated_this_cycle = true;
        } else if (pixel_data_array[x+1][y].element == AIR)
        {
            // Switch air and water particle
            simulation_pixel temp = pixel_data_array[x+1][y];
            pixel_data_array[x+1][y] = pixel_data_array[x][y];
            pixel_data_array[x][y] = temp;

            // Label particle as rendered
            pixel_data_array[x+1][y].updated_this_cycle = true;
        }
        
        
    }
    
}

// Update a water particle when detected. (x, y) is location of detected water particle
void sand_update(int x, int y, simulation_pixel pixel_data_array[(int)SIMULATION_GRID_RESOLUTION][(int)SIMULATION_GRID_RESOLUTION]) {
    // Case 1 : There is air under the water particle
    if (pixel_data_array[x][y-1].element == AIR) {
        // Decrement velocity as it falls
        pixel_data_array[x][y].velocity.y--;
        int fall_distance = -1;
        // Increment down to the desired position, go as far as possible
        while (fall_distance > pixel_data_array[x][y].velocity.y && pixel_data_array[x][y + (fall_distance - 1)].element == AIR)
        {
            fall_distance--;
        }
        // Switch air and water particle
        simulation_pixel temp = pixel_data_array[x][y+fall_distance];
        pixel_data_array[x][y+fall_distance] = pixel_data_array[x][y];
        pixel_data_array[x][y] = temp;
        
        // Label particle as rendered
        pixel_data_array[x][y+fall_distance].updated_this_cycle = true;
    }
    
    // Case 2 : There is water under the water particle
    if (pixel_data_array[x][y-1].element == WATER) {
        // Decrement velocity as it falls
        pixel_data_array[x][y].velocity.y--;
        int fall_distance = -1;
//        // Increment down to the desired position, go as far as possible
//        while (fall_distance > pixel_data_array[x][y].velocity.y && pixel_data_array[x][y + (fall_distance - 1)].element == AIR)
//        {
//            fall_distance--;
//        }
        // Switch air and water particle
        simulation_pixel temp = pixel_data_array[x][y+fall_distance];
        pixel_data_array[x][y+fall_distance] = pixel_data_array[x][y];
        pixel_data_array[x][y] = temp;
        
        // Label particle as rendered
        pixel_data_array[x][y+fall_distance].updated_this_cycle = true;
    }
    
    // Case 3 : There is sand under sand
    if (pixel_data_array[x][y-1].element != AIR && pixel_data_array[x][y-1].element != WATER) {
        if (pixel_data_array[x][y].velocity.x == 0)
        {
            if (randomZeroOne() > 0.5) {
                pixel_data_array[x][y].velocity.x = pixel_data_array[x][y].velocity.y;
                pixel_data_array[x][y].velocity.y = 0;
            }
            else
            {
                pixel_data_array[x][y].velocity.x = -pixel_data_array[x][y].velocity.y;
                pixel_data_array[x][y].velocity.y = 0;
            }
            if (pixel_data_array[x-1][y-1].element != AIR && pixel_data_array[x-1][y-1].element != WATER) {
                pixel_data_array[x][y].velocity.x = 1;
                pixel_data_array[x][y].velocity.y = 0;
            }
            if (pixel_data_array[x+1][y-1].element != AIR && pixel_data_array[x+1][y-1].element != WATER) {
                pixel_data_array[x][y].velocity.x = -1;
                pixel_data_array[x][y].velocity.y = 0;
            }
        }
        if (pixel_data_array[x][y].velocity.x > 0 && (pixel_data_array[x+1][y-1].element == AIR || pixel_data_array[x+1][y-1].element == WATER)) {
            // Switch sand and water particle
            simulation_pixel temp = pixel_data_array[x+1][y-1];
            pixel_data_array[x+1][y-1] = pixel_data_array[x][y];
            pixel_data_array[x][y] = temp;
            
            // Label particle as rendered
            pixel_data_array[x+1][y-1].updated_this_cycle = true;
        }
        if (pixel_data_array[x][y].velocity.x < 0 && (pixel_data_array[x-1][y-1].element == AIR || pixel_data_array[x-1][y-1].element == WATER)) {
            // Switch sand and water particle
            simulation_pixel temp = pixel_data_array[x-1][y-1];
            pixel_data_array[x-1][y-1] = pixel_data_array[x][y];
            pixel_data_array[x][y] = temp;
            
            // Label particle as rendered
            pixel_data_array[x-1][y-1].updated_this_cycle = true;
        }
//        if (pixel_data_array[x][y].velocity.x >= 0 && (pixel_data_array[x+1][y-1].element != AIR && pixel_data_array[x][y-1].element != WATER)) {
//            pixel_data_array[x][y].velocity.x--;
//        }
//        if (pixel_data_array[x][y].velocity.x <= 0 && (pixel_data_array[x-1][y-1].element != AIR && pixel_data_array[x][y-1].element != WATER)) {
//            pixel_data_array[x][y].velocity.x++;
//        }
    }
}
#endif
