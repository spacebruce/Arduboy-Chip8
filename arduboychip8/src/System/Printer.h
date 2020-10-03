#pragma once
#include "../font.h"

class Printer : public Print
{
private:
  uint8_t* buffer;
  uint8_t bufferWidth, bufferHeight;
  uint8_t x = 0, y = 0;
  uint8_t xStart = 0;
  static constexpr uint8_t characterWidth = 4;
  static constexpr uint8_t characterHeight = 5;

  void newLine()
  {
    this->x = this->xStart;
    this->y += this->characterHeight + 1;
  };
  void nextLetter()
  {
    this->x += this->characterWidth + 1;
    if((this->x + this->characterWidth) > this->bufferWidth)
    {
      this->newLine();
    }
  }
public:
  Printer() = default;
  Printer(const Screen64x32& screen)
  {
    this->buffer = screen.buffer;
    this->bufferWidth = 64;
    this->bufferHeight = 32;
  }
  Printer(const Screen32x16& screen)
  {
    this->buffer = screen.buffer;
    this->bufferWidth = 32;
    this->bufferHeight = 16;
  }
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
    //Handle character
    uint8_t offset = 0x24;  //default to ?
    switch(letter)
    {
      case '\n':
        this->newLine();
        return 1;
      break;
      case ' ':
        this->nextLetter();
        return 1;
      break;
      case '0' ... '9': //Numbers
        offset = (letter - '0') + 0x0;
      break;
      case 'A' ... 'Z': //Uppercase
        offset = (letter - 'A') + 0xA;
      break;
      case 'a' ... 'z': //Lowercase
        offset = (letter - 'a') + 0xA;
      break;
      case '!':  // !
        offset = 0x25;
      break;
      default:  // ?
        offset = 0x24;
      break;
    }
    offset *= characterHeight;  //5 bytes per char (5 * 36 = 180, safely in 8bit range)

    //Cache and preshift glyph
    uint8_t charBuffer[characterHeight];
    for(uint8_t i = 0; i < characterHeight; ++i)
    {
      charBuffer[i] = static_cast<uint8_t>(pgm_read_byte(&(FontData[offset + i])) >> 4) & 0x0F;
    }

    //Draw char to buffer
    uint8_t charX = characterWidth - 1; //draw byte backwards. Why is it backwards?
    for(uint8_t drawX = this->x; drawX < (this->x + this->characterWidth); ++drawX)
    {
      uint8_t charY = 0;
      for(uint8_t drawY = this->y; drawY < (this->y + this->characterHeight); ++drawY)
      {
        const uint8_t colour = (charBuffer[charY] >> charX) & 0x1;

        const uint8_t columnHeight = 8;
        const size_t row = (drawY / columnHeight);
        const size_t bitIndex = (drawY % columnHeight);
        const size_t bufferIndex = ((row * bufferWidth) + drawX);
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
    this->nextLetter();

    return 1; //what does this signal?
  }
};
