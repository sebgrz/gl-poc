#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <cglm/struct.h>
#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define SPRITE_TILE_SIZE 16

const vec3s vertices[] = {
    {.x = 0.0f, .y = 100.0f, .z = 0.0f}, // vertex
    {.x = 0.0f, .y = 1.0f, .z = 0.0f},
    {.x = 0.0f, .y = 0.0f, .z = 0.0f},     // vertex
    {.x = 0.0f, .y = 0.0f, .z = 0.0f},
    {.x = 100.0f, .y = 0.0f, .z = 0.0f},   // vertex
    {.x = 1.0f, .y = 0.0f, .z = 0.0f},
    {.x = 100.0f, .y = 0.0f, .z = 0.0f},   // vertex
    {.x = 1.0f, .y = 0.0f, .z = 0.0f},
    {.x = 100.0f, .y = 100.0f, .z = 0.0f}, // vertex
    {.x = 1.0f, .y = 1.0f, .z = 0.0f},
    {.x = 0.0f, .y = 100.0f, .z = 0.0f},   // vertex
    {.x = 0.0f, .y = 1.0f, .z = 0.0f},
};

GLuint triangleVertexBuffer;
GLuint vertexArrayObject;
GLuint texture;

int cameraLeftMove, cameraRightMove, cameraUpMove, cameraDownMove;

typedef struct Object
{
    GLuint vertexArrayObjectID;
    vec3s position;
    ivec2s spritePosition;
} Object;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    switch (key) {
        case GLFW_KEY_UP:
            if (action == GLFW_PRESS) {
                cameraUpMove = 1;
            } else if (action == GLFW_RELEASE) {
                cameraUpMove = 0;
            }
            return;
        case GLFW_KEY_DOWN:
            if (action == GLFW_PRESS) {
                cameraDownMove = 1;
            } else if (action == GLFW_RELEASE) {
                cameraDownMove = 0;
            }
            return;
        case GLFW_KEY_LEFT:
            if (action == GLFW_PRESS) {
                cameraLeftMove = 1;
            } else if (action == GLFW_RELEASE) {
                cameraLeftMove = 0;
            }
            return;
        case GLFW_KEY_RIGHT:
            if (action == GLFW_PRESS) {
                cameraRightMove = 1;
            } else if (action == GLFW_RELEASE) {
                cameraRightMove = 0;
            }
            return;
        default:
            return;

    }
    if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
    }
}
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

    glfwSetKeyCallback(window, keyCallback);
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

    vertexShader = loadAndCompileShader("vertex_shader_framebuffer.glsl", GL_VERTEX_SHADER);
    fragmentShader = loadAndCompileShader("fragment_shader_framebuffer.glsl", GL_FRAGMENT_SHADER);
    GLuint shaderProgramFramebuffer = glCreateProgram();
    glAttachShader(shaderProgramFramebuffer, vertexShader);
    glAttachShader(shaderProgramFramebuffer, fragmentShader);
    glLinkProgram(shaderProgramFramebuffer);
    glDetachShader(shaderProgramFramebuffer, vertexShader);
    glDetachShader(shaderProgramFramebuffer, fragmentShader);
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
    // texture without blur
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int textureWidth, textureHeight, textureChannels;
    stbi_set_flip_vertically_on_load(1);
    unsigned char *textureData = stbi_load("spelunky_shop.png", &textureWidth, &textureHeight, &textureChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(textureData);

    glUseProgram(shaderProgram);

    GLuint uniformTextureID = glGetUniformLocation(shaderProgram, "tex");
    GLuint uniformModelID = glGetUniformLocation(shaderProgram, "model");
    GLuint uniformCameraID = glGetUniformLocation(shaderProgram, "camera");
    GLuint uniformProjectionID = glGetUniformLocation(shaderProgram, "projection");
    GLuint uniformSpritePositionID = glGetUniformLocation(shaderProgram, "spritePosition");
    GLuint uniformSpriteSizeID = glGetUniformLocation(shaderProgram, "spriteSize");

    glUniform1i(uniformTextureID, 0);
    glUniform1f(uniformSpriteSizeID, 1.0 / (textureWidth / SPRITE_TILE_SIZE));

    Object objects[] = {
        {.vertexArrayObjectID = vertexArrayObject, .position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .spritePosition = { .x = 0, .y = 6 }},
        {.vertexArrayObjectID = vertexArrayObject, .position = {.x = 150.0f, .y = 0.0f, .z = 0.0f},  .spritePosition = { .x = 1, .y = 6 }},
    };

    float cameraX, cameraY = 0.0f;
    mat4s projectionMatrix = glms_ortho(0.0f, 640.0f, 0.0f, 480.0f, 0.1f, 100.0f);

    // Framebuffer -->
    GLuint frameBuffer;
    GLuint renderedTexture;
    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

    glGenTextures(1, &renderedTexture);
    glBindTexture(GL_TEXTURE_2D, renderedTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 640, 480, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glGenerateMipmap(GL_TEXTURE_2D);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    GLuint status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        printf("framebuffer status: %d", status);
        exit(123);
    }

    static const GLfloat framebufferVertices[] = { 
      -1.0f, -1.0f, 0.0f,
       1.0f, -1.0f, 0.0f,
      -1.0f,  1.0f, 0.0f,
      -1.0f,  1.0f, 0.0f,
       1.0f, -1.0f, 0.0f,
       1.0f,  1.0f, 0.0f,
    };

    GLuint framebufferVertexbuffer, framebufferVAO;
    glGenBuffers(1, &framebufferVertexbuffer);
    glGenVertexArrays(1, &framebufferVAO);
    glBindVertexArray(framebufferVAO);
    glBindBuffer(GL_ARRAY_BUFFER, framebufferVertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(framebufferVertices), framebufferVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    GLuint uniformRenderedTextureID = glGetUniformLocation(shaderProgramFramebuffer, "renderedTexture");
    // <--- Framebuffer

    while (!glfwWindowShouldClose(window))
    {
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
        glViewport(0,0,640,480);
        
        if (cameraLeftMove) {
            cameraX += 1.0;
        }
        if (cameraRightMove) {
            cameraX -= 1.0;
        }
        if (cameraUpMove) {
            cameraY -= 1.0;
        }
        if (cameraDownMove) {
            cameraY += 1.0;
        }
        mat4s cameraMatrix = glms_translated(GLMS_MAT4_IDENTITY, (vec3s){.x = cameraX, .y = cameraY, .z = -0.1f});
        glClearColor(0.5f, 0.5f, 0.5f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        glUniformMatrix4fv(uniformProjectionID, 1, GL_FALSE, (const GLfloat *)projectionMatrix.raw);
        glUniformMatrix4fv(uniformCameraID, 1, GL_FALSE, (const GLfloat *)cameraMatrix.raw);
        for (int i = 0; i < 2; i++)
        {
            Object *obj = (objects + i);

            mat4s modelMatrix = glms_translated(GLMS_MAT4_IDENTITY, obj->position);
            glUniformMatrix4fv(uniformModelID, 1, GL_FALSE, (const GLfloat *)modelMatrix.raw);
            glUniform2uiv(uniformSpritePositionID, 1, (const GLuint *)obj->spritePosition.raw);

            glBindVertexArray(obj->vertexArrayObjectID);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
        
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, 640, 480);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgramFramebuffer);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, renderedTexture);
        glUniform1i(uniformRenderedTextureID, 0);

        glBindVertexArray(framebufferVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
