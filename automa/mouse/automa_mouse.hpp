//g++ main_a.cpp -Wall -Wextra -lsfml-graphics -lsfml-system -lsfml-window
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
  Dead,
  Port,
  Airport,
  Wall,
  Booked
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
  int n_righe, n_col;
  Grid m_grid;
  double beta = 0., gamma = 0., deathRate = 0., lockdownLimit = 1.;
  int resTime = 0, nVaccinati = 0;

public:
  World(int n_righe_in, int n_col_in, double beta_, double gamma_, double deathRate_in, double ld_in, int resTime_in, int vaccinati_in) : n_righe(n_righe_in), n_col(n_col_in), m_grid((n_righe * n_col), {Condition::Susceptible, 0}), beta{beta_}, gamma{gamma_},
                                                                                                                                          deathRate{deathRate_in}, lockdownLimit{ld_in}, resTime{resTime_in}, nVaccinati{vaccinati_in}
  {
    assert(n_righe > 0 && n_col > 0);
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
  int getNVaccinati() const noexcept
  {
    return nVaccinati;
  }
  int getNRighe() const noexcept
  {
    return n_righe;
  }
  int getNCol() const noexcept
  {
    return n_col;
  }
  int size() const noexcept
  {
    return n_col * n_righe;
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

  inline Condition const &getCondition(int r, int c) const noexcept
  {
    if (r < 0 || c < 0 || r >= n_righe || c >= n_col)
    {
      return wall;
    }
    //auto const i = (r + m_size) % m_size;
    //auto const j = (c + m_size) % m_size;
    //assert(i >= 0 && i < m_size && j >= 0 && j < m_size);
    auto const index = r * n_col + c;
    //assert(index >= 0 && index < static_cast<int>(m_grid.size()));
    return m_grid[index].condition;
  }

  inline Condition &setCondition(int r, int c) noexcept
  {
    //auto const i = (r + m_size) % m_size;
    //auto const j = (c + m_size) % m_size;
    assert(r >= 0 && r < n_righe && c >= 0 && c < n_col);
    auto const index = r * n_col + c;
    assert(index >= 0 && index < static_cast<int>(m_grid.size()));
    return m_grid[index].condition;
  }
  int &Time(int r, int c) noexcept
  {
    auto const index = r * n_col + c;
    return m_grid[index].time;
  }
  int const &Time(int r, int c) const noexcept
  {
    auto const index = r * n_col + c;
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

inline void move_cell(World &current, int day)
{
  std::vector<from_to> fromTo;
  fromTo.reserve(10 + .8 * day);
  int start_righe = 0, start_col = 0;
  int n_righe = current.getNRighe(), n_col = current.getNCol();
  char each = 1;
  if (day % 2 == 1)
  {
    start_col = n_col - 1;
    start_righe = n_righe - 1;
    n_righe = -1;
    n_col = -1;
    each = -1;
  }
  for (int r = start_righe; r != n_righe; r += each)
  {
    for (int c = start_col; c != n_col; c += each)
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
      fromTo.push_back({r, c, r + row, c + column});
    }
  }
  for (int i = 0; i < (int)fromTo.size(); ++i)
  {
    current.setCondition(fromTo[i].r_t, fromTo[i].c_t) = current.getCondition(fromTo[i].r_f, fromTo[i].c_f);
    current.setCondition(fromTo[i].r_f, fromTo[i].c_f) = Condition::Empty;
  }
}

inline World evolve(World &current, int day)
{
  double beta = current.get_beta(), gamma = current.get_gamma(), deathRate = current.get_deathRate();
  int resTime = current.get_res_time();
  int const n_righe = current.getNRighe(), n_col = current.getNCol();
  World next(n_righe, n_col, beta, gamma, deathRate, current.getLockdownLimit(), resTime, current.getNVaccinati());
  move_cell(current, day);
  for (int r = 0; r != n_righe; ++r)
  {
    for (int c = 0; c != n_col; ++c)
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
  return next;
}

inline void WriteText(sf::RenderWindow &window, const std::string &string, short pos_x, short pos_y)
{
  sf::Font font;
  if (!font.loadFromFile("arial.ttf"))
    std::cerr << "\n\nError loading font\n\n";
  sf::Text text;
  text.setFont(font);
  text.setCharacterSize(pos_y / 2);
  text.setFillColor(sf::Color::White);
  text.setString(string);
  float text_w = text.getGlobalBounds().width;
  float text_h = text.getGlobalBounds().height;
  text.setPosition(pos_x - text_w / 2, (pos_y - text_h) / 2);
  window.draw(text);
}

/*void changeState(sf::RenderWindow& window, World& current)
{
  while(!sf::Mouse::Button::Left);
  sf::Vector2f pos {sf::Mouse::getPosition(window)};
  int pos_x = (pos.x - pan_x) / pixel_ratio;
  int pos_y = (pos.y - pan_y) / pixel_ratio;
  current.setCondition(pos_x, pos_y) = ++current.getCondition(pos_x, pos_y) % 3;
}*/

inline bool operator<(sf::Color right, sf::Color left)
{
  return right.toInteger() < left.toInteger();
}
void Window(int T, World &pan, sf::Image immagine, int n_righe, int n_col, short width = 800, short height = 600)
{
  sf::RenderWindow window(sf::VideoMode(width, height), "Andamento S, I e R");
  //sf::CircleShape point(7.f);
  sf::RectangleShape background(sf::Vector2f(width, height));
  background.setFillColor(sf::Color::Black);
  window.draw(background);

  sf::Vector2u dim = immagine.getSize();
  short imm_height = dim.x, imm_width = dim.y;
  const float pan_x = 0.025 * width, pan_y = 0.025 * height;
  const float width_p = .95 * width / n_col, height_p = .95 * height / n_righe;
  float pixel_ratio = width_p;
  if (width_p > height_p)
    pixel_ratio = height_p;
  sf::RectangleShape point(sf::Vector2f(1.f, 1.f));
  for (short row = 0; row < imm_height; ++row) //Creazione da immagine
    for (short col = 0; col < imm_width; ++col)
    {
      if (immagine.getPixel(row, col).r == 255 && immagine.getPixel(row, col).g < 230)
        pan.setCondition(col, row) = Condition::Port;
      else if (immagine.getPixel(row, col).g > 250 && immagine.getPixel(row, col).r < 230)
        pan.setCondition(col, row) = Condition::Airport;
      else if (immagine.getPixel(row, col).b > 150)
        pan.setCondition(col, row) = Condition::Wall;
      else
        pan.setCondition(col, row) = Condition::Susceptible;
    }
  while (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) //Scelta da mouse
  {
    while (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
      ;
    sf::Vector2f pos{sf::Mouse::getPosition(window)};
    int pos_x = std::round((pos.x - pan_x - 6.f) / pixel_ratio);
    int pos_y = std::round((pos.y - pan_y - 6.f) / pixel_ratio);
    std::cerr << "x: " << pos.x << "    col: " << pos_x << "\ty: " << pos.y << "   row:" << pos_y << "\n\n";
    if (pos_x >= n_col || pos_x < 0 || pos_y >= n_righe || pos_y < 0)
      continue;
    Condition state = pan.getCondition(pos_y, pos_x);
    char state_i = static_cast<char>(state);
    state = static_cast<Condition>((state_i + 1) % 3);
    pan.setCondition(pos_y, pos_x) = state;
    for (int row = 0; row < n_righe; ++row)
    {
      for (int col = 0; col < n_col; ++col)
      {
        point.setFillColor(sf::Color::Black);
        state = pan.getCondition(row, col);
        point.setPosition(col * 2.f + pan_x, row * 2.f + pan_y);
        switch (state)
        {
        case Condition::Susceptible:
          point.setFillColor(sf::Color::Blue);
          break;
        case Condition::Infected:
          point.setFillColor(sf::Color::Red);
          break;
        case Condition::Healed:
          point.setFillColor(sf::Color::Green);
          break;
        case Condition::Dead:
          point.setFillColor(sf::Color{220, 200, 0, 255});
          break;
        case Condition::Port:
          point.setFillColor(sf::Color::Magenta);
          break;
        case Condition::Airport:
          point.setFillColor(sf::Color::Cyan);
          break;
        default:
          break;
        }
        window.draw(point);
      }
    }
    window.display();
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
  }
  Condition state;
  bool lockdown = false;
  int deaths = 0;
  for (int i = 1; i <= T; ++i) //Evoluzione
  {
    window.clear(sf::Color::Black);
    for (int row = 0; row < n_righe; ++row)
    {
      for (int col = 0; col < n_col; ++col)
      {
        point.setFillColor(sf::Color::Black);
        state = pan.getCondition(row, col);
        point.setPosition(col * 2.f + pan_x, row * 2.f + pan_y);
        switch (state)
        {
        case Condition::Susceptible:
          point.setFillColor(sf::Color::Blue);
          break;
        case Condition::Infected:
          point.setFillColor(sf::Color::Red);
          break;
        case Condition::Healed:
          point.setFillColor(sf::Color::Green);
          break;
        case Condition::Dead:
          point.setFillColor(sf::Color{220, 200, 0, 255});
          break;
        case Condition::Port:
          point.setFillColor(sf::Color::Magenta);
          break;
        case Condition::Airport:
          point.setFillColor(sf::Color::Cyan);
          break;
        default:
          break;
        }
        window.draw(point);
      }
    }
    int infected = 0, healed = 0, empty = 0, suscep = 0;
    for (int r = 0; r < n_righe; r++)
    {
      for (int c = 0; c < n_col; ++c)
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
        else if (state == Condition::Dead)
          ++deaths;
      }
    }
    std::string dati{"Day: "};
    dati = dati + std::to_string(i) + "   Susceptibles: " + std::to_string(suscep) + "   Infected: " + std::to_string(infected) + "   Healed: " + std::to_string(healed) + "   Dead: " + std::to_string(deaths);
    WriteText(window, dati, width / 2, pan_y);
    std::default_random_engine eng{std::random_device{}()};
    std::uniform_int_distribution<int> dist_righe{0, n_righe - 1}, dist_col{0, n_col - 1};
    int vaccinatiPerGiorno = std::round(pan.getNVaccinati() / 10.);
    int totVaccinati = 0;
    for (int j = 0; j != vaccinatiPerGiorno && i >= T / 5 && i < ((T / 5) + 10); ++j)
    {
      auto r = dist_righe(eng);
      auto c = dist_col(eng);
      if (totVaccinati != suscep)
      {
        while (pan.getCondition(r, c) != Condition::Susceptible)
        {
          r = dist_righe(eng);
          c = dist_col(eng);
        }
        pan.setCondition(r, c) = Condition::Healed;
        ++totVaccinati;
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
  //  std::this_thread::sleep_for(std::chrono::milliseconds(100));
    pan = evolve(pan, i);
  }
  sf::Event chiusura;
  while (window.waitEvent(chiusura))
    if (chiusura.type == sf::Event::Closed)
      window.close();
}