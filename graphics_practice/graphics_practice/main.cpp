#include "main.h"
#include "Camera.h"
#include "Control.h"
#include "Floor.h"
#include "MatrixStack.h"
#include "SOIL.h"
#include <cstdlib>

Control control;
vector<vec4> vertices;
vector<vec4> colors;
vector<vec3> normals;
vector<vec2> textures;

ObjectLoader planetObjLoader;
ObjectLoader orbitObjLoader;
ObjectLoader cubeObjLoader;
GLSLShader program;

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
	program.LoadFromFile(GL_VERTEX_SHADER, "vertex.glsl");
	program.LoadFromFile(GL_FRAGMENT_SHADER, "fragment.glsl");
	program.CreateAndLinkProgram();

	srand(time(NULL));
	
	//바닥 격자점 만들기
	sceneObject = new Floor(15, 15);

	//GPU로 보낼 데이터 버퍼 세팅
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	unsigned int bufferIndex = 0;
	unsigned int bufferSize = sizeof(vec4) * (vertices.size() + colors.size());

	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, bufferSize, NULL, GL_DYNAMIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, bufferIndex, sizeof(vec4) * vertices.size(), &vertices[0]);
	bufferIndex += sizeof(vec4) * vertices.size();

	glBufferSubData(GL_ARRAY_BUFFER, bufferIndex, sizeof(vec4) * colors.size(), &colors[0]);
	bufferIndex += sizeof(vec4) * colors.size();

	/* Shader에 들어가는 Input 값들임 */
	bufferIndex=0;
	//Vertex Position (vPosition)
	program.AddAttribute("vPosition");
	glEnableVertexAttribArray(program["vPosition"]);
	glVertexAttribPointer(program["vPosition"], 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(bufferIndex));
	bufferIndex += sizeof(vec4) * vertices.size();

	//Vertex Color (vColor)
	program.AddAttribute("vColor");
	glEnableVertexAttribArray(program["vColor"]);
	glVertexAttribPointer(program["vColor"], 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(bufferIndex));

	program.AddUniform("AmbientProduct");
	program.AddUniform("DiffuseProduct");
	program.AddUniform("SpecularProduct");
	program.AddUniform("LightPosition");
	program.AddUniform("ModelView");
	program.AddUniform("Projection");
	program.AddUniform("LookAt");

	glEnable(GL_DEPTH_TEST);
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
	glutTimerFunc(1, timer, 0);
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
