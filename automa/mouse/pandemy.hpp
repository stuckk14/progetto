#include <random>
#include <cassert>
#include <vector>
#ifndef PANDEMY_HPP
#define PANDEMY_HPP

enum class Condition : char
{
  Empty,
  Susceptible,
  Infected,
  Healed,
  Dead,
  Port,
  Airport,
  Desert,
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

bool operator==(State const &, State const &);

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

  double get_beta() const noexcept;
  void setLockdown(bool) noexcept;
  double get_gamma() const noexcept;
  double get_deathRate() const noexcept;
  int get_res_time() const noexcept;
  double getLockdownLimit() const noexcept;
  int getNVaccinati() const noexcept;
  int getNRighe() const noexcept;
  int getNCol() const noexcept;
  int size() const noexcept;
  Condition const &getCondition(int, int) const noexcept;
  Condition &setCondition(int, int) noexcept;
  int &Time(int, int) noexcept;
  int const &Time(int, int) const noexcept;
  friend bool operator==(World const&, World const&);
};


template <Condition C>
int neighbours(World const &world, int r, int c);

bool probability(double prob);

void move_port(World &, Condition , int, int);

void move_cell(World &, int);

World evolve(World &, int);
#endif