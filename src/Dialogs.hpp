#pragma once



namespace Dialogs
{
	enum class Result
	{
		Accept,
		Reject,
		Cancel
	};

	enum class Type
	{
		Ok,
		OkCancel,
		YesNo,
		YesNoCancel
	};

	enum class Icon
	{
		Info,
		Warning,
		Error,
		Question
	};

	Result MessageBoxDialog(
		const std::string& title, 
		const std::string& message, 
		Type type,
		Icon icon, 
		std::optional<Result> defaultAction = std::nullopt);

	std::optional<std::filesystem::path> OpenFileDialog();
	std::optional<std::filesystem::path> SaveFileAsDialog();

	std::ostream& operator<<(std::ostream& os, Result result);
}