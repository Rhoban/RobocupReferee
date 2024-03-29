/*****************************************************************************/
/*! \file    GameState.cpp
 *  \author  LH
 *  \date    2011-05-26
 *
 *  \brief   Game informations sent by the referee box
 *****************************************************************************/
#include "robocup_referee/game_state.h"
#include "robocup_referee/referee_client.h"

#include <iostream>

using namespace std;

namespace robocup_referee
{
/* The value -1 is used to know if the wireless is working */
GameState::GameState()
{
  m_struct_version = -1;
  m_num_player = -1;
  m_actual_game_state = -1;
  m_first_half = -1;
  m_kick_off_team = -1;
  m_sec_game_state = -1;
  m_drop_in_team = -1;
  m_drop_in_time = -1;
  m_estimated_secs = -1;
  m_secondary_team = -1;
  m_secondary_mode = -1;
  forcePresent = false;
}

GameState::~GameState()
{
}


/* Use a broadcasted message to update the GameState */
bool GameState::update_from_message(char const* message)
{
  m_last_game_state_update.update();
  int struct_version = chars_to_int(message, 4, 6);

  if (struct_version != RefereeClient::protocolVersion)
  {
    std::cerr << "Game controller bad version " << struct_version << std::endl;
    return false;
  }

  m_struct_version = struct_version;
  m_num_player = chars_to_int(message, 7, 8);
  m_game_type = chars_to_int(message, 8, 9);
  m_actual_game_state = chars_to_int(message, 9, 10);
  m_first_half = chars_to_int(message, 10, 11);
  m_kick_off_team = chars_to_int(message, 11, 12);
  m_sec_game_state = chars_to_int(message, 12, 13);
  m_secondary_team = chars_to_int(message, 13, 14);
  m_secondary_mode = chars_to_int(message, 14, 15);

  m_drop_in_team = chars_to_int(message, 17, 18);
  m_drop_in_time = chars_to_int(message, 18, 20);
  m_estimated_secs = chars_to_int(message, 20, 22);
  m_secondary_secs = chars_to_int(message, 22, 24);

  for (int i = 0; i < NB_TEAMS; i++)
    m_team[i].update_from_message(message + 24, i);
  return true;
}

void GameState::set_params(int team1_id, int team2_id, int nb1, int nb2)
{
  m_team[0].set_params(team1_id, nb1);
  m_team[1].set_params(team2_id, nb2);
}
  

/* GETTERS */
int GameState::getLastUpdate() const
{
  return forcePresent ? 0 : m_last_game_state_update.elapsed_time();
}

int GameState::getStructVersion() const
{
  return m_struct_version;
}

int GameState::getGameType() const
{
  return m_game_type;
}

int GameState::getNumPlayer() const
{
  return m_num_player;
}

int GameState::getActualGameState() const
{
  return m_actual_game_state;
}

int GameState::getFirstHalf() const
{
  return m_first_half;
}

int GameState::getKickOffTeam() const
{
  return m_kick_off_team;
}

int GameState::getSecGameState() const
{
  return m_sec_game_state;
}

int GameState::getSecondaryTeam() const
{
  return m_secondary_team;
}

int GameState::getSecondaryMode() const
{
  return m_secondary_mode;
}

int GameState::getDropInTeam() const
{
  return m_drop_in_team;
}

int GameState::getDropInTime() const
{
  return m_drop_in_time;
}

int GameState::getEstimatedSecs() const
{
  return m_estimated_secs;
}

int GameState::getSecondarySecs() const
{
  return m_secondary_secs;
}

int GameState::getNbTeam() const
{
  return NB_TEAMS;
}

const Team& GameState::getTeam(int teamNumber) const
{
  return m_team[teamNumber];
}

const bool GameState::hasTeam(int teamId) const
{
  for (int k = 0; k < getNbTeam(); k++)
  {
    const Team& team = getTeam(k);
    if (team.getTeamNumber() == teamId)
      return true;
  }
  return false;
}

const Team& GameState::getTeamById(int teamId) const
{
  for (int k = 0; k < getNbTeam(); k++)
  {
    const Team& team = getTeam(k);
    if (team.getTeamNumber() == teamId)
      return m_team[k];
  }

  throw std::runtime_error("teamid does not exist");
}

ostream& operator<<(ostream& flux, GameState const* gs)
{
  flux << "time since last update : ";
  flux << gs->getLastUpdate() << endl;
  flux << "struct_version : " << gs->getStructVersion() << endl;
  flux << "num_player : " << gs->getNumPlayer() << endl;
  flux << "gameState : " << gs->getActualGameState() << endl;
  flux << "first_half : " << gs->getFirstHalf() << endl;
  flux << "kick_off_team : " << gs->getKickOffTeam() << endl;
  flux << "sec_game_state : " << gs->getSecGameState() << endl;
  flux << "sec_game_mode : " << gs->getSecondaryMode() << endl;
  flux << "sec_game_team : " << gs->getSecondaryTeam() << endl;
  flux << "secondary_secs : " << gs->getSecondarySecs() << endl;
  flux << "drop_in_team : " << gs->getDropInTeam() << endl;
  flux << "drop_in_time : " << gs->getDropInTime() << endl;
  flux << "estimated_secs : " << gs->getEstimatedSecs() << endl;
  for (int i = 0; i < NB_TEAMS; i++)
  {
    flux << "Team " << i << std::endl;
    flux << gs->getTeam(i);
  }
  return flux;
}

void GameState::set_game_state(int actual_game_state)
{
  forcePresent = true;
  m_actual_game_state = actual_game_state;
  m_last_game_state_update.update();
}

void GameState::set_kickoff_team(int kick_off_team)
{
  forcePresent = true;
  m_kick_off_team = kick_off_team;
  m_last_game_state_update.update();
}

void GameState::set_secondary_mode(int second_state, int secondary_team, int secondary_secs)
{
  forcePresent = true;
  m_secondary_secs = secondary_secs;
  m_secondary_team = secondary_team;
  m_secondary_secs = secondary_secs;
  m_last_game_state_update.update();
}

void GameState::set_penalized(int team_id, int player_id, bool is_penalized)
{
  forcePresent = true;
  int id = player_id;
  for (int i = 0; i < NB_TEAMS; i++)
  {
    auto& team = m_team[i];
    if (team_id == team.getTeamNumber() && 0 <= id && id < team.getNbRobots())
    {
      Robot* robot = team.getRobotRef(id);
      robot->set_penalized(is_penalized);
    }
  }
}

}  // namespace robocup_referee
