#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "pandemy.hpp"

TEST_CASE("Test SIR 1") {
  Population state;
  int popTot = 1000, initialInfected = 30, T = 30;
  double beta = 0.5, gamma = 0.3;
  Pandemy pan{popTot, initialInfected, T, beta, gamma};
  for (int i = 0; i < T; ++i) pan.evolve();
  state = pan.getState();
  CHECK(state.I == doctest::Approx(22).epsilon(.03));
  CHECK(state.R == doctest::Approx(667).epsilon(.01));
  CHECK(state.S == doctest::Approx(311).epsilon(.01));
}
TEST_CASE("Test SIR 2") {
  Population state;
  int popTot = 300, initialInfected = 45, T = 78;
  double beta = 0.9, gamma = 0.2;
  Pandemy pan{popTot, initialInfected, T, beta, gamma};
  for (int i = 0; i < T; ++i) pan.evolve();
  state = pan.getState();
  CHECK(state.I == doctest::Approx(0).epsilon(.01));
  CHECK(state.R == doctest::Approx(299).epsilon(.01));
  CHECK(state.S == doctest::Approx(1).epsilon(.05));
}
TEST_CASE("Test SIR 3") {
  Population state;
  int popTot = 15000000, initialInfected = 20, T = 150;
  double beta = 0.85, gamma = 0.3;
  Pandemy pan{popTot, initialInfected, T, beta, gamma};
  for (int i = 0; i < T; ++i) pan.evolve();
  state = pan.getState();
  CHECK(state.I == doctest::Approx(0).epsilon(.01));
  CHECK(state.R == doctest::Approx(14221230).epsilon(.000001));
  CHECK(state.S == doctest::Approx(778767).epsilon(.0001));
}