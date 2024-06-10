#pragma once
#include <glm/vec4.hpp>


struct Shader;

class ShaderStorage
{
public:
	const Shader& TextShader(glm::vec4 color);

private:
	static std::shared_ptr<Shader> s_Shader;
	static int s_FontColorLoc;
};