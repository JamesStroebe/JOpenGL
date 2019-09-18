#include "Render/VertexArray.h"
#include "Render/Shader.h"

#include <glfw/glfw3.h>



#include <iostream>

int main() {

	if (!glfwInit())
		std::cout << "Could not Init GLFW!\n";

	GLFWwindow* window = glfwCreateWindow(1080, 720, "OpenGLTesting", nullptr, nullptr);

	glfwMakeContextCurrent(window);


	glfwSwapInterval(0);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glClearColor(0.07, 0.03, 0.18, 1.0);

	VertexArray va = VertexArray();
	va.Bind();
	float positions[3 * 3] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	VertexBuffer vb(positions, sizeof(positions));
	vb.Bind();

	BufferLayout layout = {
		{ShaderDataType::Float3, "a_Position"},
		
	};
	vb.SetLayout(layout);

	unsigned int indices[3] = {
		0, 1, 2
	};

	IndexBuffer ib(indices, sizeof(indices));
	ib.Bind();

	va.AddVertexBuffer(vb);
	va.SetIndexBuffer(ib);

	Shader shader("assets/shaders/Shader.glsl");
	shader.Bind();

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

		vb.Bind();
		ib.Bind();
		shader.Bind();
		glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
		shader.Unbind();
		ib.Unbind();
		vb.Unbind();
		
		glfwPollEvents();
		glfwSwapBuffers(window);

	}
	glfwTerminate();
}