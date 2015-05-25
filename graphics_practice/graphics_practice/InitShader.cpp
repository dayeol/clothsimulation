
#include <fstream>
#include "Angel.h"

namespace Angel {

void printShaderInfoLog(GLuint obj) {
	int infologLength = 0;
	int charsWritten  = 0;
	char *infoLog;
	glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);
	if (infologLength > 0) {
		infoLog = (char *)malloc(infologLength);
		glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("printShaderInfoLog: %s\n",infoLog);
		free(infoLog);
	}
	else {
		printf("Shader Info Log: OK\n");
	}
}

void printProgramInfoLog(GLuint obj) {
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;
    glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);
    if (infologLength > 0) {
        infoLog = (char *)malloc(infologLength);
        glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
        printf("printProgramInfoLog: %s\n",infoLog);
        free(infoLog);
    }
    else {
        printf("Program Info Log: OK\n");
    }
}
// Create a NULL-terminated string by reading the provided file
static char*
readShaderSource(const char* fn)
{
	FILE *fp;
    char *content = NULL;

    int count=0;

    if (fn != NULL) {
        fopen_s(&fp, fn,"rt");

        if (fp != NULL) {
            fseek(fp, 0, SEEK_END);
            count = ftell(fp);
            rewind(fp);

            if (count > 0) {
                content = (char *)malloc(sizeof(char) * (count+1));
                count = fread(content,sizeof(char),count,fp);
                content[count] = '\0';
            }
            fclose(fp);
        }
    }
    return content;
}

// Create a GLSL program object from vertex and fragment shader files
GLuint
InitShader(const char* vert_file, const char* frag_file)
{
    char *vs, *fs;

	vs = readShaderSource(vert_file);
	fs = readShaderSource(frag_file);
	if (vs == NULL || fs == NULL) {
		printf("No shader files\n");
		exit(1);
	}

	GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
	GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);

	const char *vv = vs;
	const char *ff = fs;

	glShaderSource(vert_shader, 1, &vv, NULL);
	glShaderSource(frag_shader, 1, &ff, NULL);

	free(vs);
	free(fs);

	glCompileShader(vert_shader);
	glCompileShader(frag_shader);

	GLuint shader_program = glCreateProgram();
	glAttachShader(shader_program, vert_shader);
	glAttachShader(shader_program, frag_shader);
	glLinkProgram(shader_program);

	printShaderInfoLog(vert_shader);
	printShaderInfoLog(frag_shader);
	printProgramInfoLog(shader_program);
	
	std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
	std::cout << "OpenGL Version " << glGetString(GL_VERSION) << " supported" << std::endl;
	std::cout << "Shading Language Version "<< glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	
	if(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader){
		std::cout << "Ready for GLSL\n" << std::endl;
	}
	else {
		std::cout << "Not totally ready :( \n" << std::endl;
	}

	return shader_program;

}


}  // Close namespace Angel block

