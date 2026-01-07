#include "Object.h"

void Object::Draw(const Camera& camera, const Light& light)
{
    shader.Use();
    GLuint modelLocation = glGetUniformLocation(shader.GetProgram(), "model");
    glm::mat4 model = transform.GetMatrix();
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
    GLuint viewLocation = glGetUniformLocation(shader.GetProgram(), "view");
    glm::mat4 view = camera.GetView();
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
    GLuint perspectiveLocation = glGetUniformLocation(shader.GetProgram(), "perspective");
    glm::mat4 perspective = camera.GetProjection();
    glUniformMatrix4fv(perspectiveLocation, 1, GL_FALSE, glm::value_ptr(perspective));

    //GLuint viewPosLoc = glGetUniformLocation(shader.GetProgram(), "viewPos");
    //glUniform3fv(viewPosLoc, 1, glm::value_ptr(camera.position));

    GLuint lightPosLoc = glGetUniformLocation(shader.GetProgram(), "lightPos");
    glUniform3fv(lightPosLoc, 1, glm::value_ptr(light.position));
    GLuint lightColLoc = glGetUniformLocation(shader.GetProgram(), "lightColor");
    glUniform3fv(lightColLoc, 1, glm::value_ptr(light.color));
    GLuint lightIntLoc = glGetUniformLocation(shader.GetProgram(), "lightInt");
    glUniform1f(lightIntLoc, light.intensity);

    mesh.Draw();
    shader.Unuse();
    
}