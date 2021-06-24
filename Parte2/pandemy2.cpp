#include "pandemy2.hpp"
constexpr Condition wall = Condition::Wall;
inline bool operator==(State const &left, State const &right)
{
    return left.condition == right.condition && left.time == right.time;
}

double World::get_beta() const noexcept
{
    return beta;
}
void World::setLockdown(bool command) noexcept
{
    if (command)
        beta /= 2.;
    else
        beta *= 2.;
}

double World::get_gamma() const noexcept
{
    return gamma;
}

double World::get_deathRate() const noexcept
{
    return deathRate;
}
int World::get_res_time() const noexcept
{
    return resTime;
}
double World::getLockdownLimit() const noexcept
{
    return lockdownLimit;
}
int World::getNVaccinati() const noexcept
{
    return nVaccinati;
}
int World::getNRighe() const noexcept
{
    return n_righe;
}
int World::getNCol() const noexcept
{
    return n_col;
}
int World::size() const noexcept
{
    return n_col * n_righe;
}

Condition const &World::getCondition(int r, int c) const noexcept
{
    if (r < 0 || c < 0 || r >= n_righe || c >= n_col)
    {
        return wall;
    }
    auto const index = r * n_col + c;
    return m_grid[index].condition;
}

Condition &World::setCondition(int r, int c) noexcept
{
    assert(r >= 0 && r < n_righe && c >= 0 && c < n_col);
    auto const index = r * n_col + c;
    assert(index >= 0 && index < static_cast<int>(m_grid.size()));
    return m_grid[index].condition;
}
inline int &World::Time(int r, int c) noexcept
{
    auto const index = r * n_col + c;
    return m_grid[index].time;
}
/*int const &World::Time(int r, int c) const noexcept
{
    auto const index = r * n_col + c;
    return m_grid[index].time;
}*/

inline bool operator==(World const &l, World const &r)
{
    return l.m_grid == r.m_grid;
}
//FINE DELLA CLASSE!

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
            if (current.getCondition(r, c) == porto && neighbours<Condition::Empty>(current, r, c) != 0)
            {
                int row = -1, column;
                for (; row != 2; ++row)
                    for (column = -1; column != 2; ++column)
                        if (current.getCondition(r + row, c + column) == Condition::Empty)
                        {
                            destCell.push_back(point{r + row, c + column});
                            ++i;
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
    if (current.getCondition(destCell[arriveCell].r, destCell[arriveCell].c) == porto || current.getCondition(oldR, oldC) == porto)
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
            if (current.getCondition(r, c) == Condition::Wall || empty_cells == 0 || current.getCondition(r, c) == Condition::Desert || current.getCondition(r, c) == Condition::Empty || current.getCondition(r, c) == Condition::Dead)
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
void evolve(World &current, int day)
{
    //std::cerr << "day: " <<day <<"\n\n\n";
    double beta = current.get_beta(), gamma = current.get_gamma(), deathRate = current.get_deathRate();
    int resTime = current.get_res_time();
    int const n_righe = current.getNRighe(), n_col = current.getNCol();
    move_cell(current, day);
    World next = current;
    //World next(n_righe, n_col, beta, gamma, deathRate, current.getLockdownLimit(), resTime, current.getNVaccinati());
    for (int r = 0; r != n_righe; ++r)
    {
        for (int c = 0; c != n_col; ++c)
        {
            //next.setCondition(r, c) = current.getCondition(r, c);
            //next.Time(r, c) = current.Time(r, c);
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
    current = next;
}
