#include "SvgDeserializer2.hpp"
#include <tinyxml2.h>
#include <stb_image.h>
#include <raylib.h> // DecodeDataBase64
#include "Canvas2/Elements/ArrowElement.hpp"
#include "Canvas2/Elements/ShapeElement.hpp"
#include "Canvas2/Elements/TextElement.hpp"
#include "Canvas2/Elements/ImageElement.hpp"
#include "Render/Renderer.hpp"

namespace
{
	std::string LoadFileContent(const std::string& filename)
	{
		std::ifstream file(filename);
		if (!file.is_open())
		{
			LOG_ERROR("Failed to open file: {0}", filename);
			return "";
		}

		std::stringstream ss;
		ss << file.rdbuf();
		file.close();

		return ss.str();
	}

	std::unique_ptr<tinyxml2::XMLDocument> LoadXMLDocument(const std::string& svgContent)
	{
		auto doc = std::make_unique<tinyxml2::XMLDocument>();
		doc->Parse(svgContent.c_str());

		if (doc->Error())
		{
			LOG_ERROR("Failed to parse SVG: {0}", doc->ErrorName());
			return nullptr;
		}

		return std::move(doc);
	}

	glm::vec4 ParseColor(const char* attribute)
	{
		if (attribute == nullptr)
		{
			return { 0, 0, 0, 1 };
		}

		// TODO: This function is not robust. It only works for the specific format used in the svg files.
		// Parse rgb(r g b / a)
		std::istringstream iss(attribute);
		std::vector<std::string> colorStr;
		std::string strRed, strGreen, strBlue, strSlash, strAlpha;
		iss.get(); iss.get(); iss.get(); iss.get(); // Skip rgb(
		iss >> strRed >> strGreen >> strBlue >> strSlash >> strAlpha;

		float red = std::stof(strRed) / 255.0f;
		float green = std::stof(strGreen) / 255.0f;
		float blue = std::stof(strBlue) / 255.0f;
		float alpha = std::stof(strAlpha);

		return { red, green, blue, alpha };
	}

	std::string DeserializeTextContent(const tinyxml2::XMLElement& textElement)
	{
		if (textElement.GetText())
			return textElement.GetText();

		std::string textContent = "";
		for (auto tspan = textElement.FirstChildElement(); tspan != nullptr; tspan = tspan->NextSiblingElement())
		{
			if (tspan->Name() == std::string("tspan"))
			{
				if (const char* text = tspan->GetText())
				{
					textContent += text;
				}
				else
				{
					LOG_ERROR("Unsupported text content in tspan element in line: {}", tspan->GetLineNum());
				}
			}
			else
			{
				LOG_ERROR("Unsupported element type in <text>: <{}> in line: {}", tspan->Name(), tspan->GetLineNum());
			}
		}
		return textContent;
	}
}

SvgDeserializer2::SvgDeserializer2(CanvasCamera& camera, CanvasElements& elements, EventQueue& eventQueue)
	: m_Camera(camera), m_Elements(elements), m_EventQueue(eventQueue)
{
}

void SvgDeserializer2::Deserialize(const std::string& filename)
{
	LOG_DEBUG("Deserialize Canvas from SVG");

	(void)filename; // temporarily filename is hardcoded
	std::string svgContent = LoadFileContent("canvas.svg");
	LOG_DEBUG("SVG content: {}", svgContent);

	// Load SVG
	auto doc = LoadXMLDocument(svgContent);
	if (!doc)
	{
		LOG_ERROR("Failed to parse SVG document");
		return;
	}

	// Clear canvas
	m_Elements.Clear();

	// Load all elements at the root level and iterate over them
	auto root = doc->RootElement();
	for (auto element = root->FirstChildElement(); element != nullptr; element = element->NextSiblingElement())
	{
		DeserializeElement(*element);
	}
}

void SvgDeserializer2::DeserializeElement(const tinyxml2::XMLElement& element)
{
	const std::string_view name = element.Name();
	if (name == "path")
		DeserializeArrow(element);
	else if (name == "polygon")
		DeserializeShape(element);
	else if (name == "text")
		DeserializeText(element);
	else if (name == "image")
		DeserializeImage(element);
	else if (name == "defs")
		return; // ignore, arrowhead markers
	else if (name == "style")
		return; // ignore, fonts
	else if (name == "rect")
		return; // ignore, document border
	else
		LOG_ERROR("Unsupported element type: <{}> in line: {}", name, element.GetLineNum());
}

void SvgDeserializer2::DeserializeArrow(const tinyxml2::XMLElement& element)
{
	// TODO: Implement some better builder instead of
	// parsing it by hand like an animal
	const char* rawD = element.Attribute("d");
	std::string d = rawD ? rawD : "";

	// Split the string by spaces
	std::istringstream iss(d);
	std::vector<std::string> commands;
	std::string word;
	while (std::getline(iss, word, ' '))
	{
		commands.push_back(word);
	}

	auto arrow = std::make_unique<Elements::ArrowElement>(m_Camera);
	for (int i = 0; i < commands.size(); i++)
	{
		const auto& command = commands[ i ];
		if (command == "M" or command == "L") // Arrow's sharp point
		{
			glm::vec2 point { 
				std::stof(commands.at( i + 1 )), 
				std::stof(commands.at( i + 2 )) 
			};
			arrow->GetData().Points.emplace_back(point, m_Camera);

			i += 2;
		}
		if (command == "Q") // Arrow's bezier point
		{
			LOG_ERROR("Bezier points are not supported yet");
		}
	}

	arrow->GetData().StrokeColor = ParseColor(element.Attribute("stroke"));
	arrow->GetData().Thickness = element.FloatAttribute("stroke-width", 1.0f);
	// TODO: ArrowheadColor

	m_Elements.Add(std::move(arrow));
}

void SvgDeserializer2::DeserializeShape(const tinyxml2::XMLElement& element)
{
	const char* rawPoints = element.Attribute("points");
	const char* points = rawPoints ? rawPoints : "";

	// TODO: Wrap points in string_view and use Utils::Strings::SplitBy
	// Split the string by spaces
	std::istringstream iss(points);
	std::vector<std::string> pointsStr;
	std::string word;
	while (std::getline(iss, word, ' ')) {
		pointsStr.push_back(word);
	}
	std::vector<glm::vec2> pointsVec;
	for (const auto& point : pointsStr)
	{
		// TODO: Utils::Strings::SplitBy(',')
		std::istringstream iss(point);
		std::vector<std::string> pointStr;
		std::string word;
		while (std::getline(iss, word, ',')) {
			pointStr.push_back(word);
		}
		pointsVec.emplace_back(std::stof(pointStr.at(0)), std::stof(pointStr.at(1)));
	}

	auto shape = std::make_unique<Elements::ShapeElement>(m_Camera);
	for (const auto& point : pointsVec)
	{
		shape->GetData().Points.emplace_back(point, m_Camera);
	}

	const char* style = element.Attribute("style");
	const bool isBlendMultiply = style ? std::strstr(style, "mix-blend-mode: multiply") != nullptr : false;
	if (isBlendMultiply)
		shape->GetData().BlendMode = Render::BlendMode::Multiply;
	else
		shape->GetData().BlendMode = Render::BlendMode::Default;

	shape->GetData().Color = ParseColor(element.Attribute("fill"));

	m_Elements.Add(std::move(shape));
}

void SvgDeserializer2::DeserializeText(const tinyxml2::XMLElement& element)
{
	float x = element.FloatAttribute("x");
	float y = element.FloatAttribute("y");
	float fontSize = element.FloatAttribute("font-size", 16.0f);

	std::string textContent = DeserializeTextContent(element);
	auto text = std::make_unique<Elements::TextElement>(m_Camera, m_EventQueue);
	text->GetData().Position = { x, y };
	text->GetData().SetUtf8Text(textContent);
	text->GetData().FontSize = fontSize;

	m_Elements.Add(std::move(text));
}

void SvgDeserializer2::DeserializeImage(const tinyxml2::XMLElement& element)
{
	float x = element.FloatAttribute("x");
	float y = element.FloatAttribute("y");

	// TODO: When image scaling is implemented, use these width and height.
	//       Currently width and height are the same as the image
	[[maybe_unused]] float width = element.FloatAttribute("width");
	[[maybe_unused]] float height = element.FloatAttribute("height");

	const char* rawHref = element.Attribute("href");
	std::string_view href = rawHref ? rawHref : "";

	if (not href.starts_with("data:image/png;base64,"))
	{
		LOG_ERROR("Image is not correctly encoded. Line: {}", element.GetLineNum());
		return;
	}

	href.remove_prefix(22); // Remove the "data:image/png;base64," prefix
	std::string base64 { href.begin(), href.end() }; // TODO: This is probably redundant

	// Decode base64
	int pngDataSize;
	uint8_t* pngData = DecodeDataBase64((const unsigned char*)base64.c_str(), &pngDataSize);
	
	// Load image with stb
	int pngImageWidth, pngImageHeight;
	unsigned char* rgbaData = stbi_load_from_memory(pngData, pngDataSize, &pngImageWidth, &pngImageHeight, nullptr, 4);
	::MemFree(pngData);
	if (rgbaData == nullptr)
	{
		LOG_ERROR("Failed to load image encoded in base64. Line: {}", element.GetLineNum());
		return;
	}
	const size_t rgbaDataSize = pngImageWidth * pngImageHeight * 4;

	auto image = std::make_unique<Elements::ImageElement>();
	image->GetData().Position = { x, y };
	image->GetData().Width = pngImageWidth;
	image->GetData().Height = pngImageHeight;
	image->GetData().TextureId = Renderer::LoadTextureFromBytes(std::span { rgbaData, rgbaDataSize }, pngImageWidth, pngImageHeight);

	m_Elements.Add(std::move(image));
}
