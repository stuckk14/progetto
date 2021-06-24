#include "pandemy1.hpp"
#include <cmath>
#include <cassert>

bool Pandemy::invariant()
    {
        return std::round(pop.S + pop.I + pop.R) == N && pop.S >= 0 && pop.I >= 0 && pop.R >= 0;
    }
    
Population Pandemy::GetState() const
    {
        return pop;
    }
void Pandemy::evolve()
    {
        assert(invariant());
        auto const coeff = beta * (pop.S / N);
        auto const I_old = pop.I;
        pop.R = pop.R + gamma * I_old;
        pop.I = I_old + coeff * I_old - gamma * I_old;
        pop.S = pop.S - coeff * I_old;
        ++T;
        assert(invariant());
    }