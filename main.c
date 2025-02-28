#include <GLFW/glfw3.h>
#include <GL/gl.h>

int main()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_WAYLAND);
	if (!glfwInit()) {
		return -1;
	}

	GLFWwindow *window = glfwCreateWindow(640, 480, "lorem", NULL, NULL);
	if (!window) {
		return -1;
	}

	glfwMakeContextCurrent(window);

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.5f, 0.5f, 0.5f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
