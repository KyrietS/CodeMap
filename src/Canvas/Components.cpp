#include "pch.hpp"
#include "Components.hpp"
#include "Canvas/ScriptableEntity.hpp"


namespace Components
{
	// This destructor needs to be defined here because
	// NativeScript's destructor calls std::unique_ptr's destructor
	// here. For destructing std::unique_ptr a std::default_delete
	// is used and it needs full definition of a type to work.
	//
	//  Source: https://ortogonal.github.io/cpp/forward-declaration-and-smart-pointers/
	//
	NativeScript::Instance::~Instance() = default;
	NativeScript::Instance::Instance(const Instance& other) noexcept
		: Instantiate(other.Instantiate), m_Instance(nullptr) {}
}
