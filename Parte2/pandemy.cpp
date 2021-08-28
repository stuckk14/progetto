#include "pandemy.hpp"
constexpr Condition wall = Condition::Wall;
inline bool operator==(State const &left, State const &right) {
  return left.condition == right.condition && left.time == right.time;
}

double World::getBeta() const noexcept { return beta; }
void World::setLockdown(bool command) noexcept {
  if (command)
    beta /= 2.5;
  else
    beta *= 2.5;
}
double World::getGamma() const noexcept { return gamma; }

double World::getDeathRate() const noexcept { return deathRate; }
int World::getResTime() const noexcept { return resTime; }
double World::getLockdownLimit() const noexcept { return lockdownLimit; }
int World::getNVaccinated() const noexcept { return nVaccinated; }
int World::getNRows() const noexcept { return nRows; }
int World::getNCol() const noexcept { return nCol; }
int World::size() const noexcept { return nCol * nRows; }

Condition const &World::condition(int r, int c) const noexcept {
  if (r < 0 || c < 0 || r >= nRows || c >= nCol) {
    return wall;
  }
  auto const index = r * nCol + c;
  return mGrid[index].condition;
}
Condition &World::condition(int r, int c) noexcept {
  assert(r >= 0 && r < nRows && c >= 0 && c < nCol);
  auto const index = r * nCol + c;
  assert(index >= 0 && index < static_cast<int>(mGrid.size()));
  return mGrid[index].condition;
}
inline int &World::Time(int r, int c) noexcept {
  auto const index = r * nCol + c;
  return mGrid[index].time;
}
// Funzione per non ripetere "static_cast<World const&>" per tutto il programma
World const &wToConst(World &world) {
  return static_cast<World const &>(world);
}

inline int neighbours(Condition const &C, World const &world, int r, int c) {
  int result = 0;
  for (int row = -1; row != 2; ++row) {
    for (int column = -1; column != 2; ++column) {
      if (world.condition(r + row, c + column) == C) {
        ++result;
      }
    }
  }
  if (world.condition(r, c) == C) {
    return result - 1;
  } else {
    return result;
  }
}

inline bool probability(double prob) {
  std::default_random_engine gen{std::random_device{}()};
  std::uniform_real_distribution<double> dist(0., 1.);
  double exitus = dist(gen);
  if (prob == 0.) return false;
  return exitus <= prob;
}

inline void movePort(World &current, Condition porto, int oldR, int oldC) {
  std::vector<point> destCell;
  destCell.resize(200);
  int i = 0;
  for (int r = 0; r < current.getNRows(); ++r) {
    for (int c = 0; c < current.getNCol(); ++c) {
      if (wToConst(current).condition(r, c) == porto &&
          neighbours(Condition::Empty, current, r, c) != 0) {
        int row = -1, column;
        for (; row != 2; ++row)
          for (column = -1; column != 2; ++column)
            if (wToConst(current).condition(r + row, c + column) ==
                Condition::Empty) {
              destCell.push_back(point{r + row, c + column});
              ++i;
            }
      }
    }
  }
  if (i == 0) return;
  std::default_random_engine gen{std::random_device{}()};
  std::uniform_int_distribution<int> dist(0, destCell.size() - 1);

  int arriveCell = dist(gen);
  if (wToConst(current).condition(destCell[arriveCell].r,
                                  destCell[arriveCell].c) == porto ||
      wToConst(current).condition(oldR, oldC) == porto)
    return;
  Condition temp = wToConst(current).condition(destCell[arriveCell].r,
                                               destCell[arriveCell].c);
  current.condition(destCell[arriveCell].r, destCell[arriveCell].c) =
      wToConst(current).condition(oldR, oldC);
  current.condition(oldR, oldC) = temp;
}

inline void moveCell(World &current, int day) {
  std::vector<fromTo> fromTo;
  fromTo.reserve(10 + .8 * day);
  int startRows = 0, startCol = 0;
  int nRows = current.getNRows(), nCol = current.getNCol();
  char each = 1;
  if (day % 2 == 1) {
    startCol = nCol - 1;
    startRows = nRows - 1;
    nRows = -1;
    nCol = -1;
    each = -1;
  }
  for (int r = startRows; r != nRows; r += each) {
    for (int c = startCol; c != nCol; c += each) {
      int emptyCells = neighbours(Condition::Empty, current, r, c);
      emptyCells += neighbours(Condition::Airport, current, r, c);
      emptyCells += neighbours(Condition::Port, current, r, c);
      if (wToConst(current).condition(r, c) == Condition::Wall ||
          emptyCells == 0 ||
          wToConst(current).condition(r, c) == Condition::Desert ||
          wToConst(current).condition(r, c) == Condition::Empty ||
          wToConst(current).condition(r, c) == Condition::Dead) {
        continue;
      }
      std::default_random_engine gen{std::random_device{}()};
      std::uniform_int_distribution<int> dist(0, emptyCells);
      int exitus = dist(gen);
      if (exitus == 0) {
        continue;
      }
      int countEmpty = 0, row = -1, column = -1;
      for (; row != 2 && countEmpty != exitus; ++row) {
        for (column = -1; column != 2 && countEmpty != exitus; ++column) {
          if (wToConst(current).condition(r + row, c + column) ==
                  Condition::Empty ||
              wToConst(current).condition(r + row, c + column) ==
                  Condition::Port ||
              wToConst(current).condition(r + row, c + column) ==
                  Condition::Airport) {
            ++countEmpty;
          }
          if (countEmpty == exitus) break;
        }
        if (countEmpty == exitus) break;
      }
      if (wToConst(current).condition(r + row, c + column) == Condition::Port ||
          wToConst(current).condition(r + row, c + column) ==
              Condition::Airport) {
        movePort(current, wToConst(current).condition(r + row, c + column), r,
                 c);
        continue;
      }

      current.condition(r + row, c + column) = Condition::Booked;
      fromTo.push_back({r, c, r + row, c + column});
    }
  }
  
  for (auto const &point : fromTo) {
    current.condition(point.rT, point.cT) =
        wToConst(current).condition(point.rF, point.cF);
    current.condition(point.rF, point.cF) = Condition::Empty;
  }
}

void evolve(World &current, int day) {
  double beta = current.getBeta(), gamma = current.getGamma(),
         deathRate = current.getDeathRate();
  int resTime = current.getResTime();
  int const nRows = current.getNRows(), nCol = current.getNCol();
  moveCell(current, day);
  World next = current;
  for (int r = 0; r != nRows; ++r) {
    for (int c = 0; c != nCol; ++c) {
      if (wToConst(current).condition(r, c) == Condition::Dead)
        next.condition(r, c) = Condition::Empty;
      if (wToConst(current).condition(r, c) == Condition::Infected &&
          current.Time(r, c) >= resTime) {
        if (probability(gamma)) {
          if (probability(deathRate))
            next.condition(r, c) = Condition::Dead;
          else
            next.condition(r, c) = Condition::Healed;
        }
      } else if (wToConst(current).condition(r, c) == Condition::Susceptible) {
        int nInfected = neighbours(Condition::Infected, current, r, c);
        double betaBis = beta * (nInfected / 8.);
        if (probability(betaBis)) {
          next.condition(r, c) = Condition::Infected;
          next.Time(r, c) = 0;
        }
      }
      ++next.Time(r, c);
    }
  }
  current = next;
}