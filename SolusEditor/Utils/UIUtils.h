#pragma once

#include <string>
#include <cstdint>
#include <functional>

namespace Solus
{
	
	namespace UIUtils
	{
		const uint64_t KB_SIZE = 1024;
		const uint64_t MB_SIZE = KB_SIZE * KB_SIZE;
		const uint64_t GB_SIZE = MB_SIZE * MB_SIZE;

		std::string ConvertSizeToString(uint64_t size);

		enum class PopupType : uint8_t
		{
			INFO,
			WARNING,
			ERROR,
			STRING_INPUT
		};

		enum class PopupAnswerType : uint8_t
		{
			YES_NO,
			YES_NO_CANCEL,
			OK_CANCEL,
			OK
		};

		struct PopupData
		{
			std::string title;
			std::string question;
			std::string inputQuestion;
			std::string inputString;
			bool allowEmptyInput = false;
			PopupType type = PopupType::INFO;
			PopupAnswerType answerType = PopupAnswerType::OK_CANCEL;
			bool isModal = false;
			std::function<void(std::string&)> okAction;

			void* id = nullptr;

			PopupData& operator=(const PopupData& other)
			{
				type = other.type;
				answerType = other.answerType;
				isModal = other.isModal;
				title = other.title;
				question = other.question;
				inputQuestion = other.inputQuestion;
				allowEmptyInput = other.allowEmptyInput;
				okAction = other.okAction;
				return *this;
			}
		};

		bool OpenPopup(const std::string& id);

		void CloseCurrentPopup();

		void RenderPopup(PopupData& data);
	}
}