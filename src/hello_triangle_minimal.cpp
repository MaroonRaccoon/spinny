
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <SDL.h>
#include <SDL_opengles2.h>
#endif

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

std::string readFile(std::string path)
{
    std::fstream in(path, std::ios::in);
    if (in.fail()) throw std::runtime_error("File not found: " + path);
    std::stringstream buf;
    buf << in.rdbuf();
    return buf.str();
}

GLuint initShader()
{
    // read in shader code
    std::string vertexSourceString = readFile("/res/shaders/hello_triangle.vert");
    std::string fragmentSourceString = readFile("/res/shaders/hello_triangle.frag");

    GLchar *vertexSource = vertexSourceString.data();
    GLchar *fragmentSource = fragmentSourceString.data();

    // Create and compile vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    GLint vertCompileSuccess = GL_FALSE;
    int vertLogLength;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertCompileSuccess);
    glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &vertLogLength);
    if (vertLogLength > 0) {
        std::vector<char> errorMessage(vertLogLength + 1);
        glGetShaderInfoLog(vertexShader, vertLogLength, NULL, errorMessage.data());
        std::cerr << "Error message from vertex shader compilation:" << std::endl;
        std::cerr << std::string(errorMessage.data()) << std::endl;
    }

    // Create and compile fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    // Link vertex and fragment shader into shader program and use it
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    return shaderProgram;
}

void initGeometry(GLuint shaderProgram)
{
    // Create vertex buffer object and copy vertex data into it
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    GLfloat vertices[] = 
    {
        0.0f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Specify the layout of the shader vertex data (positions only, 3 floats)
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

void mainLoop(void* mainLoopArg) 
{
    SDL_Window* pWindow = (SDL_Window*)mainLoopArg;

    // Clear screen
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw the vertex buffer
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // Swap front/back framebuffers
    SDL_GL_SwapWindow(pWindow);
}

int main(int argc, char** argv)
{
    int winWidth = 512, winHeight = 512;

    // Create SDL window
    SDL_Window *pWindow = 
        SDL_CreateWindow("Hello Triangle Minimal", 
                         SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                         winWidth, winHeight, 
                         SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);

    // Create OpenGLES 2 context on SDL window
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetSwapInterval(1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GLContext glc = SDL_GL_CreateContext(pWindow);
    std::cout << "INFO: GL version: " << glGetString(GL_VERSION) << std::endl;

    // Set clear color to black
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    // Get actual GL window size in pixels, in case of high dpi scaling
    SDL_GL_GetDrawableSize(pWindow, &winWidth, &winHeight);
    std::cout << "INFO: GL window size = " << winWidth << "x" << winHeight << std::endl;
    glViewport(0, 0, winWidth, winHeight);   

    // Initialize shader and geometry
    GLuint shaderProgram = initShader();
    initGeometry(shaderProgram);

    // Start the main loop
    void* mainLoopArg = pWindow;

#ifdef __EMSCRIPTEN__
    int fps = 0; // Use browser's requestAnimationFrame
    emscripten_set_main_loop_arg(mainLoop, mainLoopArg, fps, true);
#else
    while(true) 
        mainLoop(mainLoopArg);
#endif

    return 0;
}
