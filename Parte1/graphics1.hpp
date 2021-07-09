#include <vector>
#include <cmath>
#include <cassert>
#include <iostream>
#include <thread>
#include <iomanip>
#include <SFML/Graphics.hpp>
#include "pandemy1.hpp"
#ifndef GRAPHICS1_HPP
#define GRAPHICS1_HPP
struct SIR
{
  int S{}, I{}, H{}, D{};
};
class Graphics
{
  sf::RenderWindow &window;
  short width, height;
  float panX, panY;

public:
  Graphics(sf::RenderWindow &winIn, short widthIn, short heightIn, float panXIn, float panYIn) : window{winIn}, width{widthIn}, height{heightIn}, panX{panXIn}, panY{panYIn} {}

  void drawAxis();
  void writeTextGraph(const std::string &string, short posX, short posY);
  void drawGraph(Population &state, const int i, const int duration);
};


void printSIR(Population &);

void window(int, Pandemy &, int, short, short);
#endif