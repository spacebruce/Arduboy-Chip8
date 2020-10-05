#pragma once
#include "../font.h"
#include "Screen64x32.h"

class Printer : public Print
{
private:
  Screen64x32* buffer;
  uint8_t x = 0, y = 0;
  uint8_t xStart = 0;
  bool inverted = false;
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
    if((this->x + this->characterWidth) > Screen64x32::bufferWidth)
    {
      this->newLine();
    }
  }
public:
  Printer() = default;
  Printer(const Screen64x32& screen)
  {
    this->buffer = &screen;
  }
  void setXStart(const uint8_t x)
  {
    this->xStart = x;
  }
  void setInverted(const bool inverted)
  {
    this->inverted = inverted;
  }
  void clear()
  {
    this->setPosition(0,0);
    this->setXStart(0);
  }
  void setBuffer(const Screen64x32& screen)
  {
    this->buffer = &screen;
  }
  void setPosition(uint8_t x, uint8_t y)
  {
    this->x = x;
    this->y = y;
  }
  size_t write(uint8_t letter)
  {
    //Handle character
    uint8_t offset;
    switch(letter)
    {
      case '\0':  //perish.
        return 0;
      break;
      case '\n':
        this->newLine();
        return 1;
      break;
      case ' ':
        this->nextLetter();
        return 1;
      break;
      case '0' ... '9': // Numbers
        offset = (letter - '0') + 0x0;
      break;
      case 'A' ... 'Z': // Uppercase
        offset = (letter - 'A') + 0xA;
      break;
      case 'a' ... 'z': // Lowercase
        offset = (letter - 'a') + 0xA;
      break;
      case '?':  // Symbol '?'
        offset = 0x24;
      break;
      case '!':  // Symbol '!'
        offset = 0x25;
      break;

      default:  // unknown char, whatever
        return 1;
      break;
    }
    offset *= characterHeight;  //5 bytes per char (5 * 37 = 185, safely in 8bit range)

    //Draw char to buffer
    uint8_t charY = 0;
    for(uint8_t drawY = this->y; drawY < (this->y + this->characterHeight); ++drawY)
    {
      uint8_t glyph = static_cast<uint8_t>(pgm_read_byte(&(FontData[offset + charY])) >> 4) & 0x0F;
      if(inverted)
        glyph = ~glyph;
      uint8_t charX = characterWidth - 1; //draw byte backwards. Why is it backwards?
      for(uint8_t drawX = this->x; drawX < (this->x + this->characterWidth); ++drawX)
      {
        const uint8_t colour = (glyph >> charX) & 0x1;
        this->buffer->setPixel(drawX,drawY, colour);
        --charX;
      }
      ++charY;
    }

    //Advance char position
    this->nextLetter();

    return 1; //what does this signal?
  }
};
