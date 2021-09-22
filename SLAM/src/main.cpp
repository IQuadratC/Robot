/*
#include <glm/glm.hpp>
#include <cmath>
#include <iostream>	

#define MAPSIZE 10
#define POINTSSIZE 10

inline int index(int x,int y, int size) { return x + (size * y);}

float mapData[MAPSIZE * MAPSIZE];

glm::vec2 points[POINTSSIZE];

struct mat1x2
{ 
    float x;
    float y;

    mat1x2(float x, float y) : x(x), y(y) {};
};

struct mat1x3
{ 
    float x;
    float y;
    float z;

    mat1x3(float x, float y, float z) : x(x), y(y), z(z) {};
};

inline mat1x2 transpose(glm::vec2 a){
    return mat1x2(a.x, a.y);
}

inline glm::vec2 transpose(mat1x2 a){
    return glm::vec2(a.x, a.y);
}

inline mat1x3 transpose(glm::vec3 a){
    return mat1x3(a.x, a.y, a.z);
}

inline glm::vec3 transpose(mat1x3 a){
    return glm::vec3(a.x, a.y, a.z);
}

inline mat1x3 operator* (mat1x2 a, glm::mat2x3 b){
    return mat1x3(a.x*b[0][0]+a.y*b[1][0], a.x*b[0][1]+a.y*b[1][1], a.x*b[0][2]+a.y*b[1][2]);
}

inline float operator* (glm::vec3 a, mat1x3 b){
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

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
mat1x2 func56(glm::vec2 m){
    
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

    return mat1x2(ax, ay);
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
    glm::vec3 ans(0,0,0);
    for (size_t i = 0; i < POINTSSIZE; i++)
    {
        if (points[i] == glm::vec2(0,0)){
            continue;
        }

        glm::vec2 pos = func8(E, points[i]);

        mat1x2 mat56 = func56(pos);
        glm::mat2x3 mat14 = func14(E, points[i]);

        mat1x3 a = mat56 * mat14;
        glm::vec3 b = transpose(a);
        float h = b * a;
        float hinv = 1/h;

        float c = 1 - func4(pos);
        glm::vec3 d = b * hinv * c;
        ans += d;
    }
    return ans;
}

int main(){

    mapData[index(0,0, MAPSIZE)] = 0;
    mapData[index(0,1, MAPSIZE)] = 1;
    mapData[index(1,0, MAPSIZE)] = 1;
    mapData[index(1,1, MAPSIZE)] = 1;

    points[0] = glm::vec2(0.11, 1.1);
    points[0] = glm::vec2(1.1, 0.1);
    points[0] = glm::vec2(1.1, 1.1);

    glm::vec3 a = func12(glm::vec3(0,0,0));
    std::cout << a.x << "\n" << a.y << "\n" << a.z << "\n";

    return 0;
}

*/