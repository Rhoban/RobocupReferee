/*****************************************************************************/
/*! \file    Robot.cpp
 *  \author  LH
 *  \date    2011-05-26
 *
 *  \brief   Robot informations sent by the referee box
 *****************************************************************************/
#include "robocup_referee/robot.h"

using namespace std;

namespace robocup_referee
{
Robot::Robot()
{
  m_penalty = 0;
  m_secs_till_unpenalised = 0;
}

Robot::~Robot()
{
}

int Robot::getPenalty() const
{
  return m_penalty;
}

int Robot::getSecsTillUnpenalised() const
{
  return m_secs_till_unpenalised;
}

int Robot::getWarningsCount() const
{
  return m_warnings_count;
}

int Robot::getYellowCardCount() const
{
  return m_yellow_card_count;
}

int Robot::getRedCardCount() const
{
  return m_red_card_count;
}

bool Robot::isGoalKeeper() const
{
  return m_goal_keeper;
}

/* Use a broadcasted message to update the Robot */
void Robot::update_from_message(char const* message, int numRobot)
{
  int d = Constants::nb_chars_by_robot * numRobot;  // decalage
  m_penalty = chars_to_int(message, d + 0, d + 1);
  m_secs_till_unpenalised = chars_to_int(message, d + 1, d + 2);
  m_warnings_count = chars_to_int(message, d + 2, d + 3);
  m_yellow_card_count = chars_to_int(message, d + 3, d + 4);
  m_red_card_count = chars_to_int(message, d + 4, d + 5);
  m_goal_keeper = (chars_to_int(message, d + 5, d + 6) != 0);
}

void Robot::set_penalized(bool is_penalized)
{
  m_penalty = is_penalized ? Constants::PENALTY_SUBSTITUTE : Constants::PENALTY_NONE;
  m_secs_till_unpenalised = is_penalized ? 30 : 0;
}

ostream& operator<<(ostream& flux, const Robot& r)
{
  flux << "\t\tpenalty : " << r.getPenalty() << endl;
  flux << "\t\tsecs_till_unpenalised : " << r.getSecsTillUnpenalised() << endl;
  flux << "\t\twarnings_count : " << r.getWarningsCount() << endl;
  flux << "\t\tyellow_card_count : " << r.getYellowCardCount() << endl;
  flux << "\t\tred_card_count : " << r.getRedCardCount() << endl;
  flux << "\t\tgoal_keeper : " << (r.isGoalKeeper() ? "yes" : "no") << endl;
  return flux;
}

}  // namespace robocup_referee
