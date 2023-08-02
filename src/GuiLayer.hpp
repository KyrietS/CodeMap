#pragma once
#include "Layer.hpp"
#include "Canvas/Serializer/CanvasSerializer.hpp"
#include "Events/EventQueue.hpp"
#include "Events/GuiEvents.hpp"

namespace Components
{
	struct Text;
}

class GuiLayer : public Layer
{
public:
	GuiLayer(EventQueue&);
	void OnUpdate() override;
	void OnEvent(Event& event) override;

private:
	void OnShowPopupEvent(const Events::Gui::ShowPopup&);

	void ShowProperties();
	void ShowMainMenuBar();
	void SaveCanvasToFile(const std::string& filename);
	void LoadCanvasFromFile(const std::string& filename);

	std::optional<std::reference_wrapper<Components::Text>> m_SelectedText;

	EventQueue& m_EventQueue;
};
