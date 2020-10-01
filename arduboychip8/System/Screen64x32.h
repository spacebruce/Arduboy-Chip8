#pragma once

// For size_t
#include <stddef.h>

// For uint8_t
#include <stdint.h>

// For paint8Pixels
#include <Arduboy2.h>

class Screen64x32
{
public:
	static constexpr size_t width = 64;
	static constexpr size_t height = 32;

	static constexpr size_t columnHeight = 8;

	static constexpr size_t bufferWidth = width;
	static constexpr size_t bufferHeight = (height / columnHeight);
	static constexpr size_t bufferSize = (bufferWidth * bufferHeight);

public:
	uint8_t buffer[bufferSize];
	
public:
	void setPixel(int16_t x, int16_t y, uint8_t colour)
	{
		#if defined(SCREEN_64X32_PIXEL_SAFE_MODE)
		this->setPixelSafe(x, y, colour);
		#else
		this->setPixelUnsafe(x, y, colour);
		#endif
	}

	uint8_t getPixel(uint8_t x, uint8_t y)
	{
		#if defined(SCREEN_64X32_PIXEL_SAFE_MODE)
		return this->getPixelSafe(x, y);
		#else
		return this->getPixelUnsafe(x, y);
		#endif
	}
	
	void clear()
	{
		this->fillBuffer(0x00);
	}
	
	void fillBuffer(uint8_t value)
	{
		for(size_t index = 0; index < bufferSize; ++index)
			this->buffer[index] = value;
	}

	void fillBufferFromMemory(const uint8_t (& image)[bufferSize])
	{
		for(size_t index = 0; index < bufferSize; ++index)
			this->buffer[index] = image[index];
	}

	void fillBufferFromProgmem(const uint8_t (& image)[bufferSize])
	{
		for(size_t index = 0; index < bufferSize; ++index)
			this->buffer[index] = pgm_read_byte(&image[index]);
	}

	void display()
	{
		for(size_t y = 0; y < bufferHeight; ++y)
		{
			const size_t rowLength = (y * bufferWidth);
			
			for(size_t x = 0; x < bufferWidth; ++x)
			{
				uint8_t column = this->buffer[rowLength + x];
				
				uint8_t value = 0;
				
				if((column & (1 << 0)) != 0)
					value |= (0x3 << 0);
					
				if((column & (1 << 1)) != 0)
					value |= (0x3 << 2);
					
				if((column & (1 << 2)) != 0)
					value |= (0x3 << 4);
					
				if((column & (1 << 3)) != 0)
					value |= (0x3 << 6);
				
				Arduboy2Core::paint8Pixels(value);
				Arduboy2Core::paint8Pixels(value);
			}
			
			for(size_t x = 0; x < bufferWidth; ++x)
			{
				uint8_t column = this->buffer[rowLength + x];
				
				// The compiler should translate this into a single SWAP
				column = (column << 4) | (column >> 4);
				
				uint8_t value = 0;
				
				if((column & (1 << 0)) != 0)
					value |= (0x3 << 0);
					
				if((column & (1 << 1)) != 0)
					value |= (0x3 << 2);
					
				if((column & (1 << 2)) != 0)
					value |= (0x3 << 4);
					
				if((column & (1 << 3)) != 0)
					value |= (0x3 << 6);
				
				Arduboy2Core::paint8Pixels(value);
				Arduboy2Core::paint8Pixels(value);
			}
		}
	}

private:
	uint8_t getPixelSafe(uint8_t x, uint8_t y)
	{
		if(x > (width - 1))
			return 0;
			
		if(y > (height - 1))
			return 0;

		return getPixelUnsafe(x, y);
	}

	uint8_t getPixelUnsafe(uint8_t x, uint8_t y)
	{
		const size_t row = (y / columnHeight);
		const size_t bitIndex = (y % columnHeight);
		const size_t bufferIndex = ((row * width) + x);

		return ((this->buffer[bufferIndex] >> bitIndex) & 0x1);
	}

	void setPixelSafe(int16_t x, int16_t y, uint8_t colour)
	{
		if(x < 0)
			return;
			
		if(x > static_cast<int16_t>(width - 1))
			return;
			
		if(y < 0)
			return;
			
		if(y > static_cast<int16_t>(height - 1))
			return;

		setPixelUnsafe(x, y, colour);
	}

	void setPixelUnsafe(int16_t x, int16_t y, uint8_t colour)
	{
		const size_t row = (y / columnHeight);
		const size_t bitIndex = (y % columnHeight);
		const size_t bufferIndex = ((row * width) + x);
		const size_t bit = (1 << bitIndex);

		if(colour != 0)
			this->buffer[bufferIndex] |= bit;
		else
			this->buffer[bufferIndex] &= ~bit;
	}
};