#define GL_GLEXT_PROTOTYPES

#include <cglm/struct.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const vec3s vertices[] = {
    {.x = 0.0f, .y = 0.5f, .z = 0.0f}, // vertex
    {.x = 0.0f, .y = 1.0f, .z = 0.0f},
    {.x = 0.0f, .y = 0.0f, .z = 0.0f}, // vertex
    {.x = 0.0f, .y = 0.0f, .z = 0.0f},
    {.x = 0.5f, .y = 0.0f, .z = 0.0f}, // vertex
    {.x = 1.0f, .y = 0.0f, .z = 0.0f},
    {.x = 0.5f, .y = 0.0f, .z = 0.0f}, // vertex
    {.x = 1.0f, .y = 0.0f, .z = 0.0f},
    {.x = 0.5f, .y = 0.5f, .z = 0.0f}, // vertex
    {.x = 1.0f, .y = 1.0f, .z = 0.0f},
    {.x = 0.0f, .y = 0.5f, .z = 0.0f}, // vertex
    {.x = 0.0f, .y = 1.0f, .z = 0.0f},
};

GLuint triangleVertexBuffer;
GLuint vertexArrayObject;
GLuint texture;

char *readFile(const char *filename)
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

GLuint loadAndCompileShader(const char *filePath, GLenum shaderType)
{
    char *shaderSrc = readFile(filePath);
    const char *cShaderSrc = shaderSrc;
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &cShaderSrc, NULL);
    glCompileShader(shader);
    free(shaderSrc);

    GLint success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        printf("Shader error %d %s", shaderType, infoLog);
        exit(-1);
    }

    return shader;
}

int main()
{
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

    GLuint vertexShader = loadAndCompileShader("vertex_shader.glsl", GL_VERTEX_SHADER);
    GLuint fragmentShader = loadAndCompileShader("fragment_shader.glsl", GL_FRAGMENT_SHADER);
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDetachShader(shaderProgram, vertexShader);
    glDetachShader(shaderProgram, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glGenBuffers(1, &triangleVertexBuffer);
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    glBindBuffer(GL_ARRAY_BUFFER, triangleVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vec3), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vec3), (void *)(sizeof(vec3)));
    glEnableVertexAttribArray(1);

    // Texture
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int textureWidth, textureHeight, textureChannels;
    unsigned char *textureData = stbi_load("spelunky_shop.png", &textureWidth, &textureHeight, &textureChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(textureData);

    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, "tex"), 0);
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.5f, 0.5f, 0.5f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(vertexArrayObject);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
