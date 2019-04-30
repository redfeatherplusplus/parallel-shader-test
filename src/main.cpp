#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <thread>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <GLFW/glfw3.h>

// The functions below are fluff, skip to main. 
// Important methods are lambdas in main.

GLFWwindow* window;

const GLchar *readFile( const char *fname )
{
	FILE *f = fopen( fname, "rb" ) ;

	if( !f )
	{
		throw std::runtime_error(std::string("Could not open shader file! ") + std::string(fname)) ;
	}

	fseek( f, 0, SEEK_END ) ;
	long fsize = ftell( f ) ;
	fseek( f, 0, SEEK_SET ) ;

	GLchar *data = new GLchar[fsize + 1] ;
	fread( data, 1, fsize, f ) ;
	data[ fsize ] = 0 ;
	fclose( f ) ;

	return const_cast<const GLchar*>(data) ;
}

void init_window()
{
	glfwInit();
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 ) ;
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 6 ) ;
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE ) ;
	glfwWindowHint( GLFW_RESIZABLE, GL_FALSE ) ;

	window = glfwCreateWindow( 800, 600, "Window", nullptr, nullptr ) ;
	glfwMakeContextCurrent( window ) ;
	glewInit();

	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
	if (GLEW_KHR_parallel_shader_compile)
		std::cout << "KHR_parallel_shader_compile enabled!" << std::endl;
}

int main(int argc, char *argv[]) 
{
	init_window();

	// References:
	// https://www.khronos.org/registry/OpenGL/extensions/KHR/KHR_parallel_shader_compile.txt 
	// https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glGetProgram.xhtml
	// https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glGetShader.xhtml
	
	// Testing parallel shader compilation.

	// You can add a comment to any of the shaders to force them to recompile,
	// This needs to be done between runs to invalidate the shader cache.
	using shader_list_t = std::vector<std::pair<const char*, GLuint>>;
	shader_list_t tri_shader_list;
	shader_list_t tri2_shader_list;
	tri_shader_list.push_back({"../src/shaders/tri-vert.glsl", GL_VERTEX_SHADER});
	tri_shader_list.push_back({"../src/shaders/tri-frag.glsl", GL_FRAGMENT_SHADER}); // tri-frag-big.glsl
	tri2_shader_list.push_back({"../src/shaders/tri2-vert.glsl", GL_VERTEX_SHADER});
	tri2_shader_list.push_back({"../src/shaders/tri2-frag.glsl", GL_FRAGMENT_SHADER}); // tri2-frag-big.glsl

	// The default of -1 uses max threads.
	// From testing in other programs this does indeed spawn multiple threads.
	// However, if this is not set to -1 no parallel compilation occurs.
	glMaxShaderCompilerThreadsKHR(-1);

    auto load_shaders = [](shader_list_t shader_list, GLuint program)
    {
		std::vector<const GLchar *> sources;
		std::vector<GLuint> shaders;
        for( auto shader : shader_list )
        {
            const GLchar* source = readFile( shader.first ) ;
			sources.push_back(source);

            GLuint s = glCreateShader( shader.second ) ;
            glShaderSource( s, 1, &source, NULL ) ;
            glCompileShader( s ) ;
			shaders.push_back(s);

			// This should not always return true immediately, 
			// it is waiting for the shader to finish
			// compilation and then always returns true.
            GLint done_compiling;
            glGetShaderiv( s, GL_COMPLETION_STATUS_KHR, &done_compiling );
            if (done_compiling)
                std::cout << "Error: Shader compilation finished immediately!" << std::endl;

        }

		for(int i=0; i<shaders.size(); i++)
		{
			GLint compiled;
			glGetShaderiv( shaders[i], GL_COMPILE_STATUS, &compiled );
			if (compiled)
				std::cout << "Shader is compiled!" << std::endl;

			glAttachShader( program, shaders[i] ) ;
			delete[] sources[i] ;
		}
    };

	GLuint tri_program = glCreateProgram() ;
	GLuint tri2_program = glCreateProgram() ;
	load_shaders(tri2_shader_list, tri2_program);
	load_shaders(tri_shader_list, tri_program);

	// Testing parallel program linking.
	// This appears to work correctly

	using program_list_t = std::vector<GLuint>;
	program_list_t tri_programs;
	tri_programs.push_back(tri_program);
	tri_programs.push_back(tri2_program);

	auto link_programs = [](program_list_t programs)
	{
		for (auto program : programs)
		{
        	glLinkProgram( program ) ;
		}

		for (auto program : programs)
		{
			GLint done_linking;
			do
			{
				glGetProgramiv( program, GL_COMPLETION_STATUS_KHR, &done_linking );
				if (!done_linking)
					std::cout << "Program still linking!" << std::endl;

				using namespace std::chrono_literals;
				std::this_thread::sleep_for(1s);
			} while (!done_linking);
			
			GLint linked;
			glGetProgramiv( program, GL_LINK_STATUS, &linked );
			if (linked)
				std::cout << "Program is linked!" << std::endl;
		}
	};

	link_programs(tri_programs);

	return 0;
}