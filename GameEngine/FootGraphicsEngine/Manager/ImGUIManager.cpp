#include "GraphicsPch.h"
#include "ImGUIManager.h"

// IMGUI 관련 헤더
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

namespace GraphicsEngineSpace
{
	std::shared_ptr<ImGUIManager> ImGUIManager::instance = nullptr;

	ImGUIManager::ImGUIManager()
		: imGUIEnable(true)
		, isFrame(false)
	{

	}

	ImGUIManager::~ImGUIManager()
	{
	}

	std::shared_ptr<ImGUIManager> ImGUIManager::GetInstance()
	{
		if (instance == nullptr)
			instance = std::make_shared<ImGUIManager>();

		return instance;
	}

	void ImGUIManager::Init()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
	}

	void ImGUIManager::Finalize()
	{
		ImGui::DestroyContext();
	}

	void ImGUIManager::InitImplWin(HWND hwnd)
	{
		ImGui_ImplWin32_Init(hwnd);
	}

	LRESULT ImGUIManager::SetWndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
		{
			return true;
		}

		return false;
	}

	void ImGUIManager::ShutDownImplWin()
	{
		ImGui_ImplWin32_Shutdown();
	}

	void ImGUIManager::InitImplDX11(ID3D11Device* device, ID3D11DeviceContext* dc)
	{
		ImGui_ImplDX11_Init(device, dc);
	}

	void ImGUIManager::ShutDownImplDX11()
	{
		ImGui_ImplDX11_Shutdown();
	}

	void ImGUIManager::Frame()
	{

		if (imGUIEnable == true)
		{
			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();
		}
	}

	void ImGUIManager::Render()
	{
		if (imGUIEnable == true)
		{
			ImGui::Render();
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		}
	}

	void ImGUIManager::EndFrame()
	{
		if(imGUIEnable == true)
		{
			ImGui::EndFrame();
		}
	}

	/*void ImGUIManager::UpdateCamFrame()
	{
		Frame();
		if(ImGui::Begin("Cam Rotation"))
		{
			ImGui::SliderFloat("Position X", &camPosition.x, -100.f, 100.f);
			ImGui::SliderFloat("Position Y", &camPosition.y, -100.f, 100.f);
			ImGui::SliderFloat("Position Z", &camPosition.z, -100.f, 100.f);

			ImGui::SliderFloat("Rotation X", &camRotator.x, 0.f, 360.f);
			ImGui::SliderFloat("Rotation Y", &camRotator.y, 0.f, 360.f);
			ImGui::SliderFloat("Rotation Z", &camRotator.z, 0.f, 360.f);
		}
		ImGui::End();
	}*/

	bool ImGUIManager::Begin(std::string guiTitle)
	{
		//Frame();
		return ImGui::Begin(guiTitle.c_str());
	}

	bool ImGUIManager::SliderFloat(std::string label, float* value, float minValue, float maxValue)
	{
		return ImGui::SliderFloat(label.c_str(), value, minValue, maxValue);
	}

	bool ImGUIManager::SliderFloat2(std::string label, float value[2], float minValue, float maxValue)
	{
		return ImGui::SliderFloat2(label.c_str(), value, minValue, maxValue);
	}

	bool ImGUIManager::SliderFloat3(std::string label, float value[3], float minValue, float maxValue)
	{
		return ImGui::SliderFloat3(label.c_str(), value, minValue, maxValue);
	}

	bool ImGUIManager::SliderFloat4(std::string label, float value[4], float minValue, float maxValue)
	{
		return ImGui::SliderFloat4(label.c_str(), value, minValue, maxValue);
	}

	void GraphicsEngineSpace::ImGUIManager::Text(std::string text)
	{
		ImGui::Text(text.c_str());
	}

	void ImGUIManager::Text(const char* text, ...)
	{
		// 사실상 Imgui::Text의 내부 구현을 따라한 수준
		va_list args;
		va_start(args, text);
		ImGui::TextV(text, args);
		va_end(args);
	}

	void ImGUIManager::End()
	{
		ImGui::End();
	}
}
