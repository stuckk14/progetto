#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "pandemy2.hpp"
#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP
struct SIR
{
  int S{}, I{}, H{}, D{};
};
class Graphics
{
  sf::RenderWindow &window;
  sf::VertexArray mappa;
  sf::Image image;
  short immWidth, immHeight, width, height;
  float panX, panY, widthP, heightP, pixelRatio;

public:
  Graphics(sf::RenderWindow &winIn, sf::Image &imageIn, short winWidth, short winHeight) : window{winIn}, image{imageIn}, width{winWidth}, height{winHeight}
  {
    immWidth = image.getSize().x;
    immHeight = image.getSize().y;
    mappa.resize(immHeight * immWidth * 8);
    panX = std::abs(immWidth * 2 - winWidth) / 2, panY = std::abs(immHeight * 2 - winHeight) / 2;

  }
  short getPanY() const;
  void loadImage(World &);
  SIR createArray(const World &);
  void drawArray();
  void writeText(const std::string &, short, short);
  void chooseMouse(World &);
};

void Window(int T, World &, sf::Image image, short, short);
#endif