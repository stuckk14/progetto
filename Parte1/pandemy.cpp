#include "pandemy1.hpp"

#include <cassert>
#include <cmath>

bool Pandemy::invariant() {
  return std::round(pop.S + pop.I + pop.R) == N && pop.S >= 0 && pop.I >= 0 &&
         pop.R >= 0;
}

Population Pandemy::getState() const { return pop; }
void Pandemy::evolve() {
  assert(invariant());
  auto const coeff = beta * (pop.S / N);
  auto const IOld = pop.I;
  pop.R = pop.R + gamma * IOld;
  pop.I = IOld + coeff * IOld - gamma * IOld;
  pop.S = pop.S - coeff * IOld;
  ++T;
  assert(invariant());
}