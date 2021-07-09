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
  float pan_x, pan_y;

public:
  Graphics(sf::RenderWindow &win_in, short width_in, short height_in, float pan_x_in, float pan_y_in) : window{win_in}, width{width_in}, height{height_in}, pan_x{pan_x_in}, pan_y{pan_y_in} {}

  void drawAxis();
  void WriteTextGraph(const std::string &string, short pos_x, short pos_y);
  void drawPoint(Population &state, const int i, const int duration);
};


void printSIR(Population &);

void window(int, Pandemy &, int, short, short);
#endif