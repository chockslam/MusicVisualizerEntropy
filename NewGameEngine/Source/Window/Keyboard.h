/// CODE was written with help by ChiliTomatoNoodle (https://www.youtube.com/c/ChiliTomatoNoodle)

#pragma once
#include <queue>
#include <bitset>
#include <optional>

/// <summary>
/// Class represent Keyboard
/// </summary>
class Keyboard
{
	friend class Window; // window can access its private member
public:
	/// <summary>
	/// Class represent Event.
	/// </summary>
	class Event
	{
	public:
		/// <summary>
		/// enum represent the type of the event
		/// </summary>
		enum class Type
		{
			Press,
			Release,
		};
	private:
		Type type;
		unsigned char code; // code of the pressed key.
	public:
		Event(Type type, unsigned char code) 
			:
			type(type),
			code(code)
		{}
		bool IsPress() const 
		{
			return type == Type::Press;
		}
		bool IsRelease() const 
		{
			return type == Type::Release;
		}
		unsigned char GetCode() const 
		{
			return code;
		}
	};
public:
	Keyboard() = default;
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator=(const Keyboard&) = delete;
	// key event stuff
	bool KeyIsPressed(unsigned char keycode) const ;
	std::optional<Event> ReadKey() ;
	bool KeyIsEmpty() const ;
	void FlushKey() ;
	// char event stuff
	std::optional<char> ReadChar() ;
	bool CharIsEmpty() const ;
	void FlushChar() ;
	void Flush() ;
	// autorepeat control
	void EnableAutorepeat() ;
	void DisableAutorepeat() ;
	bool AutorepeatIsEnabled() const ;
private:
	void OnKeyPressed(unsigned char keycode) ;
	void OnKeyReleased(unsigned char keycode) ;
	void OnChar(char character) ;
	void ClearState() ;
	template<typename T>
	static void TrimBuffer(std::queue<T>& buffer) ;
private:
	static constexpr unsigned int nKeys = 256u;
	static constexpr unsigned int bufferSize = 16u;
	bool autorepeatEnabled = false;
	std::bitset<nKeys> keystates;
	std::queue<Event> keybuffer;
	std::queue<char> charbuffer;
};