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
	void OnShowPropertiesEvent(const Events::Gui::ShowProperties&);
	void OnShowPopupEvent(const Events::Gui::ShowPopup&);

	void ShowProperties();
	void ShowPropertiesFor(Components::Transform&);
	void ShowPropertiesFor(Components::Text&);
	void ShowPropertiesFor(Components::Arrow&);
	void ShowPropertiesFor(Components::Highlight&);
	void ShowMainMenuBar();
	void SaveCanvasToFile(const std::string& filename);
	void LoadCanvasFromFile(const std::string& filename);

	EventQueue& m_EventQueue;
	std::unique_ptr<Entity> m_SelectedEntity;
};
