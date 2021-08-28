#ifndef PANDEMY_HPP
#define PANDEMY_HPP

struct Population {
  double S{}, I{}, R{};
};

class Pandemy {
  int N{};
  int T;
  double gamma = 0., beta = 0.;
  Population pop;

 public:
  Pandemy(int NIn, int I, int TIn, double betaIn, double gammaIn)
      : N{NIn},
        T{TIn},
        gamma{gammaIn},
        beta{betaIn},
        pop{static_cast<double>(NIn - I), static_cast<double>(I), 0} {}

  bool invariant();

  Population getState() const;

  void evolve();
};
#endif