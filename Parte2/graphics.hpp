#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP
#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>

#include "pandemy.hpp"

struct SIR {
  int S{}, I{}, H{}, D{};
};
class Graphics {
  sf::RenderWindow &window;
  sf::VertexArray mappa;
  sf::Image image;
  int immWidth, immHeight, width, height;
  float panX, panY, widthP, heightP, pixelRatio;

 public:
  Graphics(sf::RenderWindow &winIn, sf::Image &imageIn, int winWidth,
           int winHeight)
      : window{winIn}, image{imageIn}, width{winWidth}, height{winHeight} {
    immWidth = image.getSize().x;
    immHeight = image.getSize().y;
    mappa.resize(immHeight * immWidth * 8);
    panX = std::abs(immWidth * 2 - winWidth) / 2,
    panY = std::abs(immHeight * 2 - winHeight) / 2;
  }
  int getPanY() const;
  void loadImage(World &);
  SIR createArray(World const &);
  void drawArray();
  void writeText(std::string const &, int, int);
  void chooseMouse(World &);
};

void Window(int T, World &, sf::Image &, int, int);
#endif