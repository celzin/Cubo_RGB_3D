#include<Windows.h>
#include<GLFW/glfw3.h>
#include<gl/GLU.h>
#include<iostream>
#include<vector>
#include<cstdlib>
#include<cmath>

static float angulo = 5.0;
static float lastMousePos = 0.0;
static bool firstTimeMouse = true;

// Cg
struct vec3 {
	float x, y, z;
	vec3() :x(0.0), y(0.0), z(0.0) {}
	vec3(float x_, float y_, float z_) :x(x_), y(y_), z(z_) {}
	vec3 operator+(const vec3& v)const { return vec3(x + v.x, y + v.y, z + v.z); }
	vec3 operator*(float k) { return vec3(x * k, y * k, z * k); }
	float mag() { return sqrt(x * x + y * y + z * z); }
	void normaliza() {
		float m = mag();
		x = x / m; y = y / m; z = z / m;
	}
	vec3 prodVetorial(vec3 v) {
		return vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
};
//...Fim

// Passando parametros para os vertices do cubo
void rect(vec3 p1, vec3 p2, vec3 p3, vec3 p4)
{
	glBegin(GL_QUADS);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3fv(&p1.x);
	glColor3f(0.0, 1.0, 0.0);
	glVertex3fv(&p2.x);
	glColor3f(0.0, 0.0, 1.0);
	glVertex3fv(&p3.x);
	glColor3f(1.0, 1.0, 0.0);
	glVertex3fv(&p4.x);
	glEnd();
}

void desenhaCubo(GLuint id, float s)
{
	float d = s / 2.0;

	vec3 v1(-d, d, d);
	vec3 v2(-d, -d, d);
	vec3 v3(d, -d, d);
	vec3 v4(d, d, d);

	vec3 v5(d, d, -d);
	vec3 v6(d, -d, -d);
	vec3 v7(-d, -d, -d);
	vec3 v8(-d, d, -d);

	glNewList(id, GL_COMPILE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// frente
	rect(v1, v2, v3, v4);

	// direita
	rect(v4, v3, v6, v5);

	// tras
	rect(v5, v8, v7, v6);

	// esquerda
	rect(v1, v8, v7, v2);

	// topo
	rect(v1, v4, v5, v8);

	// baixo
	rect(v2, v7, v6, v3);

	glEndList();
}
//...Fim

//...INICIO CAMERA...

class Camera
{
private:
	vec3 m_pos;
	vec3 m_dir;
	vec3 m_left;
	vec3 m_up;
	vec3 m_veloc;
	float m_scl = 0.25;
	float m_yaw;
	float m_PI = 3.14159265;

public:
	Camera(vec3 pos);
	~Camera();
	void ativar();

	void forward();
	void back();
	void left();
	void right();
	void updateYaw(float dYaw);
	void update();

private:
	float cvtToRad(float ang);
};

Camera::Camera(vec3 pos)
{
	m_pos = vec3(pos);
	m_dir = vec3(0, 0, -1);
	m_left = vec3(-1, 0, 0);
	m_up = vec3(0, 1, 0);
	m_veloc = vec3(0, 0, 0);
}

Camera::~Camera()
{
}

void Camera::ativar()
{
	vec3 look = m_pos + m_dir;
	gluLookAt(m_pos.x, m_pos.y, m_pos.z, look.x, look.y, look.z, m_up.x, m_up.y, m_up.z);
}

void Camera::forward()
{
	m_veloc = m_dir * m_scl;
	m_pos = m_pos + m_veloc;
}

void Camera::back()
{
	m_veloc = m_dir * (-m_scl);
	m_pos = m_pos + m_veloc;
}

void Camera::left()
{
	m_veloc = m_left * m_scl;
	m_pos = m_pos + m_veloc;
}

void Camera::right()
{
	m_veloc = m_left * (-m_scl);
	m_pos = m_pos + m_veloc;
}

void Camera::updateYaw(float dYaw)
{
	m_yaw += dYaw;
}

void Camera::update()
{
	float ang = cvtToRad(m_yaw);
	m_dir.x = sin(ang);
	m_dir.z = -cos(ang);
	m_dir.normaliza();
	m_left = m_up.prodVetorial(m_dir);
}

float Camera::cvtToRad(float ang)
{
	return (ang * m_PI) / 180.0;
}

Camera camera(vec3(-10, 0, 0));

//...FIM CAMERA...

// Display List
unsigned int esferaID[3];
unsigned int cuboID[3];
//...Fim

void redimensiona(int w, int h)
{
	glViewport(0, 0, w, h);

	float aspect = (float)w / (float)h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, aspect, 0.1, 500.0);

	glMatrixMode(GL_MODELVIEW);
}

void desenha()
{
	glLoadIdentity();

	// Cria a matriz
	camera.ativar();

	// Passando parametros do Cubo
	glPushMatrix();
	glTranslatef(-0.0, 0.0, -70.0);
	glRotatef(angulo, 1.0, 1.0, 0.0);
	glCallList(cuboID[0]);
	glPopMatrix();
	glFlush();

	angulo += 1.0;
	if (angulo >= 360.0)
		angulo = 0.0;
}

// Declarando a funcao que interpreta os comando do teclado
void teclado_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	else if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		// Forward
		camera.forward();
	}
	else if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		// Back
		camera.back();
	}
	else if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		// Left
		camera.left();
	}
	else if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		// Right
		camera.right();
	}
}

// Declarando a funcao que interpreta os comandos do mouse
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	float dx;

	if (firstTimeMouse)
	{
		dx = 0;
		lastMousePos = xpos;
		firstTimeMouse = false;
	}
	dx = xpos - lastMousePos;
	lastMousePos = xpos;
	camera.updateYaw(dx);
	camera.update();
}

void init(GLFWwindow* window)
{
	// Declarando a callback do mouse
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Declarando a callback do teclado
	glfwSetKeyCallback(window, teclado_callback);

	//glClearColor(0.0, 0.15, 0.25, 1.0); // Cor de fundo
	glClearColor(0.0, 0.0, 0.0, 1.0);

	glEnable(GL_DEPTH_TEST);
	//glVertexPointer(3, GL_FLOAT, 0, vertices);
	//glColorPointer(3, GL_FLOAT, 0, colour);

	cuboID[0] = glGenLists(3);
	desenhaCubo(cuboID[0], 10.0);
}

int main(void)
{
	const int LARGURA = 800;
	const int ALTURA = 600;

	// Initialize the library 
	glfwInit();

	// Create a windowed mode window and its OpenGL context 
	GLFWwindow* window = glfwCreateWindow(LARGURA, ALTURA, "3D RGB CUBE", NULL, NULL);

	// Make the window's context current
	glfwMakeContextCurrent(window);

	// Init
	init(window);

	float valor = 0.0;
	float lastTime = 0.0;
	float FPS = 60.0;

	// Loop until the user closes the window
	while (!glfwWindowShouldClose(window))
	{
		float now;
		float dt;
		do {
			now = (float)glfwGetTime();
			dt = now - lastTime;

		} while (dt < 1.0 / FPS);
		lastTime = now;

		// Poll for and process events
		glfwPollEvents();

		// Render here
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		int largura, altura;
		glfwGetFramebufferSize(window, &largura, &altura);

		redimensiona(largura, altura);

		desenha();

		// Swap front and back buffers
		glfwSwapBuffers(window);
	}

	glDeleteLists(cuboID[0], 3);
	glfwTerminate();
	return 0;
}