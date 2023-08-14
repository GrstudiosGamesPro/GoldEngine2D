#include "CAudio.h"
#include "../../UI/EditorUI/EditorGUI.h"


void CAudio::start() {

}


void CAudio::draw (Entity* owner) {
	EditorGUI::Text ("Audio Source");
	string pathSound = EditorGUI::InputText("Audio Path", owner->getComponent<AudioSource>().AudioPath);
    if (ImGui::BeginDragDropTarget())
    {
        ImGuiDragDropFlags target_flags = 0;
        target_flags |= ImGuiDragDropFlags_AcceptBeforeDelivery;
        target_flags |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect;


        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MUSICH_PATH", target_flags))
        {
            if (ImGui::IsMouseReleased (0)) {
                const char* receivedString = static_cast<const char*>(payload->Data);


                pathSound = receivedString;
                std::cout << "String recibido: " << owner->getComponent<AudioSource>().AudioPath << std::endl;
            }
        }


        ImGui::EndDragDropTarget();
    }

	if (owner->getComponent<AudioSource>().AudioPath != pathSound) {
		owner->getComponent<AudioSource>().AudioPath = pathSound;
        owner->getComponent<AudioSource>().Reset();
	}

	AudioSource* sr = &owner->getComponent<AudioSource>();

	sr->SetVolumen(glm::abs(EditorGUI::Slider("Volumen", *sr->GetVolumen(), 0, 1)));
    sr->SetPan (EditorGUI::Float("Pan", sr->GetPan()));
}