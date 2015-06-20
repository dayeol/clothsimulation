#include "main.h"
#include "Camera.h"
#include "Control.h"
#include "Floor.h"
#include "Cloth.h"
#include "Sphere.h"
#include "Bar.h"
#include "Skybox.h"
#include "MatrixStack.h"
#include "SOIL.h"
#include <cstdlib>


#define C_WIDTH 1.5
#define C_HEIGHT 1.5
const float timeStep = 0.1f;

Control control;  
vector<vec4> vertices;
vector<vec4> particles;
vector<vec4> shadows;
vector<GLushort> indices;

vector<vec4> colors;
vector<vec3> normals;
vector<vec4> particleNormals;
vector<vec2> textures;
vector<vec2> particleTextures;

ObjectLoader planetObjLoader;
ObjectLoader orbitObjLoader;
ObjectLoader cubeObjLoader;
ObjectLoader sphereObjLoader;
GLSLShader program, computeShader, computeShadow, objectShader, skyShader, shadowShader;

GLuint g_verticesBuffer[3];
GLuint g_shadowBuffer;
GLuint g_normalsBuffer;
GLuint g_objectVerticesBuffer;
GLuint vertexBuffer;
GLuint vertexBufferCloth;

GLuint shadowTexture;

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

void setLight()
{
	vec4 light_position = vec4(0, 0, 5.0, 1.0);
	vec4 light_ambient(0.7, 0.7, 0.7, 1.0);
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


void drawScene()
{
	projection = camera.projection();
	look_at = camera.lookAt();
	
	if (controller.kick)
	{
		controller.sphereX += 0.1;
		if (controller.sphereX > 30)
		{
			controller.kick = false;
		}
	}

	program.Use();
	{
		glUniformMatrix4fv(program("Projection"), 1, GL_TRUE, projection);
		glUniformMatrix4fv(program("LookAtMat"), 1, GL_TRUE, look_at);
		//setLight();
	}
	program.UnUse();

	objectShader.Use();
	{
		glUniformMatrix4fv(objectShader("Projection"), 1, GL_TRUE, projection);
		glUniformMatrix4fv(objectShader("LookAtMat"), 1, GL_TRUE, look_at);
	}
	objectShader.UnUse();

	skyShader.Use();
	{
		glUniformMatrix4fv(skyShader("Projection"), 1, GL_TRUE, projection);
		glUniformMatrix4fv(skyShader("LookAtMat"), 1, GL_TRUE, look_at);
	}
	skyShader.UnUse();

	shadowShader.Use();
	{
		glUniformMatrix4fv(shadowShader("Projection"), 1, GL_TRUE, projection);
		glUniformMatrix4fv(shadowShader("LookAtMat"), 1, GL_TRUE, look_at);
	}
	shadowShader.UnUse();

	ostringstream ss;
	ss << controller.kfr;
	string currentKFR = "Frictional Coefficient: " + ss.str();
	glRasterPos2f(-0.9, 0.9);
	glColor3f(1, 0, 0);
	for (int i = 0; i < currentKFR.length(); i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, currentKFR[i]);

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
	program.AddAttribute("v_normal");
	program.AddAttribute("vTexCoord");
	program.AddUniform("ModelView");
	program.AddUniform("Projection");
	program.AddUniform("LookAt");
	program.AddUniform("TextureColor");
	program.AddUniform("TextureNormal");
	program.AddUniform("isWireframe");

	//Compute Program
	computeShader.LoadFromFile(GL_COMPUTE_SHADER, "compute.glsl");
	computeShader.CreateAndLinkProgram();
	computeShader.AddUniform("perRow");
	computeShader.AddUniform("isWind");
	computeShader.AddUniform("isPin");
	computeShader.AddUniform("dt");
	computeShader.AddUniform("structRest");//rest distance of structural spring
	computeShader.AddUniform("shearRest");//rest distance of shear spring
	computeShader.AddUniform("sphere"); // sphere matrix
	computeShader.AddUniform("sphereX"); // sphere x position
	computeShader.AddUniform("kfr"); //friction coefficient

	//Compute Shadow
	computeShadow.LoadFromFile(GL_COMPUTE_SHADER, "computeShadow.glsl");
	computeShadow.CreateAndLinkProgram();
	computeShadow.AddUniform("row");
	computeShadow.AddUniform("col");

	//Shadow Shader
	shadowShader.LoadFromFile(GL_VERTEX_SHADER, "shadowVert.glsl");
	shadowShader.LoadFromFile(GL_FRAGMENT_SHADER, "shadowFrag.glsl");
	shadowShader.CreateAndLinkProgram();
	shadowShader.AddAttribute("vPosition");
	shadowShader.AddUniform("ModelView");
	shadowShader.AddUniform("Projection");
	shadowShader.AddUniform("LookAt");
	shadowShader.AddUniform("TextureColor");

	//Object Shader Program
	objectShader.LoadFromFile(GL_VERTEX_SHADER, "objectVert.glsl");
	objectShader.LoadFromFile(GL_FRAGMENT_SHADER, "objectFrag.glsl");
	objectShader.CreateAndLinkProgram();
	objectShader.AddAttribute("vPosition");
	objectShader.AddAttribute("v_normal");
	objectShader.AddAttribute("vTexCoord");
	objectShader.AddUniform("ModelView");
	objectShader.AddUniform("Projection");
	objectShader.AddUniform("LookAt");
	objectShader.AddUniform("TextureColor");
	objectShader.AddUniform("TextureNormal");
	objectShader.AddUniform("isWireframe");
	objectShader.AddUniform("isFloor");
	objectShader.AddUniform("hasNormal");
	objectShader.AddUniform("isSpecular");

	//Skybox Shader Program
	skyShader.LoadFromFile(GL_VERTEX_SHADER, "skyVert.glsl");
	skyShader.LoadFromFile(GL_FRAGMENT_SHADER, "skyFrag.glsl");
	skyShader.CreateAndLinkProgram();
	skyShader.AddAttribute("vPosition");
	skyShader.AddUniform("Projection");
	skyShader.AddUniform("ModelView");
	skyShader.AddUniform("LookAt");
	skyShader.AddUniform("Skybox");
}
void initScene()
{
	//구 obj 로드
	sphereObjLoader.readObjFile("TexSphere.obj");
	
	//바닥 격자점 만들기
	sceneObject = new Floor(15, 15);

	//Cloth 생성
	Cloth * cloth = new Cloth(C_WIDTH, C_HEIGHT, ROWS, COLS);
	
	//구 생성
	sphereObjLoader.addVerticesList();
	sphereObjLoader.addTextureCoord();
	sphereObjLoader.addNormals();
	Sphere * sphere = new Sphere(0, sphereObjLoader.faces.size() * 3);
	Bar * bar = new Bar();
	bar->setm(Translate(vec3(-0.25, 0, 0)));
	Skybox * skybox = new Skybox();
	
	sceneObject->addChild(cloth);
	cloth->addSibling(sphere);
	sphere->addSibling(bar);
	bar->addSibling(skybox);
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
	unsigned int bufferSize = sizeof(vec2) * particleTextures.size();

	glGenBuffers(1, &vertexBufferCloth);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferCloth);
	glBufferData(GL_ARRAY_BUFFER, bufferSize, &particleTextures[0], GL_DYNAMIC_DRAW);

	program.Use();
	{
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferCloth);

		glEnableVertexAttribArray(program["vTexCoord"]);
		glVertexAttribPointer(program["vTexCoord"], 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(bufferIndex));
	}
	program.UnUse();

	bufferIndex = 0;
	bufferSize = sizeof(vec4) * vertices.size() + sizeof(vec3) * normals.size() + sizeof(vec2) * textures.size();

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, bufferSize, NULL, GL_DYNAMIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, bufferIndex, sizeof(vec4) * vertices.size(), &vertices[0]);
	bufferIndex += sizeof(vec4) * vertices.size();

	glBufferSubData(GL_ARRAY_BUFFER, bufferIndex, sizeof(vec2) * textures.size(), &textures[0]);
	bufferIndex += sizeof(vec2) * textures.size();

	glBufferSubData(GL_ARRAY_BUFFER, bufferIndex, sizeof(vec3) * normals.size(), &normals[0]);
	bufferIndex += sizeof(vec3) * normals.size();

	objectShader.Use();
	{
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		//glBufferSubData(GL_ARRAY_BUFFER, bufferIndex, sizeof(vec4) * colors.size(), &colors[0]);
		//bufferIndex += sizeof(vec4) * colors.size();
		bufferIndex = 0;

		glEnableVertexAttribArray(objectShader["vPosition"]);
		glVertexAttribPointer(objectShader["vPosition"], 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(bufferIndex));
		bufferIndex += sizeof(vec4) * vertices.size();
		
		glEnableVertexAttribArray(objectShader["vTexCoord"]);
		glVertexAttribPointer(objectShader["vTexCoord"], 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(bufferIndex));
		bufferIndex += sizeof(vec2) * textures.size();
		
		glEnableVertexAttribArray(objectShader["v_normal"]);
		glVertexAttribPointer(objectShader["v_normal"], 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(bufferIndex));
		bufferIndex += sizeof(vec3) * normals.size();
		
	}
	objectShader.UnUse();

	//skybox
	skyShader.Use();
	{
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		bufferIndex = 0;

		glEnableVertexAttribArray(skyShader["vPosition"]);
		glVertexAttribPointer(skyShader["vPosition"], 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(bufferIndex));
	}
	
	//cloth
	const int size = particles.size() * sizeof(vec4);
	glGenBuffers(3, g_verticesBuffer);
	
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, g_verticesBuffer[0]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, size, &particles[0], GL_DYNAMIC_DRAW);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, g_verticesBuffer[1]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, size, &particles[0], GL_DYNAMIC_DRAW);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, g_verticesBuffer[2]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, size, &particles[0], GL_DYNAMIC_DRAW);

	//shadow
	shadowTexture = SOIL_load_OGL_texture
	(
		"shadow.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	const int shadowSize = shadows.size() * sizeof(vec4);
	glGenBuffers(1, &g_shadowBuffer);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, g_shadowBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, shadowSize, &shadows[0], GL_DYNAMIC_DRAW);

	const int objVerticesSize = vertices.size() * sizeof(vec4);
	glGenBuffers(1, &g_objectVerticesBuffer);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, g_objectVerticesBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, objVerticesSize, &vertices[0], GL_DYNAMIC_DRAW);

	//cloth normal
	const int normalSize = particleNormals.size() * sizeof(vec4);
	glGenBuffers(1, &g_normalsBuffer);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, g_normalsBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, normalSize, &particleNormals[0], GL_DYNAMIC_DRAW);

	computeShader.Use();
	{
		glUniform1i(computeShader("perRow"), ROWS + 1);
		glUniform1f(computeShader("dt"), timeStep);
		glUniform1f(computeShader("isPin"), controller.isPin);
		glUniform1f(computeShader("structRest"), C_HEIGHT  / ROWS);
		glUniform1f(computeShader("shearRest"), sqrt(2) * (C_HEIGHT / ROWS));
		glUniformMatrix4fv(computeShader("sphere"), 1, GL_TRUE, controller.sphere);
		glUniform1f(computeShader("sphereX"), controller.sphereX);
		glUniform1f(computeShader("kfr"), controller.kfr);
	}
	computeShader.UnUse();

	computeShadow.Use();
	{
		glUniform1i(computeShader("row"), ROWS);
		glUniform1f(computeShader("col"), COLS);
	}
	computeShadow.UnUse();

	program.Use();
	{
		// Normal Input
		glBindBuffer(GL_ARRAY_BUFFER, g_normalsBuffer);
		glVertexAttribPointer(program["v_normal"], 4, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(program["v_normal"]);
	}
	program.UnUse();

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

	if (controller.sphereMovingLeft)
		controller.sphereX -= 0.01;
	else if (controller.sphereMovingRight)
		controller.sphereX += 0.01;
	
	if (controller.frictionUp)
	{
		controller.kfr += 0.01;
		if (controller.kfr > 5)
			controller.kfr = 5;
	}
	else if (controller.frictionDown)
	{
		controller.kfr -= 0.01;
		if (controller.kfr < 0)
			controller.kfr = 0;
	}

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
