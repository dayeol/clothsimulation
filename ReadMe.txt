Cloth Simulation Project Outline

작성일: 2015년 6월 20일
팀원: 김태규, 이다열, 최준혁

[조작]
뱡향키:		카메라 이동
마우스 드래그:	카메라 회전
w:		바람 Toggle
r:		초기 상태로 Reset
space:		애니메이션 Toggle
q:		Wireframe Toggle
o:		Cloth Pin 해제
p:		Perspective/Orthogonal Projection Toggle
,:		축구공 x축 감소
.:		축구공 x축 증가
c:		마찰계수 증가
v:		마찰계수 감소
k:		축구공 차기(x 축 증가, Cloth와 충돌에 이용)
ESC:		프로그램 종료

[코드]
1. Object Files
- ObjectLoader.h/ObjectLoader.c:오브젝트 로더(obj -> vector list)
- Object.h/Object.cpp:		오브젝트 Class
- Cloth.h/Cloth.cpp:		Cloth Object
- Floor.h/Floor.cpp:		Floor Object
- Bar.h/Bar.cpp:		Wooden Stick Object
- Skybox.h/Skybox.cpp:		Skybox Object
- Sphere.h/Sphere.cpp:		Sphere Object

2. Shader
- GLSLShader.h/GLSLShader.cpp:		Shader 처리 Class
- compute.glsl:				compute shader(파티클 연산 처리)
- shadowFrag.glsl/shadowVert.glsl:	shadow shader
- objectFrag.glsl/objectVert.glsl:	object shader(막대, 축구공, 지면 처리)
- skyFrag.glsl/skyVert.glsl:		skybox shader
- vertex.glsl/fragment.glsl:		particle shader

3. Resources
- ball.jpg:				축구공 Color Texture
- ball_normal.png:			축구공 Normal Map Texture
- skybox/*.jpg:				Skybox Color Texture
- grass.jpg:				지형 Color Texture
- wood.jpg:				나무막대 Color Texture
- towel_color.jpg:			걸레 Color Texture
- towel_normal.jpg:			걸레 Normal Map Texture
- shadow.png:				그림자 Color Texture
- TexSphere.obj:			구 Obj File

4. 기타
- main.cpp:				main loop
- Control.h/Control.cpp:		키보드, 마우스 Input Controller
- Camera.h/Camera.cpp:			Camera 처리
- SOIL:					Texture Loading 처리
- Angel.h/mat.h/vec.h/CheckError.h:	Vector, Matrix 처리
- MatrixStack.h:			ModelView Matrix Stack 처리