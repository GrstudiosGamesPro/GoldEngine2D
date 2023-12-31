#include "UIImplement.h"
#include "UIStyle.h"

//IMGUI DOCKING NOT WORKING

void UIImplement::start() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	UIStyle::SetStyleUI(Dracula);

	//	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(StartEngineGraphics::window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}


void UIImplement::draw() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	
}

void UIImplement::DrawCanvas() {

}

void UIImplement::RenderUI() {
	ImGui::Render();
}



void UIImplement::DrawData() {
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

UIImplement::~UIImplement() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}