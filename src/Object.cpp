#include "Object.h"

void Object::Draw(const Camera& camera, const std::vector<Light>& illuminationLights, const std::vector<ShadowLight>& shadowLights)
{
    shader->Use();
    GLuint modelLocation = glGetUniformLocation(shader->GetProgram(), "model");
    glm::mat4 model = transform.GetMatrix();
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

    GLuint viewLocation = glGetUniformLocation(shader->GetProgram(), "view");
    glm::mat4 view = camera.GetView();
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

    GLuint perspectiveLocation = glGetUniformLocation(shader->GetProgram(), "perspective");
    glm::mat4 perspective = camera.GetProjection();
    glUniformMatrix4fv(perspectiveLocation, 1, GL_FALSE, glm::value_ptr(perspective));

    GLuint viewPosLoc = glGetUniformLocation(shader->GetProgram(), "viewPos");
    glUniform3fv(viewPosLoc, 1, glm::value_ptr(camera.GetPosition()));



    // Illumination-only lights
    int numIllum = static_cast<int>(illuminationLights.size());
    for (int i = 0; i < numIllum; i++)
    {
        std::string prefix = "illuminationLights[" + std::to_string(i) + "].";

        GLuint posLoc = glGetUniformLocation(shader->GetProgram(), (prefix + "position").c_str());
        glUniform3fv(posLoc, 1, glm::value_ptr(illuminationLights[i].position));

        GLuint colLoc = glGetUniformLocation(shader->GetProgram(), (prefix + "color").c_str());
        glUniform3fv(colLoc, 1, glm::value_ptr(illuminationLights[i].color));

        GLuint intLoc = glGetUniformLocation(shader->GetProgram(), (prefix + "intensity").c_str());
        glUniform1f(intLoc, illuminationLights[i].intensity);
    }
    GLuint numIllumLoc = glGetUniformLocation(shader->GetProgram(), "numIlluminationLights");
    glUniform1i(numIllumLoc, numIllum);

    // Shadow-casting lights
    int numShadow = static_cast<int>(shadowLights.size());
    for (int i = 0; i < numShadow; i++)
    {
        std::string prefix = "shadowLights[" + std::to_string(i) + "].";

        GLuint posLoc = glGetUniformLocation(shader->GetProgram(), (prefix + "position").c_str());
        glUniform3fv(posLoc, 1, glm::value_ptr(shadowLights[i].position));

        GLuint colLoc = glGetUniformLocation(shader->GetProgram(), (prefix + "color").c_str());
        glUniform3fv(colLoc, 1, glm::value_ptr(shadowLights[i].color));

        GLuint intLoc = glGetUniformLocation(shader->GetProgram(), (prefix + "intensity").c_str());
        glUniform1f(intLoc, shadowLights[i].intensity);

        std::string matrixName = "lightSpaceMatrices[" + std::to_string(i) + "]";
        GLuint matrixLoc = glGetUniformLocation(shader->GetProgram(), matrixName.c_str());
        glUniformMatrix4fv(matrixLoc, 1, GL_FALSE, glm::value_ptr(shadowLights[i].lightSpaceMatrix));

        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, shadowLights[i].shadowMap.GetTexture());

        std::string samplerName = "shadowMaps[" + std::to_string(i) + "]";
        GLuint samplerLoc = glGetUniformLocation(shader->GetProgram(), samplerName.c_str());
        glUniform1i(samplerLoc, i);
    }
    GLuint numShadowLoc = glGetUniformLocation(shader->GetProgram(), "numShadowLights");
    glUniform1i(numShadowLoc, numShadow);

    mesh->Draw();
    shader->Unuse();
    
}

void Object::DrawDepth(std::shared_ptr<Shader> depthShader, const glm::mat4& lightSpaceMatrix)
{
    depthShader->Use();

    GLuint modelLoc = glGetUniformLocation(depthShader->GetProgram(), "model");
    glm::mat4 model = transform.GetMatrix();
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    GLuint lightSpaceLoc = glGetUniformLocation(depthShader->GetProgram(), "lightSpaceMatrix");
    glUniformMatrix4fv(lightSpaceLoc, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

    mesh->Draw();
    depthShader->Unuse();
}

void Object::Update(const UpdateContext& updateContext)
{
    for(auto& comp : components)
        comp->Update(*this, updateContext);
}