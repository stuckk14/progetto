//g++ main_a.cpp -Wall -Wextra -lsfml-graphics -
#include <vector>
#include <cassert>
#include <random>
#include <cmath>
#include <algorithm>
#include <SFML/Graphics.hpp>

//Classe automa cellulare, cella piena, evoluzione probabilistica con numeri casuali.

enum class Condition : char
{
  Empty,
  Susceptible,
  Healed,
  Infected,
  Wall,
  Booked,
  Dead
};
struct from_to
{
  int r_f, c_f, r_t, c_t;
};

constexpr Condition wall = Condition::Wall;

class World
{
  using Grid = std::vector<Condition>;
  int m_size;
  Grid m_grid;
  double beta = 0., gamma = 0., deathRate = 0.;

public:
  World(int n, double beta_, double gamma_, double deathRate_in) : m_size(n), m_grid(m_size * m_size, Condition::Susceptible), beta{beta_}, gamma{gamma_}, 
                                                                    deathRate{deathRate_in}
  {
    assert(m_size > 0);
  }

  double get_beta() const
  {
    return beta;
  }

  double get_gamma() const
  {
    return gamma;
  }

  double get_deathRate() const
  {
    return deathRate;
  }
  int size() const
  {
    return m_size;
  }

  auto begin_iterator() const
  {
    auto it = m_grid.begin();
    return it;
  }

  auto end_iterator() const
  {
    auto it = m_grid.end();
    return it;
  }

  Condition const &getCondition(int r, int c) const noexcept
  {
    if (r < 0 || c < 0 || r >= m_size || c >= m_size)
    {
      return wall;
    }
    //auto const i = (r + m_size) % m_size;
    //auto const j = (c + m_size) % m_size;
    //assert(i >= 0 && i < m_size && j >= 0 && j < m_size);
    auto const index = r * m_size + c;
    //assert(index >= 0 && index < static_cast<int>(m_grid.size()));
    return m_grid[index];
  }

  Condition &setCondition(int r, int c) noexcept
  {
    //auto const i = (r + m_size) % m_size;
    //auto const j = (c + m_size) % m_size;
    assert(r >= 0 && r < m_size && c >= 0 && c < m_size);
    auto const index = r * m_size + c;
    assert(index >= 0 && index < static_cast<int>(m_grid.size()));
    return m_grid[index];
  }

  friend bool operator==(World const &l, World const &r)
  {
    return l.m_grid == r.m_grid;
  }
}; //FINE DELLA CLASSE!

template <Condition C>
inline int neighbours(World const &world, int r, int c)
{
  int result = 0;
  for (int row = -1; row != 2; ++row)
  {
    for (int column = -1; column != 2; ++column)
    {
      if (world.getCondition(r + row, c + column) == C)
      {
        ++result;
      }
    }
  }
  if (world.getCondition(r, c) == C)
  {
    return result - 1;
  }
  else
  {
    return result;
  }
}

inline bool probability(double prob)
{
  std::default_random_engine gen{std::random_device{}()};
  std::uniform_real_distribution<double> dist(0., 1.);
  double exitus = dist(gen);
  if (prob == 0.)
    return false;
  return exitus <= prob;
}

inline void move_cell(World &next, int day)
{
  std::vector<from_to> fromTo;
  int init = 0, out = next.size();
  char each = 1;
  if(day % 2 == 1)
  {
    init = next.size() - 1;
    out = -1;
    each = -1;
  }
  for (int r = init; r != out; r += each)
  {
    for (int c = init; c != out; c += each)
    {
      short int empty_cells = neighbours<Condition::Empty>(next, r, c);
      if (empty_cells == 0 || next.getCondition(r, c) == Condition::Empty || next.getCondition(r, c) == Condition::Dead)
      {
        continue;
      }
      std::default_random_engine gen{std::random_device{}()};
      std::uniform_int_distribution<short int> dist(0, empty_cells);
      short int exitus = dist(gen);
      if (exitus == 0)
      {
        continue;
      }
      int count_empty = 0, row = -1, column = -1;
      for (; row != 2 && count_empty != exitus; ++row)
      {
        for (column = -1; column != 2 && count_empty != exitus; ++column)
        {
          if (next.getCondition(r + row, c + column) == Condition::Empty)
          {
            ++count_empty;
          }
          if (count_empty == exitus)
            break;
        }
        if (count_empty == exitus)
          break;
      }
      next.setCondition(r + row, c + column) = Condition::Booked;
      fromTo.push_back({r, c, r + row, c + column});
    }
  }
  for (int i = 0; i < (int)fromTo.size(); ++i)
  {
    next.setCondition(fromTo[i].r_t, fromTo[i].c_t) = next.getCondition(fromTo[i].r_f, fromTo[i].c_f);
    next.setCondition(fromTo[i].r_f, fromTo[i].c_f) = Condition::Empty;
  }
}

inline World evolve(World& current, int day)
{
  double beta = current.get_beta(), gamma = current.get_gamma(), deathRate = current.get_deathRate();
  int const N = current.size();
  World next(N, beta, gamma, deathRate);
  move_cell(current, day);
  for (int r = 0; r != N; ++r)
  {
    for (int c = 0; c != N; ++c)
    {
      next.setCondition(r, c) = current.getCondition(r, c);
      /*if(current.getCondition(r, c) == Condition::Dead)
        next.setCondition(r, c) = Condition::Empty;*/
      if (current.getCondition(r, c) == Condition::Infected)
      {
        if (probability(gamma))
        {
          if(probability(deathRate))
            next.setCondition(r, c) = Condition::Dead;
          else
            next.setCondition(r, c) = Condition::Healed;
        }
      }
      else if (current.getCondition(r, c) == Condition::Susceptible)
      {
        //da studiare
        int n_infected = neighbours<Condition::Infected>(current, r, c);
        //double beta0 = beta*(7./16.);
        double beta_bis = beta * (n_infected / 8.);
        if (probability(beta_bis))
        {
          next.setCondition(r, c) = Condition::Infected;
        }
      }
    }
  }
  return next;
}

void WriteText(sf::RenderWindow &window, const std::string &string, short pos_x, short pos_y)
{
  sf::Font font;
  if (!font.loadFromFile("arial.ttf"))
    std::cerr << "\n\nError loading font\n\n";
  sf::Text text;
  text.setFont(font);
  text.setCharacterSize(15);
  text.setFillColor(sf::Color::Black);
  text.setString(string);
  float text_w = text.getGlobalBounds().width;
  text.setPosition(pos_x - text_w, pos_y);
  window.draw(text);
}
void Window(int T, World &pan, int N, short width = 800, short height = 600)
{
  sf::RenderWindow window(sf::VideoMode(width, height), "Andamento S, I e R");
  //sf::RectangleShape point(sf::Vector2f(3.f, 3.f));
  sf::CircleShape point(7.f);
  sf::RectangleShape background(sf::Vector2f(width, height));
  background.setFillColor(sf::Color::White);
  window.draw(background);
  const float pan_x = 0.05 * width, pan_y = 0.05 * height;
  const float width_p = .9 * width / N, height_p = .9 * height / N;
  getchar();
  Condition state;
  for (int i = 0; i < T; ++i)
  {
    for (int row = 0; row < N; ++row)
    {
      for (int col = 0; col < N; ++col)
      {
        point.setFillColor(sf::Color::Blue);
        state = pan.getCondition(row, col);
        point.setPosition(col * width_p + pan_x, row * height_p + pan_y);
        if (state == Condition::Infected)
          point.setFillColor(sf::Color::Red);
        else if (state == Condition::Healed)
          point.setFillColor(sf::Color::Green);
        else if (state == Condition::Dead)
          point.setFillColor(sf::Color{220, 200, 0, 255});
        else if (state == Condition::Empty)
          point.setFillColor(sf::Color::Black);
        window.draw(point);
      }
    }
    {
      int a = 0, b = 0, c = 0;
      for (int r = 0; r < N; r++)
      {
        for (int c = 0; c < N; ++c)
        {
          state = pan.getCondition(r, c);
          if (state == Condition::Infected)
            ++a;
          if (state == Condition::Healed)
            ++b;
          if (state == Condition::Empty)
            ++c;
        }
      }
      std::cerr << "Infected: " << a << "\tHealed: " << b << "\tEmpty: " << c << std::endl;
    }
    window.display();
    std::this_thread::sleep_for(std::chrono::milliseconds(750));
    pan = evolve(pan, i);
  }
  sf::Event chiusura;
  while (window.waitEvent(chiusura))
    if (chiusura.type == sf::Event::Closed)
      window.close();
}