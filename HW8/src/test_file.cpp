#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
//#include "imgui.h"
//#include "imgui_impl_glfw_gl3.h"
#include "shader.h"
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 600

unsigned int VAO, VBO;
float PosX = 0;
float PosY = 0;
// mouse click point
vector<glm::vec2> point;
float vertices[100];
// surpose 1000 point
int curvePointCount = 1000;
float animation = 0;
int checkpoint = 0;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void cursor_pos_callback(GLFWwindow* window, double x, double y);
void click_callback(GLFWwindow* window, int button, int action, int mods);
long int jiecheng(int x);
float* Bezier(vector<glm::vec2> point);


void transform(vector<glm::vec2> vertex) {
	float transformVertex[1000];
	int n = vertex.size();
	if (n == 1) return;

	vector<glm::vec2> nextVertexs = vector<glm::vec2>();
	for (int i = 0; i < n - 1; i++) {
		float tx = (1 - animation) * vertex[i].x + animation * vertex[i + 1].x;
		float ty = (1 - animation) * vertex[i].y + animation * vertex[i + 1].y;
		glm::vec2 nextVertex = glm::vec2(tx, ty);
		transformVertex[i * 2] = tx;
		transformVertex[i * 2 + 1] = ty;
		nextVertexs.push_back(nextVertex);
	}

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(float) * nextVertexs.size(), transformVertex, GL_STATIC_DRAW);

	unsigned int VAO;
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glPointSize(10.0f);
	glDrawArrays(GL_POINTS, 0, nextVertexs.size());

	glPointSize(1.0f);
	glDrawArrays(GL_LINE_STRIP, 0, nextVertexs.size());

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	transform(nextVertexs);
}

int main() {
	glfwInit();
	// OpenGL version & mode setting
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a window & context/viewpoint setting
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "16340164-Bezier", NULL, NULL);
	if (window == NULL) {
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad init
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}

	// shader init
	Shader shader("vShaderSrc.txt", "fShaderSrc.txt");
	point = vector<glm::vec2>();
	/*
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfwGL3_Init(window, true);
	ImGui::StyleColorsLight();
	*/
	
	glfwSetCursorPosCallback(window, cursor_pos_callback);
	glfwSetMouseButtonCallback(window, click_callback);

	shader.use();
	
	// ---- render loop ----
	while (!glfwWindowShouldClose(window)) {

		processInput(window);
		glfwPollEvents();
		//ImGui_ImplGlfwGL3_NewFrame();
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//point put in vertices
		for (int i = 0; i < point.size(); i++) {
			vertices[i * 2] = point[i].x;
			vertices[i * 2 + 1] = point[i].y;
		}
		//vertics
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(float) * point.size(), vertices, GL_STATIC_DRAW);	
		//postion
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		//draw arr
		glPointSize(10.0f);
		glDrawArrays(GL_POINTS, 0, point.size());
		glDrawArrays(GL_LINE_STRIP, 0, point.size());
		
		//beizer curve
		float* curve = Bezier(point);
		//curve point
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(float) * curvePointCount, curve, GL_STATIC_DRAW);
		//postion
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// draw arr  1000  point 
		glPointSize(1.0f);
		glDrawArrays(GL_POINTS, 0, curvePointCount);
		// 动态呈现效果
		if (point.size() >= 3) {
			checkpoint++;
			transform(point);
			animation += 0.001;
			animation = animation > 1 ? 1 : animation;

		}
		
		//ImGui::Render();
		//ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
	}


	//ImGui_ImplGlfwGL3_Shutdown();
	//ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	// press ESC to close
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

void cursor_pos_callback(GLFWwindow* window, double x, double y) {
	PosX = (x - WINDOW_WIDTH / 2) / (WINDOW_WIDTH / 2);
	PosY = ((WINDOW_HEIGHT - y) - WINDOW_HEIGHT / 2) / (WINDOW_HEIGHT / 2);
}

void click_callback(GLFWwindow* window, int button, int action, int mods) {
	// 监听左键点击事件
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		checkpoint = 0;
		int flag = 0;
		for (int i = 0; i < point.size(); i++) {
			if (point[i].x == PosX && point[i].y == PosY) {
				flag = 1;
			}
		}
		if (flag == 0 && point.size() < 20) {
			point.push_back(glm::vec2(PosX, PosY));
		}
	}
	// 监听右键点击事件
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		checkpoint = 0;
		vector<glm::vec2>::iterator iter;
		for (iter = point.begin(); iter != point.end(); iter++) {
			if (pow(iter->x - PosX, 2) + pow(iter->y - PosY, 2) <= 0.1*0.1) {
				point.erase(iter);
				break;
			}
		}
	}
}

// 阶乘
long int jiecheng(int x) {
	if (x == 0) return 1;
	int result = 1;
	for (int i = 1; i <= x; i++) {
		result *= i;
	}
	return result;
}

//bezier curve 公式
float* Bezier(vector<glm::vec2> point) {
	//beizer curve 点个数
	int num = 0;
	//beizer curve 点坐标
	float* bezierCurve = new float[10000];
	for (float t = 0; t < 1; t += 0.001, num++) {
		// x,y 坐标 = 0
		bezierCurve[num * 2] = 0;
		bezierCurve[num * 2 + 1] = 0;
		for (int i = 0, n = point.size() - 1; i <= n; i++) {
			// 常数部分 C=n!/i!(n-i)!
			float C = jiecheng(n) / (jiecheng(i) * jiecheng(n - i));
			//X
			bezierCurve[num * 2] += C * point[i].x  * pow(1 - t, n - i) * pow(t, i);
			//Y
			bezierCurve[num * 2 + 1] += C * point[i].y  * pow(1 - t, n - i) * pow(t, i);
		}

	}

	return bezierCurve;
}

