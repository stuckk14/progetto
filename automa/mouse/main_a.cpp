#include <chrono>
#include <iostream>
#include <random>
#include <thread>
#include "automa_mouse.hpp"

/*void print(std::ostream &os, World const &world)
{
    constexpr auto clear = "\033[2J";
    const auto N = world.size();

    os << clear;
    os << '+' << std::string(N, '-') << "+\n";
    for (int r = 0; r != N; ++r)
    {
        os << '|';
        for (int c = 0; c != N; ++c)
        {
            switch (world.condition(r, c))
            {
            case Condition::Infected:
                std::cout << '*';
                break;
            case Condition::Removed:
                std::cout << 'x';
                break;
            default:
                std::cout << ' ';
                break;
            }
        }
        os << "|\n";
    }
    os << '+' << std::string(N, '-') << "+\n";
}
*/
int main()
{
    constexpr int world_size = 50;
    double beta = 0.5, gamma = 0.3, deathRate = 0.02, lockdownLimit = 0.25;
    int daysToDeath = 3, nVaccinati = 0;
    World world(world_size, beta, gamma, deathRate, lockdownLimit, daysToDeath, nVaccinati);

    {
        std::default_random_engine eng{std::random_device{}()};
        std::uniform_int_distribution<int> dist{0, world_size - 1};

       /* for (int i = 0; i != world_size * world_size / 5; ++i)
        {
            auto r = dist(eng);
            auto c = dist(eng);
            for (; world.getCondition(r, c) == Condition::Infected;
                r = dist(eng), c = dist(eng));
            world.setCondition(r, c) = Condition::Infected;
        }
        for (int i = 0; i != world_size * world_size / 10; ++i)
        {
            auto r = dist(eng);
            auto c = dist(eng);
            for (; world.getCondition(r, c) == Condition::Empty; r = dist(eng), c = dist(eng));
            world.setCondition(r, c) = Condition::Empty;
        }*/
    }
    std::cerr<<"lockdown limit: " << world.getLockdownLimit();
    Window(10000, world, world_size, 900, 900);
    /*
    for (int i = 0; i != 200; ++i)
    {
        world = evolve(world);
        print(std::cout, world);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }*/
}