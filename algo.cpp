#include <vector>
#include <iostream>
#include <cassert>
#include <random>
#include <SFML/Graphics.hpp>
#include <ctime>
#include <chrono>
struct Population{
    unsigned long S{}, I{}, R{};
};

enum class State : char { Suscettibile, Infetto, Rimosso};
class Pandemy {
    unsigned int N, T;
    double gamma = 0., beta = 0.;
    Population pop;
    std::vector<State> World;
    unsigned int side;

    public: 
    Pandemy(unsigned int N_in ,unsigned int T_in, double beta_in, double gamma_in): N{N_in}, T{T_in}, gamma{gamma_in}, beta{beta_in}, side{sqrt(N_in)} {}
    void setRand()
    {
        State stato;
        std::vector<int> bonus(36, 0);
        for(unsigned int row = 0; row < side; ++row)
        {
            for(unsigned int col = 0; col < side; ++col)
            {
                std::default_random_engine generator{std::random_device{}()};
                std::uniform_int_distribution<char> distribution(0, 2);
                bonus[row * col + col] = (distribution(generator));
            }
        }
        //World=bonus;
        for(unsigned int row = 0; row < side; ++row)
        {
            for(unsigned int col = 0; col < side; ++col)
            {
                std::cout << static_cast<int> (bonus[row * col + col]);
            }
            std::cout << std::endl;
        }
        for(int i=0;i < bonus.size(); ++i)
        {
            World.push_back(static_cast <State> (bonus[i]));
        }
    }
    int getSide ()
    {
        return side;
    }
    State const& state(unsigned int r, unsigned int c) const
    {
        if(r<side && c <side)
            return World [r * c + c];
        if(r >= side || c >= side)
            return State::Rimosso;
    }
    State& state(unsigned int r, unsigned int c)
    {
        if(r >= side || c >= side)
            return State::Rimosso;
        assert(r < side);
        assert(c < side);
        return World [r * c + c];
    }
    Pandemy infection_graph(Pandemy current)
    {
    std::default_random_engine generator{std::random_device{}()};
    unsigned int N = current.side;
    int a, b;
        std::cout << "Si arriva in fondo a infection graph \n\n";
    Pandemy next = current;
    for(unsigned int r = 0; r < N; ++r) 
        for(unsigned int c = 0; c < N; ++c)
        {
            auto& stato = current.state(r, c);
            short NearI = 0, NearS = 0;
            for (a = (r - 1); a <= (r + 1); ++a)
            {
                for (b = (c - 1); b <= (c + 1); ++b)
                {
                    /*if((current.state(a, b) == 0) && (a != r || b != c))
                    {
                        ++NearS;
                    }*/
                    if((current.state(a, b) == State::Infetto) && (a != r || b != c))
                        ++NearI;
                }
            }
            std::uniform_real_distribution<float> distribution(0, 1);
            next.state(r, c) = current.state(r, c);
            if(NearI != 0)
                if(distribution(generator)> beta)
                    next.state(r, c) = State::Infetto;
            if(stato == State::Infetto)
                if(distribution(generator) > gamma)
                    next.state(r, c) = State::Rimosso;
        }
    return next;
    }
    bool invariant()
    {
        return pop.S + pop.I + pop.R == N;
    }
    std::vector<Population>& infection_txt()
    {
        assert(invariant());
        std::vector<Population> result;
        auto const a = beta * (pop.S/N) * pop.I;
        auto const b = gamma * pop.I;
        for(unsigned int i = 0; i != T; ++i)
        {
        pop.S = - a;
        pop.I = a - b;
        pop.R = b;
        result.push_back(pop);
        assert(invariant());
        }
       //result.push_back(pop);
        return result;
    }
    /*std::vector<std::vector <State>>& infection_graph()
    {
        unsigned int N = current.side();

    int a, b;
    World next {N};
    for(unsigned int r = 0; r < N; ++r) 
        for(unsigned int c = 0; c < N; ++c)
        {
            auto& stato = current.cell(r, c);
            short NearAlive = 0;
            for (a = (r - 1); a <= (r + 1); ++a)
            {
                for (b = (c - 1); b <= (c + 1); ++b)
                {
                    if((current.cell(a, b) == Cell::Alive) && (a != r || b != c))
                    {
                        ++NearAlive;// std::cout << "\n\tdentro\t"<<"a: "<<a<<"\tb: " << b<<"\n\n";
                    }
                }
            }
        //    std::cout << "NearAlive: " << NearAlive << "\tr, c: "<< r << ", " << c << "\n\n";
            next.cell(r, c) = stato;
            if(stato == Cell::Alive)
                if(NearAlive < 2 || NearAlive > 3)
                    next.cell(r, c) = Cell::Dead;
            if(NearAlive == 3)
                next.cell(r, c) = Cell::Alive;
        }
    return next;
    }*/
    friend bool operator==(Pandemy const&, Pandemy const&);
    friend bool operator!=(Pandemy const&, Pandemy const&);
};

bool operator==(Pandemy const& left, Pandemy const& right)
{
    return left.World == right.World;
}
bool operator!=(Pandemy const& left, Pandemy const& right)
{
    return left.World != right.World;
}
void Window(Pandemy& epidemy, int N_Iter, short width = 800, short height = 600)
{
    sf::RenderWindow window(sf::VideoMode(width, height), "Pandemy");
    sf::CircleShape cell(5.f);
    int side = epidemy.getSide();
    for(unsigned int i = 0; i < N_Iter; ++i)
    {
        epidemy = epidemy.infection_graph(epidemy);
        for(unsigned int row = 0; row < side; ++row)
        {
            for (unsigned int col = 0; col < side; ++col)
            {
                cell.setFillColor(sf::Color::Green);
                cell.setPosition((col * 5.) + 3., (row * 5.) + 3.);
                if(epidemy.state(row, col) == State::Suscettibile)
                    cell.setFillColor(sf::Color::Blue);
                else if(epidemy.state(row, col) == State::Infetto)
                    cell.setFillColor(sf::Color::Red);
                //else if(states[row * col + col] == State::Ritirato)
                //    cell.setFillColor(sf::Color::Green);
                window.draw(cell);
            }
        }
        window.display();
    }
    sf::Event chiusura;
    while (window.waitEvent(chiusura))
        if (chiusura.type == sf::Event::Closed)
            window.close();
}
int main()
{
    Pandemy epidemy(36, 12, 45, 21);
    epidemy.setRand();
    Window(epidemy, 12, 1280, 720);
}