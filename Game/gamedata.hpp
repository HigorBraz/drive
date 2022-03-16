#ifndef GAMEDATA_HPP_
#define GAMEDATA_HPP_

#include <bitset>

enum class Input { Right, Left, Down, Up, Space };
enum class State { Menu, Playing, Win, Lose };

struct GameData {
  State m_state{State::Menu};
  std::bitset<5> m_input;  // [up, down, left, right, Space]
};

#endif
