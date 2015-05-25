//A simple class for handling GLSL shader compilation
//Author: Movania Muhammad Mobeen
//Last Modified: February 2, 2011

#include "GLSLShader.h"
#include <iostream>


GLSLShader::GLSLShader(void)
{
	_totalShaders = 0;
	_shaders[VERTEX_SHADER] = 0;
	_shaders[FRAGMENT_SHADER] = 0;
	_shaders[GEOMETRY_SHADER] = 0;
	_attributeList.clear();
	_uniformLocationList.clear();
}

GLSLShader::~GLSLShader(void)
{
	_attributeList.clear();
	_uniformLocationList.clear();
}

void GLSLShader::LoadFromString(GLenum type, const string& source) {
	GLuint shader = glCreateShader(type);

	const char * ptmp = source.c_str();
	glShaderSource(shader, 1, &ptmp, NULL);

	//check whether the shader loads fine
	GLint status;
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar *infoLog = new GLchar[infoLogLength];
		glGetShaderInfoLog(shader, infoLogLength, NULL, infoLog);
		cerr << "Compile log: " << infoLog << endl;
		delete[] infoLog;
	}

	int index = 0;
	switch (type)
	{
	case GL_VERTEX_SHADER:
		index = VERTEX_SHADER;
		break;
	case GL_FRAGMENT_SHADER:
		index = FRAGMENT_SHADER;
		break;
	case GL_GEOMETRY_SHADER:
		index = GEOMETRY_SHADER;
		break;
	case GL_COMPUTE_SHADER:
		index = COMPUTE_SHADER;
		break;
	default:
		break;
	}
	_shaders[index] = shader;
}


void GLSLShader::CreateAndLinkProgram() {
	_program = glCreateProgram();
	if (_shaders[VERTEX_SHADER] != 0) {
		glAttachShader(_program, _shaders[VERTEX_SHADER]);
	}
	if (_shaders[FRAGMENT_SHADER] != 0) {
		glAttachShader(_program, _shaders[FRAGMENT_SHADER]);
	}
	if (_shaders[GEOMETRY_SHADER] != 0) {
		glAttachShader(_program, _shaders[GEOMETRY_SHADER]);
	}
	if (_shaders[COMPUTE_SHADER] != 0){
		glAttachShader(_program, _shaders[COMPUTE_SHADER]);
	}

	//link and check whether the program links fine
	GLint status;
	glLinkProgram(_program);
	glGetProgramiv(_program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		GLint infoLogLength;

		glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar *infoLog = new GLchar[infoLogLength];
		glGetProgramInfoLog(_program, infoLogLength, NULL, infoLog);
		cerr << "Link log: " << infoLog << endl;
		delete[] infoLog;
	}

	glDeleteShader(_shaders[VERTEX_SHADER]);
	glDeleteShader(_shaders[FRAGMENT_SHADER]);
	glDeleteShader(_shaders[GEOMETRY_SHADER]);
	glDeleteShader(_shaders[COMPUTE_SHADER]);
}

void GLSLShader::Use() {
	glUseProgram(_program);
}

void GLSLShader::UnUse() {
	glUseProgram(0);
}

void GLSLShader::AddAttribute(const string& attribute) {
	_attributeList[attribute] = glGetAttribLocation(_program, attribute.c_str());
}

//An indexer that returns the location of the attribute
GLuint GLSLShader::operator [](const string& attribute) {
	return _attributeList[attribute];
}

void GLSLShader::AddUniform(const string& uniform) {
	_uniformLocationList[uniform] = glGetUniformLocation(_program, uniform.c_str());
}

GLuint GLSLShader::operator()(const string& uniform){
	return _uniformLocationList[uniform];
}
GLuint GLSLShader::GetProgram() const {
	return _program;
}
#include <fstream>
void GLSLShader::LoadFromFile(GLenum whichShader, const string& filename){
	ifstream fp;
	fp.open(filename.c_str(), ios_base::in);
	if (fp) {
		/*string line, buffer;
		while(getline(fp, line)) {
		buffer.append(line);
		buffer.append("\r\n");
		}		*/
		string buffer(std::istreambuf_iterator<char>(fp), (std::istreambuf_iterator<char>()));
		//copy to source
		LoadFromString(whichShader, buffer);
	}
	else {
		cerr << "Error loading shader: " << filename << endl;
	}
}
