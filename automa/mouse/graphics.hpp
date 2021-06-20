#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>
#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP
struct SIR{
    int S{}, I{}, H{}, D{};
};
class Graphics
{
  sf::RenderWindow &window;
  sf::VertexArray mappa;
  sf::Image image;
  short imm_width, imm_height, width, height;
  float pan_x, pan_y, width_p, height_p, pixel_ratio;

public:
  Graphics(sf::RenderWindow &win_in, sf::Image &image_in, short win_width, short win_height) : window{win_in}, image{image_in}, width{win_width}, height{win_height}
  {
    imm_width = image.getSize().x;
    imm_height = image.getSize().y;
    mappa.resize(imm_height * imm_width * 8);
    //pan_x = 0.1 * imm_width, pan_y = 0.1 * imm_height;
    pan_x = std::abs(imm_width * 2 - win_width) / 2, pan_y = std::abs(imm_height * 2 - win_height) / 2;
    /*width_p = .95 * width / imm_width, height_p = .95 * height / imm_height;
    pixel_ratio = width_p;
    if (width_p > height_p)
      pixel_ratio = height_p;*/
  }
  short getPanY() const;

  void loadImage(World &);
  SIR createArray(const World &);
  void drawArray();
  void WriteText(const std::string &, short, short);
  void chooseMouse(World&);
};

void Window(int T, World &pan, sf::Image image, short width, short height);
#endif