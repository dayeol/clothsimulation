#include "main.h"
#include "Camera.h"
#include "Control.h"
#include "Floor.h"
#include "Cloth.h"
#include "MatrixStack.h"
#include "SOIL.h"
#include <cstdlib>

#define ROWS	30
#define COLS	30
#define C_WIDTH	1.5
#define C_HEIGHT 1.5
const float timeStep = 0.1f;

Control control;
vector<vec4> vertices;
vector<vec4> particles;

vector<vec4> colors;
vector<vec3> normals;
vector<vec2> textures;

ObjectLoader planetObjLoader;
ObjectLoader orbitObjLoader;
ObjectLoader cubeObjLoader;
GLSLShader program, computeShader;

GLuint g_verticesBuffer[3];
GLuint vertexBuffer;

MatrixStack mvstack;
mat4 model_view, projection;
mat4 look_at;

Object* sceneObject;

void reshape(int w, int h)
{
	glLoadIdentity();
	glViewport(0, 0, w, h);
	camera.setRatio(w, h);
}

void idle()
{
	glutPostRedisplay();
}

void drawScene()
{
	projection = camera.projection();
	look_at = camera.lookAt();

	program.Use();
	{
		glUniformMatrix4fv(program("Projection"), 1, GL_TRUE, projection);
		glUniformMatrix4fv(program("LookAtMat"), 1, GL_TRUE, look_at);
	}
	program.UnUse();

	sceneObject->traverse();

}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawScene();

	glutSwapBuffers();
}


void initShader()
{
	//Render Program
	program.LoadFromFile(GL_VERTEX_SHADER, "vertex.glsl");
	program.LoadFromFile(GL_FRAGMENT_SHADER, "fragment.glsl");
	program.CreateAndLinkProgram();
	program.AddAttribute("vPosition");	
	program.AddAttribute("vColor");
	program.AddUniform("AmbientProduct");
	program.AddUniform("DiffuseProduct");
	program.AddUniform("SpecularProduct");
	program.AddUniform("LightPosition");
	program.AddUniform("ModelView");
	program.AddUniform("Projection");
	program.AddUniform("LookAt");

	//Compute Program
	computeShader.LoadFromFile(GL_COMPUTE_SHADER, "compute.glsl");
	computeShader.CreateAndLinkProgram();
	computeShader.AddUniform("perRow");
	computeShader.AddUniform("dt");
	computeShader.AddUniform("structRest");//rest distance of structural spring
	computeShader.AddUniform("shearRest");//rest distance of shear spring
}
void initScene()
{
	//바닥 격자점 만들기
	sceneObject = new Floor(15, 15);

	Cloth * cloth = new Cloth(C_WIDTH, C_HEIGHT, ROWS, COLS);
	sceneObject->addChild(cloth);
}

void setGLOptions()
{
	//for 3D depth test
	glEnable(GL_DEPTH_TEST);

	//anti-aliasing
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	glPointSize(2);
}

void setLight()
{
	vec4 light_position = vec4(0, 0, 0, 1.0);
	vec4 light_ambient(0.1, 0.1, 0.1, 1.0);
	vec4 light_diffuse(0.8, 0.8, 0.8, 1.0);
	vec4 light_specular(0.35, 0.35, 0.35, 1.0);

	vec4 material_ambient(1.0, 1.0, 1.0, 1.0);
	vec4 material_diffuse(1.0, 1.0, 1.0, 1.0);
	vec4 material_specular(1.0, 1.0, 1.0, 1.0);
	float material_shininess = 10;

	vec4 ambient_product = light_ambient * material_ambient;
	vec4 diffuse_product = light_diffuse * material_diffuse;
	vec4 specular_product = light_specular * material_specular;

	program.Use();
	{
		glUniform4fv(program("AmbientProduct"), 1, ambient_product);
		glUniform4fv(program("DiffuseProduct"), 1, diffuse_product);
		glUniform4fv(program("SpecularProduct"), 1, specular_product);
		glUniform4fv(program("LightPosition"), 1, light_position);
		glUniform1f(program("Shininess"), material_shininess);
	}
	program.UnUse();


}

void init()
{
	srand(time(NULL));
	initScene();

	initShader();
	//GPU로 보낼 데이터 버퍼 세팅
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	unsigned int bufferIndex = 0;
	unsigned int bufferSize = sizeof(vec4) * (vertices.size() + colors.size());

	program.Use();
	{
		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, bufferSize, NULL, GL_DYNAMIC_DRAW);

		glBufferSubData(GL_ARRAY_BUFFER, bufferIndex, sizeof(vec4) * vertices.size(), &vertices[0]);
		bufferIndex += sizeof(vec4) * vertices.size();

		glBufferSubData(GL_ARRAY_BUFFER, bufferIndex, sizeof(vec4) * colors.size(), &colors[0]);
		bufferIndex += sizeof(vec4) * colors.size();
	}
	program.UnUse();

	/* Shader에 들어가는 Input 값들임 */
	bufferIndex=0;
	//Vertex Position
	program.Use();
	{
		glVertexAttribPointer(program["vPosition"], 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(bufferIndex));
		bufferIndex += sizeof(vec4) * vertices.size();
		glEnableVertexAttribArray(program["vPosition"]);
		//Vertex Color
		glVertexAttribPointer(program["vColor"], 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(bufferIndex));
		glEnableVertexAttribArray(program["vColor"]);

	}
	program.UnUse();

	//cloth
	const int size = particles.size() * sizeof(vec4);
	glGenBuffers(3, g_verticesBuffer);
	
	//glBindBuffer(GL_SHADER_STORAGE_BUFFER, g_verticesBuffer[0]);
	//glBufferData(GL_SHADER_STORAGE_BUFFER, size, &particles[0], GL_DYNAMIC_DRAW);
	
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, g_verticesBuffer[0]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, size, &particles[0], GL_DYNAMIC_DRAW);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, g_verticesBuffer[1]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, size, &particles[0], GL_DYNAMIC_DRAW);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, g_verticesBuffer[2]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, size, 0, GL_DYNAMIC_DRAW);
	
	computeShader.Use();
	{
		glUniform1i(computeShader("perRow"), ROWS + 1);
		glUniform1f(computeShader("dt"), timeStep);
		glUniform1f(computeShader("structRest"), C_HEIGHT  / ROWS);
		glUniform1f(computeShader("shearRest"), sqrt(2) * (C_HEIGHT / ROWS));
	}
	computeShader.UnUse();

	setGLOptions();
}

void keyDown(unsigned char key, int x, int y)
{
	controller.keyDown(key, x, y);
}

void keyUp(unsigned char key, int x, int y)
{
	controller.keyUp(key, x, y);
}

void specialKeyDown(int key, int x, int y)
{
	controller.specialKeyDown(key, x, y);
}

void specialKeyUp(int key, int x, int y)
{
	controller.specialKeyUp(key, x, y);
}
#define BUFSIZE 512
void mouse(GLint button, GLint action, GLint x, GLint y)
{
	controller.mouse(button, action, x, y);

}
void mouseMove(int x, int y)
{
	controller.mouseMove(x, y);
}
void timer(int input)
{
	//controller.launchRocketOnCountdown();

	if (controller.movingUp)
		camera.move(Camera::UP);
	else if (controller.movingDown)
		camera.move(Camera::DOWN);
	if (controller.movingLeft)
		camera.move(Camera::LEFT);
	else if (controller.movingRight)
		camera.move(Camera::RIGHT);

	if (controller.zoomingIn)
		camera.zoomIn();
	else if (controller.zoomingOut)
		camera.zoomOut();
	glutTimerFunc(10, timer, 0);
	glutPostRedisplay();
}
void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB |GLUT_DEPTH);
	glutInitWindowSize( 1024, 1024 );
	glutInitWindowPosition( 0, 0 );
	controller.setWindowID(glutCreateWindow("CLOTH SIMULATION by BHBros"));
	glutDisplayFunc( display );
	glutKeyboardFunc( keyDown );
	glutKeyboardUpFunc(keyUp);
	glutSpecialFunc( specialKeyDown );
	glutSpecialUpFunc( specialKeyUp );
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMove);
	glutReshapeFunc( reshape );
	timer(0);
	glewInit();
	init();
	glutMainLoop();

}
