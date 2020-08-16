#include "UIUtils.h"

#include "IMGUI/imgui.h"
#include "IMGUI/misc/cpp/imgui_stdlib.h"

namespace Solus
{
	namespace UIUtils
	{
		std::string ConvertSizeToString(uint64_t size)
		{
			std::string sizeType;
			if (size < KB_SIZE)
				sizeType = "bytes";
			else if (size >= KB_SIZE && size < MB_SIZE)
			{
				sizeType = "kb";
				size /= KB_SIZE;
			}
			else if (size >= MB_SIZE && size < GB_SIZE)
			{
				sizeType = "mb";
				size /= MB_SIZE;
			}
			else
			{
				sizeType = "gb";
				size /= GB_SIZE;
			}
			char sizeText[128];

			sprintf_s(sizeText, "%lld %s", size, sizeType.c_str());
			return sizeText;
		}

		static std::string currentPopupId;
		static PopupData currentPopupData;

		bool OpenPopup(const std::string& id)
		{
			if (!currentPopupId.empty())
				return false;
			currentPopupId = id;
			return true;
		}

		void CloseCurrentPopup()
		{
			ImGui::CloseCurrentPopup();
			currentPopupId.clear();
		}

		void RenderPopup(PopupData& data)
		{
			if (data.title.empty() || currentPopupId.empty() || data.title != currentPopupId)
				return;
			if (!ImGui::IsPopupOpen(data.title.c_str()))
			{
				ImGui::OpenPopup(data.title.c_str());
				currentPopupData = data;
			}

			bool doPopup = false;
			if (currentPopupData.isModal)
				doPopup = ImGui::BeginPopupModal(data.title.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize);
			else
				doPopup = ImGui::BeginPopup(data.title.c_str(), ImGuiWindowFlags_AlwaysAutoResize);
			if (doPopup)
			{
				PopupType type = currentPopupData.type;

				if (type == PopupType::INFO)
					ImGui::Text("INFO");
				else if (type == PopupType::WARNING)
					ImGui::Text("WARNING");
				else if (type == PopupType::ERROR)
					ImGui::Text("ERROR");

				ImGui::Text(currentPopupData.question.c_str());
				if (type == PopupType::STRING_INPUT)
				{
					ImGui::Separator();
					ImGui::InputText(currentPopupData.inputQuestion.c_str(), &currentPopupData.inputString);
				}

				PopupAnswerType answerType = currentPopupData.answerType;

				if (answerType == PopupAnswerType::OK || answerType == PopupAnswerType::OK_CANCEL)
				{
					if (ImGui::Button("OK", ImVec2(120, 0)))
					{
						if (currentPopupData.okAction && (!currentPopupData.inputString.empty() || currentPopupData.allowEmptyInput))
							currentPopupData.okAction(currentPopupData.inputString);
						CloseCurrentPopup();
					}
					ImGui::SetItemDefaultFocus();
				}
				else if (answerType == PopupAnswerType::YES_NO || answerType == PopupAnswerType::YES_NO_CANCEL)
				{
					if (ImGui::Button("Yes", ImVec2(120, 0)))
					{
						if (currentPopupData.okAction && (!currentPopupData.inputString.empty() || currentPopupData.allowEmptyInput))
							currentPopupData.okAction(currentPopupData.inputString);
						CloseCurrentPopup();
					}
					ImGui::SetItemDefaultFocus();
					ImGui::SameLine();
					if (ImGui::Button("No", ImVec2(120, 0)))
					{
						CloseCurrentPopup();
					}
				}

				if (answerType == PopupAnswerType::OK_CANCEL || answerType == PopupAnswerType::YES_NO_CANCEL)
				{
					ImGui::SameLine();
					if (ImGui::Button("Cancel", ImVec2(120, 0)))
					{
						CloseCurrentPopup();
					}
				}
				ImGui::EndPopup();
			}
		}

		int TextInputResizeCallback(ImGuiInputTextCallbackData* data)
		{
			if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
			{
				std::string* my_str = (std::string*)data->UserData;
				IM_ASSERT(my_str->data() == data->Buf);
				my_str->resize(data->BufSize);  // NB: On resizing calls, generally data->BufSize == data->BufTextLen + 1
				data->Buf = my_str->data();
			}
			return 0;
		}

	}
}
