#include "CSprite.h"
#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;


void CSprite::start() {

}

void CSprite::draw(Entity* owner) {
    ImGui::Button("Sprite", ImVec2(ImGui::GetContentRegionAvail().x, 20));
    ImGui::Spacing();
    string newPath = EditorGUI::InputText("File Path", owner->getComponent<SpriteComponent>().TexturePath);

    if (ImGui::BeginDragDropTarget())
    {
        ImGuiDragDropFlags target_flags = 0;
        target_flags |= ImGuiDragDropFlags_AcceptBeforeDelivery;
        target_flags |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect;


        if (ImGui::IsMouseReleased(0)) {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXTUREDA_PATH", target_flags))
            {
                const char* receivedString = static_cast<const char*>(payload->Data);


                std::string convertedPath = AComponent::RemoveDir(receivedString);
                newPath = convertedPath;



                std::cout << "Convertido: " << convertedPath << std::endl;


                owner->getComponent<SpriteComponent>().LoadTexture();
                std::cout << "String recibido: " << owner->getComponent<SpriteComponent>().TexturePath << std::endl;
            }

            if (owner->getComponent<SpriteComponent>().TexturePath != newPath) {
                owner->getComponent<SpriteComponent>().TexturePath = newPath;
                owner->getComponent<SpriteComponent>().LoadTexture();
            }
        }


        ImGui::EndDragDropTarget();
    }
}