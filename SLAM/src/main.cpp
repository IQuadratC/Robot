
#include <glm/glm.hpp>
#include <cmath>	

#define MAPSIZE 10
#define POINTSSIZE 10

inline int index(int x,int y, int size) { return x + (size * y);}

float mapData[MAPSIZE * MAPSIZE];

glm::vec2 points[POINTSSIZE];


float func4(glm::vec2 m){

    float x = m.x;
    float y = m.y;
    
    int x0 = (int) x;
    int y0 = (int) y;
    int x1 = x0 + 1;
    int y1 = y0 + 1;
    
    float dx0 = (x - x0) / (x1 - x0);
    float dx1 = (x1 - x) / (x1 - x0);
    float dy0 = (y - y0) / (y1 - y0);
    float dy1 = (y1 - y) / (y1 - y0);
    
    float a = dy0 * (dx0 * mapData[index(x1, y1, MAPSIZE)] + dx1 * mapData[index(x0, y1, MAPSIZE)]) +
            dy1 * (dx0 * mapData[index(x1, y0, MAPSIZE)] + dx1 * mapData[index(x0, y0, MAPSIZE)]);

    return a;
}

glm::vec2 func56(glm::vec2 m){
    
    float x = m.x;
    float y = m.y;
    
    int x0 = (int) m.x;
    int y0 = (int) m.y;
    int x1 = x0 + 1;
    int y1 = y0 + 1;
    
    float dx0 = (x - x0) / (x1 - x0);
    float dx1 = (x1 - x) / (x1 - x0);
    float dy0 = (y - y0) / (y1 - y0);
    float dy1 = (y1 - y) / (y1 - y0);

    float ax = dy0 * (mapData[index(x1, y1, MAPSIZE)] - mapData[index(x0, y1, MAPSIZE)]) + 
            dy1 * (mapData[index(x1, y0, MAPSIZE)] - mapData[index(x0, y0, MAPSIZE)]);

    float ay = dx0 * (mapData[index(x1, y1, MAPSIZE)] - mapData[index(x0, y1, MAPSIZE)]) + 
            dx1 * (mapData[index(x1, y0, MAPSIZE)] - mapData[index(x0, y0, MAPSIZE)]);

    return glm::vec2(ax, ay);
}

glm::vec2 func8(glm::vec3 E, glm::vec2 S){
    
    glm::mat2x2 matrix;
    matrix[0][0] = cos(E.z);
    matrix[0][1] = -sin(E.z);
    matrix[1][0] = sin(E.z);
    matrix[1][1] = cos(E.z);
    
    glm::vec2 temp(E.x,E.y);
    return matrix * S + temp;
}

glm::mat2x3 func14(glm::vec3 E, glm::vec2 S){
    glm::mat2x3 matrix;
    matrix[0][0] = 1;
    matrix[0][1] = 0;
    matrix[0][2] = -(sin(E.z) * S.x) - (cos(E.z) * S.y);
    matrix[1][0] = 0;
    matrix[1][1] = 1;
    matrix[1][2] = (cos(E.z) * S.x) - (sin(E.z) * S.y);

    return matrix;
}

glm::vec3 func12(glm::vec3 E){
    for (size_t i = 0; i < POINTSSIZE; i++)
    {
        glm::vec2 pos = func8(E, points[i]);

        glm::mat2x3 mat0 = func56(pos) * func14(E, points[i]);
        glm::mat3x2 mat1;
        mat1[0][0] = mat0[0][0];
        mat1[0][1] = mat0[1][0];
        mat1[1][0] = mat0[0][1];
        mat1[1][1] = mat0[1][1];
        mat1[2][0] = mat0[0][2];
        mat1[2][1] = mat0[1][2];

        glm::mat2x2 mat2 = mat1 * mat0;

        glm::mat2x2 mat3 = glm::inverse(mat2);


    }
}

int main(){

    mapData[index(0,0, MAPSIZE)] = 0;
    mapData[index(0,1, MAPSIZE)] = 1;
    mapData[index(1,0, MAPSIZE)] = 0;
    mapData[index(1,1, MAPSIZE)] = 0;

    func4(glm::vec2(0.5, 0.5));
    func56(glm::vec2(0, 0.5));

    func14(glm::vec3(0,0,0),  glm::vec2(1,1));

    func12(glm::vec3(0.2,0.7,1));

    return 0;
}