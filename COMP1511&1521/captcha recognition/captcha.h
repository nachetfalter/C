int read_pbm(char filename[], int height, int width, int pixels[height][width]);
int get_pbm_dimensions(char filename[], int *height, int *width);
void capture(int height, int width, int pixels[height][width],int start_row[4], int start_column[4], double box_height[4], double box_width[4], int box_area[4], int final_image[height][width], int* entity_amount);
void pattern_recognition(int height, int width, int pixels[height][width],int start_row[4], int start_column[4], double box_height[4], double box_width[4], int box_area[4], int entity_amount, int final_answer[4]);
