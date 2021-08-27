#ifndef GRAPHICS1_HPP
#define GRAPHICS1_HPP
#include <SFML/Graphics.hpp>
#include <cassert>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <thread>
#include <vector>

#include "pandemy1.hpp"

class Graphics {
  sf::RenderWindow &window;
  int width, height;
  float panX, panY;

 public:
  Graphics(sf::RenderWindow &winIn, int widthIn, int heightIn, float panXIn,
           float panYIn)
      : window{winIn},
        width{widthIn},
        height{heightIn},
        panX{panXIn},
        panY{panYIn} {}

  void drawAxis();
  void writeTextGraph(const std::string &, int, int, int, bool);
  void drawGraph(Population &s, const int, const int);
  void drawLegend();
};

void printSIR(Population &);

void window(int, Pandemy &, int, int, int);
#endif