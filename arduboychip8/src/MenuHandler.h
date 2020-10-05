#pragma once

class MenuHandler
{
private:
  uint8_t selected = 0;
  uint8_t size = 8; //temp
public:
  MenuHandler() = default;
  MenuHandler(const uint8_t size)
  {
    this->size = size;
  }
  void setSelected(const uint8_t selected)
  {
    this->selected = (selected < (this->size - 1)) ? selected : (this->size - 1);
  }
  uint8_t getSelected() const
  {
    return this->selected;
  }
  uint8_t getSize() const
  {
    return this->size;
  }

  void selectNext()
  {
    if(this->selected < (this->size - 1))
      ++this->selected;
  }
  void selectPrevious()
  {
    if(this->selected > 0)
      --this->selected;
  }
};
