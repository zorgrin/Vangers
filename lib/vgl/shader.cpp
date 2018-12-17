//
// Created by nikita on 2018-12-03.
//
#include "shader.h"
#include "util.h"

void vgl::Shader::bindUniformAttribs(UniformData &data) {
	for(auto* member: data.members){
		member->attrId = getAttribute(member->name);
	}
}

std::shared_ptr<vgl::Shader> vgl::Shader::create(const std::string &vertexShaderCode,
                              const std::string &fragmentShaderCode) {
	auto objectId = vgl::loadShadersStr(vertexShaderCode, fragmentShaderCode);
	return std::make_shared<vgl::Shader>(objectId);
}


GLint vgl::Shader::getAttribute(const std::string &name) const{
	auto res = glGetUniformLocation(objectId, name.c_str());
	vgl::checkErrorAndThrow("glGetUniformLocation");
	return res;
}

void vgl::Shader::addTexture(std::shared_ptr<ITexture> texture, const std::string &name) {
	auto textureAttribute = getAttribute(name);
	textureAttributes.emplace_back(TextureAttribute(textureAttribute, texture));
}

std::shared_ptr<vgl::Shader>
vgl::Shader::createFromPath(const std::string &vertexShaderPath, const std::string &fragmentShaderPath) {
	auto objectId = vgl::loadShaders(vertexShaderPath, fragmentShaderPath);
	return std::make_shared<vgl::Shader>(objectId);
}

void vgl::Shader::render(vgl::UniformData &data, const std::shared_ptr<vgl::IVertexArray> &vertexArray, 
	const std::vector<TextureAttribute>& textureAttributes){
    use();

    for(auto* member: data.members){
        member->assignData();
    }

    for(int i = 0; i < textureAttributes.size();i++){
        glActiveTexture(GlTextures[i]);
        textureAttributes[i].texture->bind();
        glUniform1i(textureAttributes[i].getObjectId(), i);
    }

    vertexArray->enable();
    glDrawElements(GL_TRIANGLES, vertexArray->getNumElements(), GL_UNSIGNED_INT, 0);
    vgl::checkErrorAndThrow("glDrawElements");
    vertexArray->disable();
}

void vgl::Shader::render(vgl::UniformData &data, const std::shared_ptr<vgl::IVertexArray> &vertexArray) {
    render(data, vertexArray, textureAttributes);
}