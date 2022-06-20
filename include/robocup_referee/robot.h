/*****************************************************************************/
/*! \file    Robot.h
 *  \author  LH
 *  \date    2011-05-26
 *
 *  \brief   Robot informations sent by the referee box
 *	This class is used to store the robot informations
 *****************************************************************************/
#pragma once

#include <iostream>

#include "robocup_referee/constants.h"
#include "robocup_referee/utilities.h"

namespace robocup_referee
{
class Robot
{
public:
  Robot();
  ~Robot();

  int getPenalty() const;
  int getSecsTillUnpenalised() const;
  int getWarningsCount() const;
  int getYellowCardCount() const;
  int getRedCardCount() const;
  bool isGoalKeeper() const;

  /*! \brief Update the robot from a referee box message */
  void update_from_message(char const* message, int numRobot);

private:
  int m_penalty;
  int m_secs_till_unpenalised;
  int m_warnings_count;
  int m_yellow_card_count;
  int m_red_card_count;
  bool m_goal_keeper;
};

std::ostream& operator<<(std::ostream& flux, Robot const& myRobot);

}  // namespace robocup_referee
