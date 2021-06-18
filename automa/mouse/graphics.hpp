#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>
#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP
class Graphics
{
  sf::RenderWindow &window;
  sf::VertexArray mappa;
  sf::Image image;
  short width, height, imm_width, imm_height;
  float pan_x, pan_y, width_p, height_p, pixel_ratio;

public:
  Graphics(sf::RenderWindow &win_in, sf::Image &image_in, short imm_width_in, short imm_height_in) : window{win_in}, image{image_in}, imm_width{imm_width_in}, imm_height{imm_height_in}
  {
    mappa.resize(imm_height * imm_width * 8);
    pan_x = 0.025 * width, pan_y = 0.5 * height;
    width_p = .95 * width / imm_width, height_p = .95 * height / imm_height;
    pixel_ratio = width_p;
    if (width_p > height_p)
      pixel_ratio = height_p;
  }

  void loadImage(World &);
  void createArray(const World &pan);
  void drawArray();
  void WriteText(const std::string &string, short pos_x, short pos_y);
};

bool operator<(sf::Color right, sf::Color left);
void Window(int T, World &pan, sf::Image image, short width = 800, short height = 600);
#endif