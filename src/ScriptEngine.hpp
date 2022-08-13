#pragma once

class Canvas;

class ScriptEngine
{
public:
	ScriptEngine(Canvas& canvas);

	void OnScriptsUpdate();

private:
	Canvas& m_Canvas;
};

