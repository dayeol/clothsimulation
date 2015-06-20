Cloth Simulation Project Outline

�ۼ���: 2015�� 6�� 20��
����: ���±�, �̴ٿ�, ������

[����]
����Ű:		ī�޶� �̵�
���콺 �巡��:	ī�޶� ȸ��
w:		�ٶ� Toggle
r:		�ʱ� ���·� Reset
space:		�ִϸ��̼� Toggle
q:		Wireframe Toggle
o:		Cloth Pin ����
p:		Perspective/Orthogonal Projection Toggle
,:		�౸�� x�� ����
.:		�౸�� x�� ����
c:		������� ����
v:		������� ����
k:		�౸�� ����(x �� ����, Cloth�� �浹�� �̿�)
ESC:		���α׷� ����

[�ڵ�]
1. Object Files
- ObjectLoader.h/ObjectLoader.c:������Ʈ �δ�(obj -> vector list)
- Object.h/Object.cpp:		������Ʈ Class
- Cloth.h/Cloth.cpp:		Cloth Object
- Floor.h/Floor.cpp:		Floor Object
- Bar.h/Bar.cpp:		Wooden Stick Object
- Skybox.h/Skybox.cpp:		Skybox Object
- Sphere.h/Sphere.cpp:		Sphere Object

2. Shader
- GLSLShader.h/GLSLShader.cpp:		Shader ó�� Class
- compute.glsl:				compute shader(��ƼŬ ���� ó��)
- shadowFrag.glsl/shadowVert.glsl:	shadow shader
- objectFrag.glsl/objectVert.glsl:	object shader(����, �౸��, ���� ó��)
- skyFrag.glsl/skyVert.glsl:		skybox shader
- vertex.glsl/fragment.glsl:		particle shader

3. Resources
- ball.jpg:				�౸�� Color Texture
- ball_normal.png:			�౸�� Normal Map Texture
- skybox/*.jpg:				Skybox Color Texture
- grass.jpg:				���� Color Texture
- wood.jpg:				�������� Color Texture
- towel_color.jpg:			�ɷ� Color Texture
- towel_normal.jpg:			�ɷ� Normal Map Texture
- shadow.png:				�׸��� Color Texture
- TexSphere.obj:			�� Obj File

4. ��Ÿ
- main.cpp:				main loop
- Control.h/Control.cpp:		Ű����, ���콺 Input Controller
- Camera.h/Camera.cpp:			Camera ó��
- SOIL:					Texture Loading ó��
- Angel.h/mat.h/vec.h/CheckError.h:	Vector, Matrix ó��
- MatrixStack.h:			ModelView Matrix Stack ó��