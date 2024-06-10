#include "pch.hpp"
#include "ShaderStorage.hpp"
#include <raylib.h>

namespace
{
	const char* TEXT_FRAGMENT_SHADER = R"(
	#version 330

	// Input vertex attributes (from vertex shader)
	in vec2 fragTexCoord;
	in vec4 fragColor;

	// Input uniform values
	uniform sampler2D texture0;
	uniform vec4 fontColor;

	// Output fragment color
	out vec4 finalColor;

	void main()
	{
		vec4 texelColor = texture(texture0, fragTexCoord);
		finalColor = vec4(fontColor.rgb, fontColor.a * texelColor.a);
	}
)";

	std::shared_ptr<Shader> LoadTextShader()
	{
		Shader shader = ::LoadShaderFromMemory(nullptr, TEXT_FRAGMENT_SHADER);
		LOG_INFO("Text Shader loaded");
		return std::shared_ptr<Shader>( new Shader(shader), [](Shader* ptr) {
			if (ptr)
			{
				::UnloadShader(*ptr);
				delete ptr;
			}
		});
	}
}

std::shared_ptr<Shader> ShaderStorage::s_Shader;
int ShaderStorage::s_FontColorLoc;

const Shader& ShaderStorage::TextShader(glm::vec4 color)
{
	if (not s_Shader)
	{
		s_Shader = LoadTextShader();
		s_FontColorLoc = ::GetShaderLocation(*s_Shader, "fontColor");
	}

	::SetShaderValue(*s_Shader, s_FontColorLoc, &color[0], SHADER_UNIFORM_VEC4);

	return *s_Shader;
}
