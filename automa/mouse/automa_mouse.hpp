//g++ main_a.cpp -Wall -Wextra -lsfml-graphics -lsfml-system -lsfml-window
#include <vector>
#include <array>
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
struct point
{
  int r, c;
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

inline void move_port(World &current, Condition porto, int oldR, int oldC)
{
  std::vector<point> destCell;
  destCell.resize(200);
  int i = 0;
  for (int r = 0; r < current.getNRighe(); ++r)
  {
    for (int c = 0; c < current.getNCol(); ++c)
    {
      if (current.getCondition(r, c) == porto)
      {
        if (neighbours<Condition::Empty>(current, r, c) != 0)
        {
          int row = -1, column;
          for (; row != 2; ++row)
          {
            for (column = -1; column != 2; ++column)
            {
              if (current.getCondition(r + row, c + column) == Condition::Empty)
              {
                destCell.push_back(point{r + row, c + column});
                ++i;
              }
            }
          }
        }
      }
    }
  }
  if (i == 0)
    return;
  std::default_random_engine gen{std::random_device{}()};
  std::uniform_int_distribution<short int> dist(0, destCell.size() - 1);

  short int arriveCell = dist(gen);
  //std::cerr << "i: " << i << "\tr: " << destCell[arriveCell].r <<"\tc: " << destCell[arriveCell].c << "\n\n\n";
  if(current.getCondition(destCell[arriveCell].r, destCell[arriveCell].c) == porto || current.getCondition(oldR, oldC) == porto)
    return;
  Condition temp = current.getCondition(destCell[arriveCell].r, destCell[arriveCell].c);
  current.setCondition(destCell[arriveCell].r, destCell[arriveCell].c) = current.getCondition(oldR, oldC);
  current.setCondition(oldR, oldC) = temp;
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
      empty_cells += neighbours<Condition::Airport>(current, r, c);
      empty_cells += neighbours<Condition::Port>(current, r, c);
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
          if (current.getCondition(r + row, c + column) == Condition::Empty || current.getCondition(r + row, c + column) == Condition::Port || current.getCondition(r + row, c + column) == Condition::Airport)
          {
            ++count_empty;
          }
          if (count_empty == exitus)
            break;
        }
        if (count_empty == exitus)
          break;
      }
      if (current.getCondition(r + row, c + column) == Condition::Port || current.getCondition(r + row, c + column) == Condition::Airport)
      {
        move_port(current, current.getCondition(r + row, c + column), r, c);
        continue;
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
    pan_x = 0.025 * width, pan_y = 0.05 * height;
    width_p = .95 * width / imm_width, height_p = .95 * height / imm_height;
    pixel_ratio = width_p;
    if (width_p > height_p)
      pixel_ratio = height_p;
  }
  //Graphics(short width_in, short height_in) : width{width_in}, height{height_in} {}

  void loadImage(World &pan)
  {
    /*if (!image.loadFromFile(name))
      std::cerr << "Errore nell'apertura di: " << name;
    unsigned short imm_height = image.getSize().y, imm_width = image.getSize().x;*/
    for (unsigned short row = 0; row < imm_height; ++row)
    {
      for (unsigned short col = 0; col < imm_width; ++col)
      {
        if (image.getPixel(col, row).r == 255 && image.getPixel(col, row).g < 230)
          pan.setCondition(row, col) = Condition::Port;

        else if (image.getPixel(col, row).g > 250 && image.getPixel(col, row).r < 230)
          pan.setCondition(row, col) = Condition::Airport;

        else if (image.getPixel(col, row).r < 100)
          pan.setCondition(row, col) = Condition::Susceptible;

        else
          pan.setCondition(row, col) = Condition::Wall;
      }
    }
  }
  void createArray(const World &pan)
  {
    unsigned short r{};
    float lato = 1.f, spaziatura = 2.f;
    unsigned short imm_height = image.getSize().y, imm_width = image.getSize().x;
    //std::cerr << "imm_height: " << imm_height << "\timm_width: " << imm_width << "\n\n";
    for (unsigned short row = 0; row < imm_height; ++row)
    {
      unsigned short c = 0;
      for (unsigned short col = 0; col < imm_width; ++col)
      {
        c += 4;
        int index{r * imm_width * 4 + c};
        mappa[index].position = sf::Vector2f(col * 2 * lato + pan_x, row * 2 * lato + pan_y);
        mappa[index + 1].position = sf::Vector2f(col * 2 * lato + pan_x, row * 2 * lato + pan_y + spaziatura);
        mappa[index + 2].position = sf::Vector2f(col * 2 * lato + pan_x + spaziatura, row * 2 * lato + pan_y + spaziatura);
        mappa[index + 3].position = sf::Vector2f(col * 2 * lato + pan_x + spaziatura, row * 2 * lato + pan_y);

        mappa[index].color = sf::Color::Black;
        mappa[index + 1].color = sf::Color::Black;
        mappa[index + 2].color = sf::Color::Black;
        mappa[index + 3].color = sf::Color::Black;

        switch (pan.getCondition(row, col))
        {
        case Condition::Susceptible:
        {
          mappa[index].color = sf::Color::Blue;
          mappa[index + 1].color = sf::Color::Blue;
          mappa[index + 2].color = sf::Color::Blue;
          mappa[index + 3].color = sf::Color::Blue;
          break;
        }
        case Condition::Infected:
        {
          mappa[index].color = sf::Color::Red;
          mappa[index + 1].color = sf::Color::Red;
          mappa[index + 2].color = sf::Color::Red;
          mappa[index + 3].color = sf::Color::Red;
          break;
        }
        case Condition::Healed:
        {
          mappa[index].color = sf::Color::Green;
          mappa[index + 1].color = sf::Color::Green;
          mappa[index + 2].color = sf::Color::Green;
          mappa[index + 3].color = sf::Color::Green;
          break;
        }
        case Condition::Dead:
        {
          mappa[index].color = sf::Color{220, 200, 0, 255};
          mappa[index + 1].color = sf::Color{220, 200, 0, 255};
          mappa[index + 2].color = sf::Color{220, 200, 0, 255};
          mappa[index + 3].color = sf::Color{220, 200, 0, 255};
          break;
        }
        case Condition::Port:
        {
          mappa[index].color = sf::Color{185, 185, 185, 255};
          mappa[index + 1].color = sf::Color{185, 185, 185, 255};
          mappa[index + 2].color = sf::Color{185, 185, 185, 255};
          mappa[index + 3].color = sf::Color{185, 185, 185, 255};
          break;
        }
        case Condition::Airport:
        {
          mappa[index].color = sf::Color::White;
          mappa[index + 1].color = sf::Color::White;
          mappa[index + 2].color = sf::Color::White;
          mappa[index + 3].color = sf::Color::White;
          break;
        }
        default:
        {
          break;
        }
        }
      }
      ++r;
    }
  }
  void drawArray()
  {
    window.clear(sf::Color::Black);
    window.draw(mappa);
    window.display();
  }
  inline void WriteText(const std::string &string, short pos_x, short pos_y)
  {
    sf::Font font;
    if (!font.loadFromFile("arial.ttf"))
      std::cerr << "\n\nError loading font\n\n";
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::White);
    text.setString(string);
    float text_w = text.getGlobalBounds().width;
    float text_h = text.getGlobalBounds().height;
    text.setPosition(pos_x - text_w / 2, (pos_y - text_h));
    window.draw(text);
  }
};

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
void Window(int T, World &pan, sf::Image image, short width = 800, short height = 600)
{
  sf::RenderWindow window(sf::VideoMode(width, height), "Andamento S, I e R");
  //sf::CircleShape point(7.f);
  sf::RectangleShape background(sf::Vector2f(width, height));
  background.setFillColor(sf::Color::Black);
  window.draw(background);

  Graphics graph(window, image, image.getSize().x, image.getSize().y);
  graph.createArray(pan);
  graph.drawArray();
  sf::Vector2u dim = image.getSize();
  short imm_height = dim.x, imm_width = dim.y;
  const float pan_x = 0.025 * width, pan_y = 0.025 * height;
  const float width_p = .95 * width / imm_height, height_p = .95 * height / imm_width;
  float pixel_ratio = width_p;
  if (width_p > height_p)
    pixel_ratio = height_p;
  graph.loadImage(pan);

  while (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) //Scelta da mouse
  {
    while (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
      ;
    sf::Vector2f pos{sf::Mouse::getPosition(window)};
    int pos_x = std::round((pos.x) / 2.);
    int pos_y = std::round((pos.y) / 2.);
    std::cerr << "x: " << pos.x << "    col: " << pos_x << "\ty: " << pos.y << "   row:" << pos_y << "\n\n";
    //if (pos_x >= imm_width || pos_x < 0 || pos_y >= imm_height || pos_y < 0)
    //continue;
    Condition state = pan.getCondition(pos_y, pos_x);
    if (state != Condition::Susceptible && state != Condition::Infected && state != Condition::Empty)
      continue;
    char state_i = static_cast<char>(state);
    state = static_cast<Condition>((state_i + 1) % 3);
    pan.setCondition(pos_y, pos_x) = state;
    graph.createArray(pan);
    graph.drawArray();
    //std::this_thread::sleep_for(std::chrono::milliseconds(250));
  }
  Condition state;
  bool lockdown = false;
  int deaths = 0;
  for (int i = 1; i <= T; ++i) //Evoluzione
  {
    graph.createArray(pan);
    graph.drawArray();

    //Conteggi
    int infected = 0, healed = 0, empty = 0, suscep = 0;
    for (int r = 0; r < imm_height; r++)
    {
      for (int c = 0; c < imm_width; ++c)
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
    graph.WriteText(dati, width / 2, height - pan_y);
    window.display();
    //Vaccinazioni
    std::default_random_engine eng{std::random_device{}()};
    std::uniform_int_distribution<int> dist_righe{0, imm_height - 1}, dist_col{0, imm_width - 1};
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

    //Lockdown
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

    //  std::this_thread::sleep_for(std::chrono::milliseconds(100));
    pan = evolve(pan, i);
  }
  sf::Event chiusura;
  while (window.waitEvent(chiusura))
    if (chiusura.type == sf::Event::Closed)
      window.close();
}
