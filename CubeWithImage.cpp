#include <iostream>
#include <SFML/Graphics.hpp>

const int WIDTH = 800;
const int HEIGHT = 800;
const double PI = 3.14159265;

int n = 200;

std::vector<float> transform3dto2d(float x, float y, float z) {
    float xs = 0;
    float ys = 0;

    if (z != 0) {
        ys = (n * y) / z;
        xs = (n * x) / z;
    }

    std::vector<float> output = { xs, ys };

    return output;

}

std::vector<float> matrixMultiply(std::vector<std::vector<float>> mat1, std::vector<float> mat2) {
    std::vector<float> output = { 0.0, 0.0, 0.0 };
    for (int row = 0; row < mat1.size(); row++) {
        for (int num = 0; num < mat2.size(); num++) {
            output[num] += mat1[num][row] * mat2[row];
        }
    }

    return output;
}


std::vector<std::vector<float>> rotMatrix(double angle, int axis) {

    float angleRad = angle * (PI / 180);

    std::vector<std::vector<float>> matrix = { {0, 0, 0}, {0, 0, 0}, {0, 0, 0} };
    if (axis == 0) {
        matrix = { {1, 0, 0}, {0, cos(angleRad), -sin(angleRad)}, {0, sin(angleRad), cos(angleRad)} };
    }
    if (axis == 1) {
        matrix = { {cos(angleRad), 0, sin(angleRad)}, {0, 1, 0}, {-sin(angleRad), 0, cos(angleRad)} };
    }
    if (axis == 2) {
        matrix = { {cos(angleRad), -sin(angleRad), 0}, {sin(angleRad), cos(angleRad), 0}, {0, 0, 1} };
    }
    return matrix;
}

std::vector<std::vector<std::vector< float >>> cube = { 
    {{0.5, 0.5, -0.5}, {0.5, -0.5, -0.5}, {-0.5, -0.5, -0.5}, {-0.5, 0.5, -0.5}},     //Front face (one facing you)
    {{ 0.5, 0.5, 0.5 }, {0.5, -0.5, 0.5}, {-0.5, -0.5, 0.5}, {-0.5, 0.5, 0.5} },    //Back face (Facing away)
    {{0.5, 0.5, 0.5}, {0.5, -0.5, 0.5}, {0.5, -0.5, -0.5}, {0.5, 0.5, -0.5}}, //Right face
    {{-0.5, 0.5, 0.5}, {-0.5, -0.5, 0.5}, {-0.5, -0.5, -0.5}, {-0.5, 0.5, -0.5}},       //Left face
    {{0.5, 0.5, 0.5}, {-0.5, 0.5, 0.5}, {-0.5, 0.5, -0.5}, {0.5, 0.5, -0.5}},   //Top face
    {{0.5, -0.5, 0.5}, {-0.5, -0.5, 0.5}, {-0.5, -0.5, -0.5}, {0.5, -0.5, -0.5}},   //Bottom face
    

};



float topFPS;
int main()
{
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "3D Graphical cube with image manipulation");
    sf::VertexArray square(sf::Quads, 4);

    window.setFramerateLimit(600);
        
    float xs;
    float ys;

    sf::Clock clock;
    float FPS = 0;
    
    float dt = 0;

    while (window.isOpen())
    {
        

        if (topFPS < FPS) {
            topFPS = FPS;
        }
        clock.restart().asSeconds();
        
        std::cout << "FPS: " << FPS << std::endl;

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(sf::Color(100, 100, 100));

        for (int face = 0; face < cube.size(); face++) {
            for (int point = 0; point < cube[face].size(); point++) {
                //std::cout << point << std::endl;
                cube[face][point] = matrixMultiply(rotMatrix(10*dt, 0), cube[face][point]);
                cube[face][point] = matrixMultiply(rotMatrix(25*dt, 1), cube[face][point]);
                cube[face][point] = matrixMultiply(rotMatrix(40*dt, 2), cube[face][point]);

                xs = transform3dto2d(cube[face][point][0], cube[face][point][1], cube[face][point][2] + 2)[0];
                ys = transform3dto2d(cube[face][point][0], cube[face][point][1], cube[face][point][2] + 2)[1];
                //std::cout << (int)(cube[face][point][2]);
                square[point].color = sf::Color(cube[face][point][0] * 50, cube[face][point][1] * 50, cube[face][point][2] * 50);
                
                square[point].position = sf::Vector2f(xs + 300, ys + 300);
                
            }
            window.draw(square);
        }

        
        //window.draw(square);
        window.display();
        
        dt = clock.getElapsedTime().asSeconds();
        FPS = 1 / dt;
    }
    std::cout << topFPS;
    return 0;
}