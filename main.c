#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Vec3
{
    float x, y, z;
} Vec3;

const Vec3 vertices[] = {
    {.x = -0.5f, .y = -0.5f, .z = 0.0f},
    {.x = 0.5f, .y = -0.5f, .z = 0.0f},
    {.x = 0.0f, .y = 0.5f, .z = 0.0f},
};

GLuint triangleVertexBuffer;

char *readFile(char *filename)
{
    FILE *f = fopen(filename, "rt");
    fseek(f, 0, SEEK_END);
    long length = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *buffer = (char *)malloc(length + 1);
    buffer[length] = '\0';
    fread(buffer, 1, length, f);
    fclose(f);
    return buffer;
}

GLuint loadAndCompileShader(char *filePath, GLenum shaderType)
{
    char *shaderSrc = readFile(filePath);
    const char *cShaderSrc = shaderSrc;
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &cShaderSrc, NULL);
    glCompileShader(shader);
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        printf("%d %s", shaderType, infoLog);
        exit(-1);
    }
    free(shaderSrc);

    return shader;
}

int main()
{
    GLuint vertexShader = loadAndCompileShader("vertex_shader.glsl", GL_VERTEX_SHADER);
    GLuint fragmentShader = loadAndCompileShader("fragment_shader.glsl", GL_FRAGMENT_SHADER);
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_WAYLAND);
    if (!glfwInit())
    {
        return -1;
    }

    GLFWwindow *window = glfwCreateWindow(640, 480, "lorem", NULL, NULL);
    if (!window)
    {
        return -1;
    }

    glfwMakeContextCurrent(window);

    glGenBuffers(1, &triangleVertexBuffer);
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.5f, 0.5f, 0.5f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindBuffer(GL_ARRAY_BUFFER, triangleVertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), (void *)0);
        glEnableVertexAttribArray(0);
        glUseProgram(shaderProgram);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
