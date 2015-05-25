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
GLuint program;
GLuint selectProgram;

int selectedPlanet;
int orbitToggle; // orbit 보여주기 유무
int enableCloud;
float earthTimer;

MatrixStack mvstack;
mat4 model_view, projection;
mat4 look_at;

GLuint ModelView, Projection, LookAtMat;
GLuint sModelView, sProjection, sLookAtMat;
GLuint sCode;
GLuint textureLocation, textureLocationNight, textureNormalMap, textureSpec, textureCloud, EarthTime, EnableCloud;
GLuint AmbientProduct, DiffuseProduct, SpecularProduct, LightPosition, Shininess;
GLuint IsShading;
GLuint IsEarth;
GLuint UseTexture;
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

void drawScene(bool isSelect)
{
	projection = camera.projection();
	look_at = camera.lookAt();

	if (isSelect)
	{
		glUseProgram(selectProgram);
		glUniformMatrix4fv(sProjection, 1, GL_TRUE, projection);
		glUniformMatrix4fv(sLookAtMat, 1, GL_TRUE, look_at);
	}
	else
	{
		glUseProgram(program);
		glUniformMatrix4fv(Projection, 1, GL_TRUE, projection);
		glUniformMatrix4fv(LookAtMat, 1, GL_TRUE, look_at);
		earthTimer += 0.0001;
		glUniform2f(EarthTime, earthTimer, 0);


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


		glUniform4fv(AmbientProduct, 1, ambient_product);
		glUniform4fv(DiffuseProduct, 1, diffuse_product);
		glUniform4fv(SpecularProduct, 1, specular_product);
		glUniform4fv(LightPosition, 1, light_position);
		glUniform1f(Shininess, material_shininess);
	}

	//cout << "Ambient Product: " << ambient_product << endl;
	//cout << "Diffuse Product: " << diffuse_product << endl;
	//cout << "Specular Product: " << specular_product << endl;

	sceneObject->traverse(isSelect);
	//sceneObject->draw(isSelect);
}
int processSelection(int xx, int yy) 
{

	unsigned char res[4];
	GLint viewport[4];

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawScene(true);

	glGetIntegerv(GL_VIEWPORT, viewport);
	glReadPixels(xx, viewport[3] - yy, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &res);

	printf("selected:%d\n", res[0]);

	return res[0];
}
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawScene(false);

	glutSwapBuffers();
}

void init()
{
	orbitToggle = 1;
	enableCloud = 1;
	earthTimer = 0;

	srand(time(NULL));
	
	//바닥 격자점 만들기
	sceneObject = new Floor(15, 15);

	//GPU로 보낼 데이터 버퍼 세팅
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	unsigned int bufferIndex = 0;
	unsigned int bufferSize = sizeof(vec4) * (vertices.size() + colors.size());
	cout << sizeof(vec4) * vertices.size() << endl << sizeof(vec4) * colors.size() << endl;
	
	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, bufferSize, NULL, GL_DYNAMIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, bufferIndex, sizeof(vec4) * vertices.size(), &vertices[0]);
	bufferIndex += sizeof(vec4) * vertices.size();

	glBufferSubData(GL_ARRAY_BUFFER, bufferIndex, sizeof(vec4) * colors.size(), &colors[0]);
	bufferIndex += sizeof(vec4) * colors.size();

	cout << "enter buffer data has done!" << endl;
	
	program = InitShader("vertex.glsl", "fragment.glsl");
	selectProgram = InitShader("vertex_selected.glsl", "select.glsl");

	/* Shader에 들어가는 Input 값들임 */
	bufferIndex=0;
	//Vertex Position (vPosition)
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	GLuint vsPosition = glGetAttribLocation(selectProgram, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(bufferIndex));
	bufferIndex += sizeof(vec4) * vertices.size();

	//Vertex Color (vColor)
	GLuint vColor = glGetAttribLocation( program, "vColor" );
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(bufferIndex));
	bufferIndex += sizeof(vec3) * normals.size();

	AmbientProduct = glGetUniformLocation(program, "AmbientProduct");
	printf("AmbientProduct: %d\n", AmbientProduct);
	DiffuseProduct = glGetUniformLocation(program, "DiffuseProduct");
	SpecularProduct = glGetUniformLocation(program, "SpecularProduct");
	LightPosition = glGetUniformLocation(program, "LightPosition");
	Shininess = glGetUniformLocation(program, "Shininess");
	IsShading = glGetUniformLocation(program, "IsShading");
	UseTexture = glGetUniformLocation(program, "UseTexture");
	IsEarth = glGetUniformLocation(program, "IsEarth");
	EnableCloud = glGetUniformLocation(program, "EnableCloud");
	ModelView = glGetUniformLocation(program, "ModelView");
	Projection = glGetUniformLocation(program, "Projection");
	LookAtMat = glGetUniformLocation(program, "LookAt");

	sModelView = glGetUniformLocation(selectProgram, "ModelView");
	printf("sModelView: %d\n", sModelView);
	sProjection = glGetUniformLocation(selectProgram, "Projection");
	printf("sProjection: %d\n", sProjection);
	sLookAtMat = glGetUniformLocation(selectProgram, "LookAt");
	sCode = glGetUniformLocation(selectProgram, "code");

	textureLocation = glGetUniformLocation(program, "TextureColor");
	textureLocationNight = glGetUniformLocation(program, "TextureColorNight");
	textureNormalMap = glGetUniformLocation(program, "TextureNormalMap");
	textureSpec = glGetUniformLocation(program, "TextureSpec");
	textureCloud = glGetUniformLocation(program, "TextureCloud");
	EarthTime = glGetUniformLocation(program, "EarthTime");

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	selectedPlanet = 0;
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
	if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN)
	{
		selectedPlanet = processSelection(x, y);
	}
	if (selectedPlanet == 0)
	{
		controller.mouse(button, action, x, y);
	}
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
	glutInitWindowSize( 500, 500 );
	glutInitWindowPosition( 0, 0 );
	controller.setWindowID(glutCreateWindow("BH Bros' Assignment1"));
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
