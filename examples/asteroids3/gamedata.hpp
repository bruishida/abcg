#ifndef GAMEDATA_HPP_
#define GAMEDATA_HPP_

#include <bitset>

enum class Input { Right, Left, JumpUp, JumpDown, Fire };
enum class State { Playing, GameOver, Win, Ready, Go };

struct GameData {
  State m_state{State::Playing};
  std::bitset<5> m_input;  // [jumpup, jumpdown, left, right, fire]
};

#endif