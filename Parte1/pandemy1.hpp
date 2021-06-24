#ifndef PANDEMY1_HPP
#define PANDEMY1_HPP

struct Population
{
    double S{}, I{}, R{};
};

class Pandemy
{
    int N{};
    int T;
    double gamma = 0., beta = 0.;
    Population pop;

public:
    Pandemy(int N_in, int I, int T_in, double beta_in, double gamma_in) : N{N_in}, T{T_in}, gamma{gamma_in}, beta{beta_in},
                                                                          pop{static_cast<double>(N_in - I), static_cast<double>(I), 0} {}

    bool invariant();

    Population GetState() const;

    void evolve();
    //result.push_back(pop);
};
#endif