#include "graphics.hpp"
#include "window.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

std::string readFile( std::string path )
{
    std::fstream in( path, std::ios::in );
    if ( in.fail() )
        throw std::runtime_error( "File not found: " + path );
    std::stringstream buf;
    buf << in.rdbuf();
    return buf.str();
}

GLuint gfx::initShader()
{
    // read in shader code
    std::string vertexSourceString   = readFile( "/res/shaders/hello_triangle.vert" );
    std::string fragmentSourceString = readFile( "/res/shaders/hello_triangle.frag" );

    GLchar *vertexSource   = vertexSourceString.data();
    GLchar *fragmentSource = fragmentSourceString.data();

    // Create and compile vertex shader
    GLuint vertexShader = glCreateShader( GL_VERTEX_SHADER );
    glShaderSource( vertexShader, 1, &vertexSource, NULL );
    glCompileShader( vertexShader );

    GLint vertCompileSuccess = GL_FALSE;
    int   vertLogLength;
    glGetShaderiv( vertexShader, GL_COMPILE_STATUS, &vertCompileSuccess );
    glGetShaderiv( vertexShader, GL_INFO_LOG_LENGTH, &vertLogLength );
    if ( vertLogLength > 0 ) {
        std::vector< char > errorMessage( vertLogLength + 1 );
        glGetShaderInfoLog( vertexShader, vertLogLength, NULL, errorMessage.data() );
        std::cerr << "Error message from vertex shader compilation:" << std::endl;
        std::cerr << std::string( errorMessage.data() ) << std::endl;
    }

    // Create and compile fragment shader
    GLuint fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
    glShaderSource( fragmentShader, 1, &fragmentSource, NULL );
    glCompileShader( fragmentShader );

    // Link vertex and fragment shader into shader program and use it
    GLuint shaderProgram = glCreateProgram();
    glAttachShader( shaderProgram, vertexShader );
    glAttachShader( shaderProgram, fragmentShader );
    glLinkProgram( shaderProgram );
    glUseProgram( shaderProgram );

    return shaderProgram;
}

void gfx::initGeometry( GLuint shaderProgram )
{
    // Create vertex buffer object and copy vertex data into it
    GLuint vbo;
    glGenBuffers( 1, &vbo );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    GLfloat vertices [] = { 0.0f, 0.5f, 0.0f, -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f };
    glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );

    // Specify the layout of the shader vertex data (positions only, 3 floats)
    GLint posAttrib = glGetAttribLocation( shaderProgram, "position" );
    glEnableVertexAttribArray( posAttrib );
    glVertexAttribPointer( posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0 );
}

void gfx::render( gfx::Window *window )
{
    glClear( GL_COLOR_BUFFER_BIT );
    glDrawArrays( GL_TRIANGLES, 0, 3 );
    window->swap();
}
