#include <stdio.h>

#include <GLFW/glfw3.h>


int main(void)
{
    if (!glfwInit()) {
        return 1;
    }

    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

    GLFWwindow *window = glfwCreateWindow(500, 500, "", NULL, NULL);
    if (window == NULL) {
        return 1;
    }

    printf("%s\n", glGetString(GL_VERSION));

    glfwDestroyWindow(window);
    glfwTerminate();
}
