#pragma once

class MenuEntry
{
private:
public:
};

class MenuHandler
{
private:
  uint8_t selected = 0;
  uint8_t size = 8; //temp
public:
  void setSelected(const uint8_t selected)
  {
    this->selected = selected;
  }
  uint8_t getSelected() const
  {
    return this->selected;
  }
  uint8_t getSize() const
  {
    return this->size;
  }
};
