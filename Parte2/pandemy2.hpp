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
struct fromTo
{
  int rF, cF, rT, cT;
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
  int nRows, nCol;
  Grid mGrid;
  double beta = 0., gamma = 0., deathRate = 0., lockdownLimit = 1.;
  int resTime = 0, nVaccinated = 0;

public:
  World(int nRowsIn, int nColIn, double beta_, double gamma_, double deathRateIn, double ldIn, int resTimeIn, int vaccinatedIn) : nRows(nRowsIn), nCol(nColIn), mGrid((nRows * nCol), {Condition::Susceptible, 0}), beta{beta_}, gamma{gamma_},
                                                                                                                                          deathRate{deathRateIn}, lockdownLimit{ldIn}, resTime{resTimeIn}, nVaccinated{vaccinatedIn}
  {
    assert(nRows > 0 && nCol > 0);
  }

  double getBeta() const noexcept;
  void setLockdown(bool) noexcept;
  double getGamma() const noexcept;
  double getDeathRate() const noexcept;
  int getResTime() const noexcept;
  double getLockdownLimit() const noexcept;
  int getNVaccinated() const noexcept;
  int getNRows() const noexcept;
  int getNCol() const noexcept;
  int size() const noexcept;
  Condition const &getCondition(int, int) const noexcept;
  Condition &setCondition(int, int) noexcept;
  int &Time(int, int) noexcept;
  friend bool operator==(World const&, World const&);
};


template <Condition C>
int neighbours(World const &, int, int);

bool probability(double);

void movePort(World &, Condition , int, int);

void moveCell(World &, int);

void evolve(World &, int);
#endif