
#include <glm/glm.hpp>
#include <cmath>	
inline int index(int x,int y) { return x + (10 * y);}

float data[100];

glm::vec2 func8(glm::vec3 E, glm::vec2 S){
    
    glm::mat2x2 matrix;
    matrix[0][0] = cos(E.z);
    matrix[0][1] = -sin(E.z);
    matrix[1][0] = sin(E.z);
    matrix[1][1] = cos(E.z);
    
    glm::vec2 temp(E.x,E.y);
    return matrix * S + temp;
}

float func4(glm::vec2 m){

    float x = m.x;
    float y = m.y;
    
    int x0 = (int) m.x;
    int y0 = (int) m.y;
    int x1 = x0 + 1;
    int y1 = y0 + 1;
    
    float dx0 = (x - x0) / (x1 - x0);
    float dx2 = (x1 - x) / (x1 - x0);
    float dy1 = (y - y0) / (y1 - y0);
    float dy2 = (y1 - y) / (y1 - y0);
    
    float a = dy1 * (dx0 * data[index(x1, y1)] + dx1 * data[index(x0, y1)]) +
            dy2 * (dx0 * data[index(x0, y1)] + dx1 * data[index(x0, y0)]);

    return a;
}

glm::vec2 func56(){
    
    float x = m.x;
    float y = m.y;
    
    int x0 = (int) m.x;
    int y0 = (int) m.y;
    int x1 = x0 + 1;
    int y1 = y0 + 1;
    
    float dx0 = (x - x0) / (x1 - x0);
    float dx2 = (x1 - x) / (x1 - x0);
    float dy1 = (y - y0) / (y1 - y0);
    float dy2 = (y1 - y) / (y1 - y0);

    float ax = dx0 * (data[index(x1, y1)] - data[index(x0, y1)]) + 
            dx1 * (data[index(x1, y0)] - data[index(x0, y0)]);

    float ay = dy0 * (data[index(x1, y1)] - data[index(x0, y1)]) + 
            dy1 * (data[index(x1, y0)] - data[index(x0, y0)]);

    return glm::vec2(ax, ay);
}




int main(){

    

    glm::mat4 test1;
    glm::vec3 test2;




    return 0;
}