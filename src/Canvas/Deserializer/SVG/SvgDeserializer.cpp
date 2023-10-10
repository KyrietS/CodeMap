#include "SvgDeserializer.hpp"
#include <tinyxml2.h>
#include <sstream>
#include <string_view>
#include <Prefabs/TextEntity.hpp>
#include <Prefabs/ArrowEntity.hpp>
#include <stb_image.h>
#include <raylib.h> // DecodeDataBase64
#include <Prefabs/ImageEntity.hpp>
#include <Prefabs/HighlightEntity.hpp>

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
}

SvgDeserializer::SvgDeserializer(Canvas& canvas, entt::registry& registry, EventQueue& eventQueue) 
	: m_Canvas(canvas), m_Registry(registry), m_EventQueue(eventQueue)
{
}

void SvgDeserializer::Deserialize(const std::string& filename)
{
	LOG_DEBUG("Deserialize Canvas from SVG");

	std::string svgContent = LoadFileContent(filename);
	LOG_DEBUG("SVG content: {}", svgContent);

	// Load SVG
	auto doc = LoadXMLDocument(svgContent);
	if (!doc)
	{
		return;
	}

	// Clear canvas
	m_Registry.clear();

	// Load all elements at the root level and iterate over them
	auto root = doc->RootElement();
	for (auto element = root->FirstChildElement(); element != nullptr; element = element->NextSiblingElement())
	{
		DeserializeElement(*element);
	}
}

void SvgDeserializer::DeserializeElement(const tinyxml2::XMLElement& element)
{
	// TODO: Maybe use some chain of responsibility pattern here?
	const std::string_view name = element.Name();
	if (name == "text")
		DeserializeText(element);
	else if (name == "path")
		DeserializeArrow(element);
	else if (name == "image")
		DeserializeImage(element);
	else if (name == "polygon")
		DeserializeHighlight(element);
	else if (name == "defs")
		return; // Ignore
	else if (name == "style")
		return; // Ignore
	else
		LOG_ERROR("Unsupported element type: <{}> in line: {}", name, element.GetLineNum());
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

void SvgDeserializer::DeserializeText(const tinyxml2::XMLElement& element)
{
	float x = element.FloatAttribute("x");
	float y = element.FloatAttribute("y");
	float fontSize = element.FloatAttribute("font-size");

	auto text = DeserializeTextContent(element);
	TextEntity(m_Canvas.CreateEntity({ x, y }), m_EventQueue).Build(text, fontSize);
}

void SvgDeserializer::DeserializeArrow(const tinyxml2::XMLElement& element)
{
	const char* rawD = element.Attribute("d");
	std::string d = rawD ? rawD : "";

	// Split the string by spaces
	std::istringstream iss(d);
	std::vector<std::string> commands;
	std::string word;
	while (std::getline(iss, word, ' ')) {
		commands.push_back(word);
	}

	glm::vec2 begin, end, bezier;
	bool isLineCurved = false;

	for(int i = 0; i < commands.size(); i++)
	{
		const auto& command = commands[i];
		if (command == "M") // Load begin point
		{
			begin.x = std::stof(commands[i + 1]);
			begin.y = std::stof(commands[i + 2]);
			i += 2;
		}
		if (command == "L") // Load end point
		{
			end.x = std::stof(commands[i + 1]);
			end.y = std::stof(commands[i + 2]);
			i += 2;
		}
		if (command == "Q") // Load bezier point with end
		{
			bezier.x = std::stof(commands[i + 1]);
			bezier.y = std::stof(commands[i + 2]);
			end.x = std::stof(commands[i + 3]);
			end.y = std::stof(commands[i + 4]);
			i += 4;
			isLineCurved = true;
		}
	}

	auto arrow = ArrowEntity(m_Canvas.CreateEntity(begin), m_EventQueue).Build();
	arrow.GetComponent<Components::Arrow>().End = end - begin;
	if (isLineCurved)
		arrow.GetComponent<Components::Arrow>().ControlPoint = bezier - begin;
}

void SvgDeserializer::DeserializeImage(const tinyxml2::XMLElement& element)
{
	float x = element.FloatAttribute("x");
	float y = element.FloatAttribute("y");

	// TODO: When image scaling is implemented, use these width and height.
	//       Currently width and height are the same as the image
	[[maybe_unused]] float width = element.FloatAttribute("width");
	[[maybe_unused]] float height = element.FloatAttribute("height");

	const char* rawHref = element.Attribute("href");
	std::string_view href = rawHref ? rawHref : "";


	if (not href.starts_with("data:image/png;base64,")) {
		LOG_ERROR("Image is not correctly encoded");
		return;
	}
	href.remove_prefix(22);
	std::string base64{href.begin(), href.end()};

	// Decode base64
	int pngDataSize;
	uint8_t* pngData = DecodeDataBase64(reinterpret_cast<const unsigned char*>(base64.c_str()), &pngDataSize);

	// Load image with stb
	int pngImageWidth, pngImageHeight;
	unsigned char* rgbaData = stbi_load_from_memory(pngData, pngDataSize, &pngImageWidth, &pngImageHeight, nullptr, 4);
	::MemFree(pngData);
	if (rgbaData == nullptr) {
		LOG_ERROR("Failed to load image encoded in base64");
		return;
	}

	ImageEntity(m_Canvas.CreateEntity(), m_EventQueue).Build({x, y}, rgbaData, pngImageWidth, pngImageHeight);

	stbi_image_free(rgbaData);
}

glm::vec4 ParseColor(const char* attribute)
{
	if (attribute == nullptr)
	{
		return {0, 0, 0, 1};  
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

void SvgDeserializer::DeserializeHighlight(const tinyxml2::XMLElement& element)
{
	const char* rawPoints = element.Attribute("points");
	const char* points = rawPoints ? rawPoints : "";
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
		std::istringstream iss(point);
		std::vector<std::string> pointStr;
		std::string word;
		while (std::getline(iss, word, ',')) {
			pointStr.push_back(word);
		}
		pointsVec.emplace_back(std::stof(pointStr[0]), std::stof(pointStr[1]));
	}
	auto entity = HighlightEntity(m_Canvas.CreateEntity(), m_EventQueue);
	auto& highlight = entity.GetComponent<Components::Highlight>();
	highlight.Points = pointsVec;

	const char* style = element.Attribute("style");
	auto isBlendMultiply = style ? std::strstr(style, "mix-blend-mode: multiply") != nullptr : false;
	if (isBlendMultiply)
		highlight.BlendMode = Render::BlendMode::Multiply;
	else
		highlight.BlendMode = Render::BlendMode::Default;

	highlight.Color = ParseColor(element.Attribute("fill"));
}