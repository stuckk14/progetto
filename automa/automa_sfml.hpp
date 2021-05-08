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
  Removed,
  Infected,
  Wall
};

constexpr Condition wall = Condition::Wall;

class World
{
  using Grid = std::vector<Condition>;
  int m_size;
  Grid m_grid;
  double beta = 0., gamma = 0.;

public:
  World(int n, double beta_, double gamma_) : m_size(n), m_grid(m_size * m_size, Condition::Susceptible), beta{beta_}, gamma{gamma_}
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

  int size() const
  {
    return m_size;
  }

  auto begin_iterator() const {
    auto it = m_grid.begin();
    return it;
  }

  auto end_iterator() const {
    auto it = m_grid.end();
    return it;
  }

  Condition const &condition(int r, int c) const noexcept
  {
    if ( r < 0 || c < 0 || r > m_size || c > m_size ) {
      return wall;
    }
    //auto const i = (r + m_size) % m_size;
    //auto const j = (c + m_size) % m_size;
    //assert(i >= 0 && i < m_size && j >= 0 && j < m_size);
    auto const index = r* m_size + c;
    //assert(index >= 0 && index < static_cast<int>(m_grid.size()));
    return m_grid[index];
  }

  Condition &condition(int r, int c) noexcept
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
};    //FINE DELLA CLASSE!

template < Condition C >
inline int neighbours(World const &world, int r, int c)
{
  int result = 0;
  for (int row = -1; row != 2; ++row)
  {
    for (int column = -1; column != 2; ++column)
    {
      if (world.condition(r + row, c + column) == C )
      {
        ++result;
      }
    }
  }
    if (world.condition(r, c) == C )
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
  if(prob == 0.)
    return false;
  return exitus <= prob;
}

inline void move_cell ( World& next) {
  for (int r= 0; r!= next.size(); ++r) {
      for (int c = 0; c!= next.size(); ++c) {
short int empty_cells = neighbours <Condition::Empty> (next, r, c);
if (empty_cells == 0 || next.condition(r,c) == Condition::Empty ) { continue; }
std::default_random_engine gen{std::random_device{}()};
std::uniform_int_distribution< short int > dist(0 , empty_cells);
short int exitus = dist(gen);
if (exitus == 0) { continue; }
int count_empty = 0, row = -1, column = -1;
for (; row != 2 && count_empty != exitus; ++row)
  {
    for ( column = -1; column != 2 && count_empty != exitus; ++column)
    {
      if ( next.condition(r + row, c + column) == Condition::Empty )
      {
        ++count_empty;
      }
    }
    
  }
next.condition(r+row, c+column) = next.condition(r,c);
next.condition(r,c) = Condition::Empty;
} 
}
}

inline World evolve(World& current)
{
  double beta = current.get_beta(), gamma = current.get_gamma();
  int const N = current.size();
  World next(N, beta, gamma);
  move_cell(current);
  for (int r = 0; r != N; ++r)
  {
    for (int c = 0; c != N; ++c)
    { 
      next.condition(r,c) = current.condition(r,c);
      if (current.condition(r, c) == Condition::Infected)
      {
        if (probability(gamma))
        {
          next.condition(r, c) = Condition::Removed;
        }
      }
      else if (current.condition(r, c) == Condition::Susceptible)
      {
          //da studiare
        int n_infected = neighbours<Condition::Infected> (current, r, c);
        //double beta0 = beta*(7./16.);
        double beta_bis = beta * (n_infected/8.);
        if (probability(beta_bis))
        {
          next.condition(r, c) = Condition::Infected;
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
        state = pan.condition(row, col);
        point.setPosition(col * width_p + pan_x, row * height_p + pan_y);
        if (state == Condition::Infected)
          point.setFillColor(sf::Color::Red);
        else if (state == Condition::Removed)
          point.setFillColor(sf::Color::Green);
        window.draw(point);
      }
    }
    {
      int a = 0, b = 0;
      for (int r = 0; r < N; r++)
      {
        for (int c = 0; c < N; ++c)
        {
          state = pan.condition(r, c);
          if (state == Condition::Infected)
            ++a;
          if (state == Condition::Removed)
            ++b;
        }
      }
      std::cerr << "Infected: " << a << "\tRemoved: " << b << std::endl;
    }
    window.display();
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    pan = evolve(pan);
  }
  sf::Event chiusura;
  while (window.waitEvent(chiusura))
    if (chiusura.type == sf::Event::Closed)
      window.close();
}