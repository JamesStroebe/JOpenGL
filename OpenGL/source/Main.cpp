#include "Render/VertexArray.h"
#include "Render/Shader.h"
#include "Render/Texture.h"

#include "Render/Renderer.h"


#include <glfw/glfw3.h>



#include <iostream>

int main() {

	if (!glfwInit())
		std::cout << "Could not Init GLFW!\n";

	GLFWwindow* window = glfwCreateWindow(1080, 720, "OpenGLTesting", nullptr, nullptr);

	glfwMakeContextCurrent(window);


	glfwSwapInterval(0);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	VertexArray va = VertexArray();
	va.Bind();

	float positions[5 * 4] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
	};

	VertexBuffer vb(positions, sizeof(positions));
	vb.Bind();

	BufferLayout layout = {
		{ShaderDataType::Float3, "a_Position"},
		{ShaderDataType::Float2, "a_TexCoords"}
		
	};
	vb.SetLayout(layout);

	unsigned int indices[6] = {
		0, 1, 2, 2, 3, 0
	};

	IndexBuffer ib(indices, sizeof(indices));
	ib.Bind();

	va.AddVertexBuffer(vb);
	va.SetIndexBuffer(ib);

	Shader shader("assets/shaders/Shader.glsl");
	shader.Bind();

	Texture2D tex("assets/textures/woodpanel.png");
	tex.Bind();

	shader.SetUniform1i("u_Texture", 0);

	Renderer::Clear();
	while (!glfwWindowShouldClose(window)) {
		Renderer::Clear(0.07f, 0.03f, 0.18f, 1.0f);

		shader.Bind();
		va.Bind();
		va.GetIndexBuffer()->Bind();
		Renderer::DrawIndexed(va.GetIndexBuffer()->GetCount());
		va.GetIndexBuffer()->Unbind();
		va.Unbind();
		shader.Unbind();
		
		
		glfwPollEvents();
		glfwSwapBuffers(window);

	}
	glfwTerminate();
}