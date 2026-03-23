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
        GCMsg(const char* buffer,int n);
        ~GCMsg();
        
        void update_from_message(const char* buffer,int n);
        std::string to_string() const;
        int getLastUpdate() const;
        TimeStamp last_update;

        bool hasTeam(int teamId) const;
        const TeamInfo& getTeam(int teamNumber) const;
};


std::string colorStr(uint8_t color);
}