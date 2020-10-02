#pragma once
#include "../font.h"

class Printer : public Print
{
private:
  uint8_t* buffer;
  uint8_t bufferWidth, bufferHeight;
  uint8_t x = 0, y = 0;
  const uint8_t CharacterWidth = 4;
  const uint8_t CharacterHeight = 5;
public:
  Printer() = default;
  Printer(uint8_t* buffer, uint8_t width, uint8_t height)
  {
    this->setBuffer(buffer, width, height);
  }
  void setBuffer(uint8_t* buffer, uint8_t width, uint8_t height) //Possibly better way of doing this. Rethink.
  {
    this->buffer = buffer;
    this->bufferWidth = width;
    this->bufferHeight = height;
  }
  void setPosition(uint8_t x, uint8_t y)
  {
    this->x = x;
    this->y = y;
  }
  size_t write(uint8_t letter)
  {
    //Find offset in font table
    uint8_t offset = 0x24;  //default to ?
    if(letter >= 0x30 && letter <= 0x39)  //Numbers
      offset = (letter - 0x30) + 0x0;
    else if (letter >= 0x41 && letter <= 0x5A) //Capitals
      offset = (letter - 0x41) + 0xA;
    else if (letter >= 0x64 && letter <= 0x7A) //Lowercase
      offset = (letter - 0x64) + 0xA;
    else if(letter == 0x21)  // !
      offset = 0x25;
    else if(letter == 0x3F)  // ?
      offset = 0x24;

    offset *= CharacterHeight;  //5 bytes per char

    //Cache glyph
    uint8_t charBuffer[CharacterHeight];
    for(uint8_t i = 0; i < CharacterHeight; ++i)
    {
      charBuffer[i] = static_cast<uint8_t>(pgm_read_byte(&(FontData[offset + i])) >> 4) & 0x0F;
    }

    //Draw char to buffer
    uint8_t charX = CharacterWidth - 1;
    for(uint8_t drawX = this->x; drawX < (this->x + this->CharacterWidth); ++drawX)
    {
      uint8_t charY = 0;
      for(uint8_t drawY = this->y; drawY < (this->y + this->CharacterHeight); ++drawY)
      {
        const uint8_t colour = (charBuffer[charY] >> (charX)) & 0x1;

        const uint8_t columnHeight = 8;
        const size_t row = (drawY / columnHeight);
        const size_t bitIndex = (drawY % columnHeight);
        const size_t bufferIndex = ((row * bufferWidth) +drawX);
        const size_t bit = (1 << bitIndex);

        if(colour != 0)
          this->buffer[bufferIndex] |= bit;
        else
          this->buffer[bufferIndex] &= ~bit;

        ++charY;
      }
      --charX;
    }

    //Advance char position
    this->x += this->CharacterWidth + 1;
    if ((this->x + this->CharacterWidth) > this->bufferWidth)
    {
      this->x = 0;  //have a configurable x start position?
      this->y += this->CharacterHeight + 1;
    }

    return 1; //what does this signal?
  }
};
