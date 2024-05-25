#pragma once

#include "Events/Event.hpp"
#include "KeyCodes.hpp"

namespace Events::Input
{

	class KeyPressed {
	public:
		KeyPressed(KeyCode key, bool repeated = false) : m_KeyCode(key), m_Repeated(repeated) {}

		KeyCode GetKey() const { return m_KeyCode; }

		bool IsRepeated() const { return m_Repeated; }

	private:
		KeyCode m_KeyCode;
		bool m_Repeated;
	};

	class KeyReleased {
	public:
		KeyReleased(KeyCode key) : m_KeyCode(key) {}

		KeyCode GetKey() { return m_KeyCode; }

	private:
		KeyCode m_KeyCode;
	};

	class KeyTyped {
	public:
		KeyTyped(uint32_t codePoint) : m_CodePoint(codePoint) {}

		uint32_t GetCodePoint() { return m_CodePoint; }

	private:
		uint32_t m_CodePoint;
	};

}