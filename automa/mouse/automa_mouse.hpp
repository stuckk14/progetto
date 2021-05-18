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
  Infected,
  Healed,
  Wall,
  Booked,
  Dead
};
struct from_to
{
  int r_f, c_f, r_t, c_t;
};

struct State
{
  Condition condition;
  int time;
};
bool operator==(State const &left, State const &right)
{
  return left.condition == right.condition && left.time == right.time;
}
constexpr Condition wall = Condition::Wall;

class World
{
  using Grid = std::vector<State>;
  int m_size;
  Grid m_grid;
  double beta = 0., gamma = 0., deathRate = 0., lockdownLimit = 1.;
  int resTime = 0, nVaccinati = 0;

public:
  World(int n, double beta_, double gamma_, double deathRate_in, double ld_in, int resTime_in, int vaccinati_in) : m_size(n), m_grid(m_size * m_size, {Condition::Susceptible, 0}), beta{beta_}, gamma{gamma_},
                                                                                                 deathRate{deathRate_in}, lockdownLimit{ld_in}, resTime{resTime_in}, nVaccinati{vaccinati_in}
  {
    assert(m_size > 0);
  }

  double get_beta() const noexcept
  {
    return beta;
  }
  void setLockdown(bool command) noexcept
  {
    if (command)
      beta /= 2.;
    else
      beta *= 2.;
  }

  double get_gamma() const noexcept
  {
    return gamma;
  }

  double get_deathRate() const noexcept
  {
    return deathRate;
  }
  int get_res_time() const noexcept
  {
    return resTime;
  }
  double getLockdownLimit() const noexcept
  {
    return lockdownLimit;
  }
  int getNVaccinati () const noexcept
  {
    return nVaccinati;
  }
  int size() const noexcept
  {
    return m_size;
  }

  auto begin_iterator() const noexcept
  {
    auto it = m_grid.begin();
    return it;
  }

  auto end_iterator() const noexcept
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
    return m_grid[index].condition;
  }

  Condition &setCondition(int r, int c) noexcept
  {
    //auto const i = (r + m_size) % m_size;
    //auto const j = (c + m_size) % m_size;
    assert(r >= 0 && r < m_size && c >= 0 && c < m_size);
    auto const index = r * m_size + c;
    assert(index >= 0 && index < static_cast<int>(m_grid.size()));
    return m_grid[index].condition;
  }
  int &Time(int r, int c) noexcept
  {
    auto const index = r * m_size + c;
    return m_grid[index].time;
  }
  int const &Time(int r, int c) const noexcept
  {
    auto const index = r * m_size + c;
    return m_grid[index].time;
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

inline void move_cell(World &current, World& next, int day)
{
  std::vector<from_to> fromTo;
  int start = 0, end = current.size();
  char each = 1;
  if (day % 2 == 1)
  {
    start = current.size() - 1;
    end = -1;
    each = -1;
  }
  for (int r = start; r != end; r += each)
  {
    for (int c = start; c != end; c += each)
    {
      short int empty_cells = neighbours<Condition::Empty>(current, r, c);
      if (empty_cells == 0 || current.getCondition(r, c) == Condition::Empty || current.getCondition(r, c) == Condition::Dead)
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
          if (current.getCondition(r + row, c + column) == Condition::Empty)
          {
            ++count_empty;
          }
          if (count_empty == exitus)
            break;
        }
        if (count_empty == exitus)
          break;
      }
      current.setCondition(r + row, c + column) = Condition::Booked;
      next.setCondition(r + row, c + column) = next.getCondition(r, c);
      next.setCondition(r, c) = Condition::Empty;
    }
  }
  /*for (int i = 0; i < out; ++i)
  {
    for(int j = 0; j < out; ++j)
    {
      current.getCondition(i, j)
    }
    next.setCondition(fromTo[i].r_t, fromTo[i].c_t) = next.getCondition(fromTo[i].r_f, fromTo[i].c_f);
    next.setCondition(fromTo[i].r_f, fromTo[i].c_f) = Condition::Empty;
  }*/
}

inline World evolve(World &current, int day)
{
  double beta = current.get_beta(), gamma = current.get_gamma(), deathRate = current.get_deathRate();
  int resTime = current.get_res_time();
  int const N = current.size();
  World next(N, beta, gamma, deathRate, current.getLockdownLimit(), resTime, current.getNVaccinati());
  move_cell(current, next, day);
  /*for (int r = 0; r != N; ++r)
  {
    for (int c = 0; c != N; ++c)
    {
      next.setCondition(r, c) = current.getCondition(r, c);
      next.Time(r, c) = current.Time(r, c);
      if (current.getCondition(r, c) == Condition::Dead)
        next.setCondition(r, c) = Condition::Empty;
      if (current.getCondition(r, c) == Condition::Infected && current.Time(r, c) >= resTime)
      {
        if (probability(gamma))
        {
          if (probability(deathRate))
            next.setCondition(r, c) = Condition::Dead;
          else
            next.setCondition(r, c) = Condition::Healed;
        }
      }

      else if (current.getCondition(r, c) == Condition::Susceptible)
      {
        //da studiare
        int n_infected = neighbours<Condition::Infected>(current, r, c);
        //double beta0 = beta * (7. / 16.);
        double beta_bis = beta * (n_infected / 8.);
        if (probability(beta_bis))
        {
          next.setCondition(r, c) = Condition::Infected;
          next.Time(r, c) = 0;
        }
      }
      ++next.Time(r, c);
    }
  }
  std::cerr << next.Time(25, 25) << "\t\t";*/
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

/*void changeState(sf::RenderWindow& window, World& current)
{
  while(!sf::Mouse::Button::Left);
  sf::Vector2f pos {sf::Mouse::getPosition(window)};
  int pos_x = (pos.x - pan_x) / width_p;
  int pos_y = (pos.y - pan_y) / height_p;
  current.setCondition(pos_x, pos_y) = ++current.getCondition(pos_x, pos_y) % 3;
}*/

void Window(int T, World &pan, int N, short width = 800, short height = 600)
{
  sf::RenderWindow window(sf::VideoMode(width, height), "Andamento S, I e R");
  sf::RectangleShape point(sf::Vector2f(12.f, 12.f));
  //sf::CircleShape point(7.f);
  sf::RectangleShape background(sf::Vector2f(width, height));
  background.setFillColor(sf::Color{130,130,130,255});
  window.draw(background);
  const float pan_x = 0.05 * width, pan_y = 0.05 * height;
  const float width_p = .9 * width / N, height_p = .9 * height / N;
  while(!sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
  {
    while(!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left));
    sf::Vector2f pos {sf::Mouse::getPosition(window)};
    int pos_x = std::round((pos.x - pan_x - 6.f) / width_p);
    int pos_y = std::round((pos.y - pan_y - 6.f) / height_p);
    std::cerr << "x: " << pos.x << "    col: " << pos_x << "\ty: " << pos.y << "   row:"<< pos_y <<  "\n\n";
    if (pos_x >= N || pos_x < 0 || pos_y >= N || pos_y < 0)
      continue;
    Condition state = pan.getCondition(pos_y, pos_x);
    char state_i = static_cast<char>(state);
    state = static_cast<Condition> ((state_i + 1) % 3);
    pan.setCondition(pos_y, pos_x) = state;
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
    window.display();
    std::this_thread::sleep_for(std::chrono::milliseconds(300));

  }
  Condition state;
  bool lockdown = false;
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

    
    int infected = 0, healed = 0, empty = 0, suscep = 0;
    for (int r = 0; r < N; r++)
    {
      for (int c = 0; c < N; ++c)
      {
        state = pan.getCondition(r, c);
        if (state == Condition::Infected)
          ++infected;
        else if (state == Condition::Healed)
          ++healed;
        else if (state == Condition::Empty)
          ++empty;
        else if (state == Condition::Susceptible)
          ++suscep;
      }
    }

    std::default_random_engine eng{std::random_device{}()};
    std::uniform_int_distribution<int> dist{0, N - 1};
    int nVaccinati = pan.getNVaccinati();
    for (int j = 0; j != nVaccinati && i == 3; ++j)
    {
      auto r = dist(eng);
      auto c = dist(eng);
      if(j != suscep)
      {
        while (pan.getCondition(r, c) != Condition::Susceptible)
        {
          r = dist(eng);
          c = dist(eng);
        }
        pan.setCondition(r, c) = Condition::Healed;
      }
      else
        break;
    }
    double percentageInfected = static_cast<double>(infected) / suscep;
    //std::cout << pan.getLockdownLimit() << std::endl;
    //std::cout << "a: " << a << "percentage: " << percentageInfected << "\n\n";
    if (lockdown == false && percentageInfected >= pan.getLockdownLimit())
    {
      std::cout << "Inizio lockdown!"; //\tPercentuale infetti: " << (percentageInfected * 100) << "% \n\n";
      pan.setLockdown(true);
      lockdown = true;
    }
    else if (lockdown == true && percentageInfected <= (pan.getLockdownLimit()) * 0.9)
    {
      std::cout << "Fine lockdown!"; //\tPercentuale infetti: " << (percentageInfected * 100) << "% \n\n";
      pan.setLockdown(false);
      lockdown = false;
    }
    std::cerr << "\nDays: " << i << "\tInfected: " << infected << "\tHealed: " << healed << "\tEmpty: " << empty
              << "Percentuale infetti: " << (percentageInfected * 100) << "% \n\n"
              << std::endl;

    window.display();
    std::this_thread::sleep_for(std::chrono::milliseconds(70));
    pan = evolve(pan, i);
  }
  sf::Event chiusura;
  while (window.waitEvent(chiusura))
    if (chiusura.type == sf::Event::Closed)
      window.close();
}