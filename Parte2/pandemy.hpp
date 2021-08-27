#ifndef PANDEMY_HPP
#define PANDEMY_HPP
#include <algorithm>
#include <cassert>
#include <random>
#include <stdexcept>
#include <vector>

enum class Condition : char {
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
struct fromTo {
  int rF, cF, rT, cT;
};

struct point {
  int r, c;
};

struct State {
  Condition condition;
  int time;
};

bool operator==(State const &, State const &);

class World {
  using Grid = std::vector<State>;
  int nRows, nCol, resTime, nVaccinated;
  Grid mGrid;
  double beta, gamma, deathRate, lockdownLimit;

 public:
  World(int nRowsIn, int nColIn, double beta_, double gamma_,
        double deathRateIn, double ldIn, int resTimeIn, int vaccinatedIn)
      : nRows{nRowsIn},
        nCol{nColIn},
        resTime{resTimeIn},
        nVaccinated{vaccinatedIn},
        mGrid((nRows * nCol), {Condition::Susceptible, 0}),
        beta{beta_},
        gamma{gamma_},
        deathRate{deathRateIn},
        lockdownLimit{ldIn} {
    if (!(nRows > 0 && nCol > 0)) {
      throw std::runtime_error{"Image error"};
    }
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
  Condition const &condition(int, int) const noexcept;
  Condition &condition(int, int) noexcept;
  int &Time(int, int) noexcept;
};

World const &wToConst(World &);

int neighbours(Condition const &, World const &, int, int);

bool probability(double);

void movePort(World &, Condition, int, int);

void moveCell(World &, int);

void evolve(World &, int);
#endif