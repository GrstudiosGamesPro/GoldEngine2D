#include "UIImplement.h"

//IMGUI DOCKING NOT WORKING

void UIImplement::start() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL  (StartEngineGraphics::window, true);
	ImGui_ImplOpenGL3_Init	      ("#version 330");
	
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
}


void UIImplement::draw() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void UIImplement::DrawCanvas() {
	ImGui::DockSpaceOverViewport();
	
	if (ImGui::Begin("Assets")) {

		ImGui::End();
	}

	if (ImGui::Begin("Hierarchy")) {

		ImGui::End();
	}

	if (ImGui::Begin("Inspector")) {

		ImGui::End();
	}
}


void UIImplement::DrawData() {
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

UIImplement::~UIImplement() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}