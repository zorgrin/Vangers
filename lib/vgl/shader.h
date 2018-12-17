//
// Created by nikita on 2018-12-03.
//

#ifndef UPLOADBENCH_SHADER_H
#define UPLOADBENCH_SHADER_H

#include <string>
#include <vector>
#include <memory>

#include "uniform.h"
#include "util.h"
#include "base.h"
#include "vertex_array.h"
#include "itexture.h"

namespace vgl {
	class _EmptyTexture: public ITexture{
		void bind(){}
		void free(){}
	};

	class TextureAttribute: public NamedObject<GLint>{
	public:
		std::shared_ptr<ITexture> texture;

		TextureAttribute(GLint objectId, std::shared_ptr<ITexture>& texture):
			NamedObject(objectId),
			texture(texture){}
	};

	const GLuint GlTextures[] = {
		GL_TEXTURE0,
		GL_TEXTURE1,
		GL_TEXTURE2,
		GL_TEXTURE3
	};

	class Shader : public NamedObject<GLuint> {
		std::vector<TextureAttribute> textureAttributes; // TODO: move textureAttributes outside of Shader
	public:
		void bindUniformAttribs(UniformData &data);

		void addTexture(std::shared_ptr<ITexture> texture, const std::string &name);

		Shader(GLuint objectId):NamedObject(objectId){}

		Shader(Shader&& other) noexcept : NamedObject(other.objectId){}

		Shader(const Shader&) = delete;

		Shader& operator=(const Shader&) = delete;

		Shader& operator=(Shader&& other) noexcept{
			objectId = other.objectId;
			return *this;
		}

		GLint getAttribute(const std::string& name) const;

		void use() const {
			glUseProgram(objectId);
			vgl::checkErrorAndThrow("glUseProgram");
		}

		static std::shared_ptr<Shader> create(const std::string &vertexShaderCode,
		                     const std::string &fragmentShaderCode);

		static std::shared_ptr<Shader> createFromPath(const std::string &vertexShaderPath,
		                                      const std::string &fragmentShaderPath);

		void render(UniformData &data, const std::shared_ptr<IVertexArray>& vertexArray);

		void render(UniformData& data, const std::shared_ptr<IVertexArray>& vertexArray, 
			const std::vector<TextureAttribute>& textureAttributes);
			
		void free(){
			glDeleteProgram(objectId);
		}
	};
}
#endif //UPLOADBENCH_SHADER_H
