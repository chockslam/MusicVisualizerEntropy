/// CODE was written with help by ChiliTomatoNoodle (https://www.youtube.com/c/ChiliTomatoNoodle) (https://github.com/planetchili/hw3d)

#pragma once
#include "../Common/EWin.h"
#include <string>
#include <assert.h>
#include <memory>

/// <summary>
/// Surface class thta allows drawing pixels. Used to load pictures in the application.
/// </summary>
class Surface
{
public:
	/// <summary>
	/// A single pixel.
	/// </summary>
	class Color
	{
	public:
		unsigned int dword; // RGBA encoded in UINT 
	public:
		constexpr Color()  : dword()
		{}
		constexpr Color(const Color& col) 
			:
			dword(col.dword)
		{}
		constexpr Color(unsigned int dw) 
			:
			dword(dw)
		{}
		constexpr Color(unsigned char x, unsigned char r, unsigned char g, unsigned char b) 
			:
			dword((x << 24u) | (r << 16u) | (g << 8u) | b)
		{}
		constexpr Color(unsigned char r, unsigned char g, unsigned char b) 
			:
			dword((r << 16u) | (g << 8u) | b)
		{}
		constexpr Color(Color col, unsigned char x) 
			:
			Color((x << 24u) | col.dword)
		{}
		Color& operator =(Color color) 
		{
			dword = color.dword;
			return *this;
		}
		// Getters and Setters for RGBA values, using encoding/decoding techniques.
		constexpr unsigned char GetX() const 
		{
			return dword >> 24u;
		}
		constexpr unsigned char GetA() const 
		{
			return GetX();
		}
		constexpr unsigned char GetR() const 
		{
			return (dword >> 16u) & 0xFFu;
		}
		constexpr unsigned char GetG() const 
		{
			return (dword >> 8u) & 0xFFu;
		}
		constexpr unsigned char GetB() const 
		{
			return dword & 0xFFu;
		}
		void SetX(unsigned char x) 
		{
			dword = (dword & 0xFFFFFFu) | (x << 24u);
		}
		void SetA(unsigned char a) 
		{
			SetX(a);
		}
		void SetR(unsigned char r) 
		{
			dword = (dword & 0xFF00FFFFu) | (r << 16u);
		}
		void SetG(unsigned char g) 
		{
			dword = (dword & 0xFFFF00FFu) | (g << 8u);
		}
		void SetB(unsigned char b) 
		{
			dword = (dword & 0xFFFFFF00u) | b;
		}
	};

public:

	Surface(unsigned long long width, unsigned long long height) ;
	Surface(Surface&& source) ;
	Surface() {};
	Surface(Surface&) = delete;
	Surface& operator=(Surface&& donor) ;
	Surface& operator=(const Surface&) = delete;
	~Surface();
	void Clear(Color fillValue) ;
	void PutPixel(unsigned int x, unsigned int y, Color c);
	Color GetPixel(unsigned int x, unsigned int y) const;
	unsigned int GetWidth() const ;
	unsigned int GetHeight() const ;
	Color* GetBufferPtr() ;
	const Color* GetBufferPtr() const ;
	const Color* GetBufferPtrConst() const ;
	static Surface FromFile(const std::string& name);
	void Save(const std::string& filename) const;
	void Copy(const Surface& src);
	bool AlphaLoaded() const ;
private:
	Surface(unsigned int width, unsigned int height, std::unique_ptr<Color[]> pBufferParam, bool alphaLoaded = false) ;
private:
	std::unique_ptr<Color[]> pBuffer;		// Array of pixels on the surface.
	unsigned long long width;				// Width of the picture
	unsigned long long height;				// Height of the picture
	bool alphaLoaded = false;				// checks whether a single pixel contains alpha value.
};