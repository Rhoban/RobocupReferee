#pragma once
#include "RoboCupGameControlData.h"
#include "robocup_referee/time_stamp.h"
#include <string>

namespace robocup_referee
{
class GCMsg : public RoboCupGameControlData
{
public:
  GCMsg();
  GCMsg(const char* buffer, int n);
  ~GCMsg();

  void update_from_message(const char* buffer, unsigned int n);
  std::string to_string() const;
  double getLastUpdate() const;
  TimeStamp last_update;

  bool hasTeam(int teamId) const;
  const TeamInfo& getTeam(int teamNumber) const;
  enum FreeKickType
  {
    None,
    Indirect,
    Direct,
  };
  FreeKickType freeKick() const;
};

const char* colorStr(uint8_t color);
const char* stateStr(uint8_t state);
const char* setPlayStr(uint8_t setPlay);
}  // namespace robocup_referee
