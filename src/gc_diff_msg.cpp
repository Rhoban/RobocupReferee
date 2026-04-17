#include <bits/types/struct_timeval.h>
#include <stdlib.h>
#include <sys/select.h>
#include <unistd.h>
#include <map>

#include <robocup_referee/referee_client.h>
#include "rhoban_utils/timing/time_stamp.h"
#include "robocup_referee/gc_msg.h"

#include <sstream>
#include <string.h>

using namespace std;
using namespace robocup_referee;


std::string diff_to_string(const GCMsg &prev, const GCMsg &msg) 
{
    std::ostringstream m;
    //m << "version : " << (int)version << " / packetNumber : " << (int)packetNumber ;
    if (prev.playersPerTeam != msg.playersPerTeam)
      m << " / playersPerTeam : " << (int)msg.playersPerTeam << (msg.playersPerTeam==3?" fondation":" advance") ;

    if (prev.competitionType != msg.competitionType){
        m << " / competitionType : " << (int)msg.competitionType ;
        if (msg.competitionType == COMPETITION_TYPE_SMALL)
            m << " (small)";
        else if (msg.competitionType == COMPETITION_TYPE_MIDDLE)
            m << " (middle)";
        else if (msg.competitionType == COMPETITION_TYPE_LARGE)
            m << " (large)";
        m << std::endl;
    }
    if (prev.stopped != msg.stopped)
      m << "stopped : " << (int)msg.stopped << (msg.stopped ? " (true)" : " (false)") << std::endl;
    if (prev.gamePhase != msg.gamePhase){
      m << "gamePhase : " << (int)msg.gamePhase;
      if (msg.gamePhase == GAME_PHASE_NORMAL)
          m << " (normal)";
      else if (msg.gamePhase == GAME_PHASE_PENALTY_SHOOT_OUT)
          m << " (penalty shoot out)";
      else if (msg.gamePhase == GAME_PHASE_EXTRA_TIME)
          m << " (extra time)";
      else if (msg.gamePhase == GAME_PHASE_TIMEOUT)
          m << " (timeout)";
    m << std::endl;
    }
    if (prev.state != msg.state){
      m << "state : " << (int)msg.state ;
      if (msg.state == STATE_INITIAL)
          m << " (initial)";
      else if (msg.state == STATE_READY)
          m << " (ready)";
      else if (msg.state == STATE_SET)
          m << " (set)";
      else if (msg.state == STATE_PLAYING)
          m << " (playing)";
      else if (msg.state == STATE_FINISHED)
          m << " (finished)";
      m << std::endl;
    }

    if (prev.setPlay != msg.setPlay){
      m << "setPlay : " << (int)msg.setPlay ;
      if (msg.setPlay == SET_PLAY_NONE)
          m << " (none)";
      else if (msg.setPlay == SET_PLAY_DIRECT_FREE_KICK)
          m << " (direct free kick)";
      else if (msg.setPlay == SET_PLAY_INDIRECT_FREE_KICK)
          m << " (indirect free kick)";
      else if (msg.setPlay == SET_PLAY_PENALTY_KICK)
          m << " (penalty kick)";
      else if (msg.setPlay == SET_PLAY_THROW_IN)
          m << " (throw in)";
      else if (msg.setPlay == SET_PLAY_GOAL_KICK)
          m << " (goal kick)";
      else if (msg.setPlay == SET_PLAY_CORNER_KICK)
          m << " (corner kick)";
      m<< std::endl;
    }

    if (prev.firstHalf != msg.firstHalf)
        m << "firstHalf : " << (int)msg.firstHalf << std::endl;
    if (prev.kickingTeam != msg.kickingTeam)
      m << "kickingTeam : " << (int)msg.kickingTeam << std::endl;
    if (prev.secsRemaining != msg.secsRemaining)
      m << "secsRemaining : " << msg.secsRemaining << std::endl;
    if (prev.secondaryTime != msg.secondaryTime)
      m << "secondaryTime : " << msg.secondaryTime << std::endl;
    //if (memcmp(prev.teams, msg.teams, sizeof(msg.teams)) != 0){
    if (false){
    for(int t=0; t<2; t++)
    {
        m << "Team " << t << "(msg:"<<(int)msg.teams[t].messageBudget<<")"<<std::endl;
        m << "  teamNumber : " << (int)msg.teams[t].teamNumber ;
        m << "  / colour : " << colorStr((int)msg.teams[t].fieldPlayerColour) ;
        m << "  / goal : " << (int)msg.teams[t].goalkeeper << "/" << colorStr((int)msg.teams[t].goalkeeperColour) << std::endl;
        m << "  score : " << (int)msg.teams[t].score ;
        m << "  / penaltyShot : " << (int)msg.teams[t].penaltyShot ;
        m << "  / singleShots : " << (int)msg.teams[t].singleShots << std::endl;
        m << "substitules: " ;
        for(int p=0; p<MAX_NUM_PLAYERS; p++)
        {
            if (msg.teams[t].players[p].penalty == PENALTY_SUBSTITUTE){
                m << p<<" ";
            }
        }
        m<< std::endl;
        for(int p=0; p<MAX_NUM_PLAYERS; p++)
        {
            if (msg.teams[t].players[p].penalty != PENALTY_SUBSTITUTE){
            m << "  Player " << p+1 ;
            m << " penalty : " << (int)msg.teams[t].players[p].penalty ;
            m << " | secsTillUnpenalized : " << (int)msg.teams[t].players[p].secsTillUnpenalised;
            m << " | warnings : " << (int)msg.teams[t].players[p].warnings;
            m << " | cautions : " << (int)msg.teams[t].players[p].cautions << std::endl;
            }
        }
    }   
  }
    return m.str();
}


/**
 * This just dumps the informations from the referee client
 */
int main()
{
  fd_set rfds;
  char buffer[1024];
  struct timeval tv;
  rhoban_utils::UDPBroadcast broadcast(3838, 3939);
  std::map<std::string, rhoban_utils::TimeStamp> referees;
  rhoban_utils::TimeStamp fullprint=rhoban_utils::TimeStamp::now();
  GCMsg prev;
  while (true)
  {
    if ((rhoban_utils::TimeStamp::now().getTimeSec() - fullprint.getTimeSec()) > 5){
        fullprint = rhoban_utils::TimeStamp::now();
        std::cout<<"[FULL] \n"<<prev.to_string()<<std::endl;
    }
    size_t n = 1024;
    std::string ip;
    FD_ZERO(&rfds);
    FD_SET(0, &rfds);
    tv.tv_sec = 0;
    tv.tv_usec = 1000;
    if (select(1, &rfds, NULL, NULL, &tv  )>0){
      int r;
      if ((r=read(0, buffer, 1024))>0){
        buffer[r]='\0';
       std::cout<<">>> "<<buffer;
      }
    }

    if (broadcast.checkMessage((unsigned char*)buffer, n, &ip))
    {
      referees[ip] = rhoban_utils::TimeStamp::now();
      //system("clear");


      GCMsg msg(buffer, n);
      auto s=diff_to_string(prev, msg);
      if (!s.empty())
          std::cout << s << std::endl;
      prev = msg;
      /*
      for (auto& entry : referees)
      {
        double elapsed = diffMs(referees[ip], rhoban_utils::TimeStamp::now());
        std::cout << entry.first << " (last packet: " << elapsed << ")" << std::endl;
      }
        */
    }
  }
}
