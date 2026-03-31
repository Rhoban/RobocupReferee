#include "gc_msg.h"
#include "robocup_referee/RoboCupGameControlData.h"
#include <cstdint>
#include <string.h>
#include <sstream>
#include <iostream>

namespace robocup_referee
{
GCMsg::GCMsg()
{
    memset(header,0,sizeof(RoboCupGameControlData));
}

GCMsg::GCMsg(const char* buffer,int n)
{
    update_from_message(buffer,n);
}

GCMsg::~GCMsg()
{
}

int GCMsg::getLastUpdate() const{
    return last_update.elapsed_time();
}

void GCMsg::update_from_message(const char* buffer,int n)
{
    //memcpy(header,buffer,sizeof(header));
    //  uint8_t* data = (uint8_t*)(buffer + sizeof(header));
    //int i=0;
    //std::cout<<sizeof(RoboCupGameControlData)<<" vs "<<n<<std::endl;
    memcpy(header,buffer,sizeof(RoboCupGameControlData));
    last_update.update();
}

bool GCMsg::hasTeam(int teamId) const{
    for(int t=0; t<2; t++)
    {
        if(teams[t].teamNumber == teamId)
            return true;
    }
    return false;
}


const TeamInfo& GCMsg::getTeam(int teamNumber) const {
    if (teams[0].teamNumber == teamNumber)
        return teams[0];
    return teams[1];
}

std::string colorStr(uint8_t color)
{
    switch(color)
    {
case TEAM_BLUE:   return "blue";
    break;
case TEAM_RED:    return "red";
    break;
case TEAM_YELLOW: return "yellow";
    break;
case TEAM_BLACK:  return "black";
    break;
case TEAM_WHITE:  return "white";
    break;
case TEAM_GREEN:  return "green";
    break;
case TEAM_ORANGE: return "orange";
    break;
case TEAM_PURPLE: return "purple";
    break;
case TEAM_BROWN:  return "brown";
    break;
case TEAM_GRAY:   return "gray";
    break;
    }
    return "?";
}

std::string GCMsg::to_string() const
{
    std::ostringstream m;
    m << "version : " << (int)version << " / packetNumber : " << (int)packetNumber << std::endl;
    m << "playersPerTeam : " << (int)playersPerTeam << (playersPerTeam==3?" fondation":" advance") ;

    m << " / competitionType : " << (int)competitionType ;
    if (competitionType == COMPETITION_TYPE_SMALL)
        m << " (small)";
    else if (competitionType == COMPETITION_TYPE_MIDDLE)
        m << " (middle)";
    else if (competitionType == COMPETITION_TYPE_LARGE)
        m << " (large)";
    m << std::endl;

    m << "stopped : " << (int)stopped <<(stopped?" (true)":" (false)")<< std::endl;
    m << "gamePhase : " << (int)gamePhase ;
    if (gamePhase == GAME_PHASE_NORMAL)
        m << " (normal)";
    else if (gamePhase == GAME_PHASE_PENALTY_SHOOT_OUT)
        m << " (penalty shoot out)";
    else if (gamePhase == GAME_PHASE_EXTRA_TIME)
        m << " (extra time)";
    else if (gamePhase == GAME_PHASE_TIMEOUT)
        m << " (timeout)";
    m << std::endl;

    m << "state : " << (int)state ;
    if (state == STATE_INITIAL)
        m << " (initial)";
    else if (state == STATE_READY)
        m << " (ready)";
    else if (state == STATE_SET)
        m << " (set)";
    else if (state == STATE_PLAYING)
        m << " (playing)";
    else if (state == STATE_FINISHED)
        m << " (finished)";
    m << std::endl;

    m << "setPlay : " << (int)setPlay ;
    if (setPlay == SET_PLAY_NONE)
        m << " (none)";
    else if (setPlay == SET_PLAY_DIRECT_FREE_KICK)
        m << " (direct free kick)";
    else if (setPlay == SET_PLAY_INDIRECT_FREE_KICK)
        m << " (indirect free kick)";
    else if (setPlay == SET_PLAY_PENALTY_KICK)
        m << " (penalty kick)";
    else if (setPlay == SET_PLAY_THROW_IN)
        m << " (throw in)";
    else if (setPlay == SET_PLAY_GOAL_KICK)
        m << " (goal kick)";
    else if (setPlay == SET_PLAY_CORNER_KICK)
        m << " (corner kick)";
    m<< std::endl;

    m << "firstHalf : " << (int)firstHalf << std::endl;
    m << "kickingTeam : " << (int)kickingTeam << std::endl;
    m << "secsRemaining : " << secsRemaining << std::endl;
    m << "secondaryTime : " << secondaryTime << std::endl;
    for(int t=0; t<2; t++)
    {
        m << "Team " << t << std::endl;
        m << "  teamNumber : " << (int)teams[t].teamNumber << std::endl;
        m << "  fieldPlayerColour : " << colorStr((int)teams[t].fieldPlayerColour) << std::endl;
        m << "  goalkeeperColour : " << colorStr((int)teams[t].goalkeeperColour) << std::endl;
        m << "  goalkeeper :" << (int)teams[t].goalkeeper << std::endl;
        m << "  score : " << (int)teams[t].score << std::endl;
        m << "  penaltyShot : " << (int)teams[t].penaltyShot << std::endl;
        m << "  singleShots : " << (int)teams[t].singleShots << std::endl;
        m << "  messageBudget : " << (int)teams[t].messageBudget << std::endl;
        for(int p=0; p<playersPerTeam; p++)
        {
            m << "  Player " << p+1 ;
            m << " penalty : " << (int)teams[t].players[p].penalty ;
            m << " | secsTillUnpenalized : " << (int)teams[t].players[p].secsTillUnpenalised;
            m << " | warnings : " << (int)teams[t].players[p].warnings;
            m << " | cautions : " << (int)teams[t].players[p].cautions << std::endl;
        }
    }   
    return m.str();
}

} // namespace robocup_referee