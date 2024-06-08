#include "pch.hpp"
#include "Dialogs.hpp"
#include <tinyfiledialogs.h>

namespace Dialogs
{
	namespace
	{
		const char* GetDialogType(Type type)
		{
			switch (type)
			{
			case Type::Ok: return "ok";
			case Type::OkCancel: return "okcancel";
			case Type::YesNo: return "yesno";
			case Type::YesNoCancel: return "yesnocancel";
			}

			throw std::runtime_error("Invalid dialog type");
		}

		const char* GetIconType(Icon icon)
		{
			switch (icon)
			{
			case Icon::Info: return "info";
			case Icon::Warning: return "warning";
			case Icon::Error: return "error";
			case Icon::Question: return "question";
			}

			throw std::runtime_error("Invalid icon type");
		}

		int GetButtonFromResult(Type type, Result result)
		{
			// 0 - "Cancel" / "No" in Type::YesNo
			// 1 - "Ok" / "Yes"
			// 2 - "No" in Type::YesNoCancel
			constexpr int cancelOrNo = 0;
			constexpr int okOrYes = 1;
			constexpr int noInYesNoCancel = 2;
			constexpr int defaultButton = -1;

			switch(type)
			{
			case Type::Ok:
				return result == Result::Accept ? okOrYes : defaultButton;
			case Type::OkCancel:
				return result == Result::Accept ? okOrYes : cancelOrNo;
			case Type::YesNo:
				return result == Result::Accept ? okOrYes : cancelOrNo;
			case Type::YesNoCancel:
				switch (result)
				{
				case Result::Accept: return okOrYes;
				case Result::Reject: return noInYesNoCancel;
				case Result::Cancel: return cancelOrNo;
				default: throw std::runtime_error("Invalid result");
				}
			}

			throw std::runtime_error("Invalid dialog type");
		}

		Result GetResultFromButton(Type type, int button)
		{
			switch (type)
			{
			case Type::Ok:
				return Result::Accept;
			case Type::OkCancel:
				return button ? Result::Accept : Result::Cancel;
			case Type::YesNo:
				return button ? Result::Accept : Result::Reject;
			case Type::YesNoCancel:
				switch (button)
				{
				case 0: return Result::Cancel;
				case 1: return Result::Accept;
				default: return Result::Reject;
				}
			}

			throw std::runtime_error("Invalid dialog type");
		}
	} // namespace

	std::optional<std::filesystem::path> OpenFileDialog()
	{
		const std::string title = "Open File";
		const std::string defaultPath = ""; // TODO: Something different?
		const std::vector<const char*> filters = { "*.svg" };
		const std::string filterName = "SVG files (*.svg)";
		const int allowMultipleSelects = 0;
		const char* result = tinyfd_openFileDialog(
			title.c_str(),
			defaultPath.c_str(),
			static_cast<int>(filters.size()),
			filters.data(),
			filterName.c_str(),
			allowMultipleSelects
		);

		if (result == nullptr)
		{
			return std::nullopt;
		}

		return std::filesystem::path(result);
	}

	std::optional<std::filesystem::path> SaveFileAsDialog()
	{
		const std::string title = "Save File As";
		const std::string defaultPath = ""; // TODO: Something different?
		const std::vector<const char*> filters = { "*.svg" };
		const std::string filterName = "SVG files (*.svg)";

		const char* result = tinyfd_saveFileDialog(
			title.c_str(),
			defaultPath.c_str(),
			static_cast<int>(filters.size()),
			filters.data(),
			filterName.c_str()
		);

		if (result == nullptr)
		{
			return std::nullopt;
		}

		return std::filesystem::path(result);
	}

	std::ostream& operator<<(std::ostream& os, Result result)
	{
		switch (result)
		{
		case Result::Accept: return os << "Accept";
		case Result::Reject: return os << "Reject";
		case Result::Cancel: return os << "Cancel";
		default: return os << "Unknown";
		}
	}

	Result MessageBoxDialog(
		const std::string& title, const std::string& message, Type type, Icon icon, std::optional<Result> defaultAction)
	{
		int buttonClicked = tinyfd_messageBox(
			title.c_str(),
			message.c_str(),
			GetDialogType(type),
			GetIconType(icon),
			defaultAction ? GetButtonFromResult(type, *defaultAction) : -1
		);

		return GetResultFromButton(type, buttonClicked);
	}
}