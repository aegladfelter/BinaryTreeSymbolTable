#ifndef _UTILITIES_HPP
#define _UTILITIES_HPP 1

#include <iostream>
#include <algorithm>
#include <map>
#include <random>
#include <string>

class Utilities
{
  private:
      static std::default_random_engine       _generator;
      static std::uniform_int_distribution<int> _distribution;

  public:

    // Given a vector of values, randomly permute
    template <typename T>
    static void shuffle(std::vector<T>& vec)
    {
        std::random_shuffle(vec.begin(), vec.end());
    }

    // Populate a vector with integer values [low, high)
    static std::vector<int> populate(int low, int high)
    {
        std::vector<int> vec;

        for (int i = low; i < high; i++)
        {
            vec.push_back(i);
        }

        return vec;
    }

    // Populate a vector with random values NOT in [low, high); resulting vector should be size sz
    static std::vector<int> populateRandomComplement(int low, int high, int sz)
    {
        std::vector<int> vec;

        while (vec.size() != sz)
        {
            int value = _distribution(_generator);
            if (value < low || value >= high)
            {
                std::vector<int>::const_iterator it = std::find(vec.begin(), vec.end(), value);
                if (it == vec.end())
                {
                    vec.push_back(value);
                }
            }
        }

        return vec;
    }

    // Populate a vector with integer values [low, high); randomly permute
    static std::vector<int> rPopulate(int low, int high)
    {
        std::vector<int> vec = populate(low, high);
        shuffle(vec);
        return vec;
    }

    // Return a randomly generated value
    static int generate()
    {
        return _distribution(_generator);
    }
};

#endif