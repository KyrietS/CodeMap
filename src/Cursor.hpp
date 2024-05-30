#pragma once

class Cursor
{
public:
	Cursor(void* window);
	void Lock(bool status);
	void Reset();
	void SetArrowImage();

private:
	void* m_Window = nullptr;
	bool m_Locked = false;
};

struct CursorImage
{
	std::vector<uint8_t> Pixels;
	uint32_t Width;
	uint32_t Height;
};