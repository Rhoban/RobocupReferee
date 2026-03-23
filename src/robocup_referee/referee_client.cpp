/*
 * RefereeClient.cpp
 *
 *  Created on: 5 juil. 2011
 *      Author: hugo
 */
#include "robocup_referee/referee_client.h"

#include <unistd.h>
#include <sys/types.h>

#include <stdio.h>
#include <stdlib.h>

#include "robocup_referee/time_stamp.h"
#include "rhoban_utils/sockets/udp_unicast.h"
#include "robocup_referee/gc_msg.h"

namespace robocup_referee
{

RefereeClient::~RefereeClient()
{
  _mustQuit = true;
}

void RefereeClient::start()
{
  _myTeamId = 0;
  _myId = 0;
  _mustQuit = false;
  thread = new std::thread(&RefereeClient::execute, this);
}

bool RefereeClient::isIPValid(const std::string &ip)
{
  return true;
}


void RefereeClient::tick(){}

void RefereeClient::execute(void)
{
  rhoban_utils::UDPBroadcast broadcast(_portNo, _portSend);
  rhoban_utils::UDPUnicast send_unicast;
  std::cout << "Referee Client started" << std::endl;

  char buffer[1024];
  TimeStamp last;
  std::string gamecontroller_ip = "";

  while (_mustQuit == false)
  {
    usleep(1000);
    size_t n = 1024;
    std::string ip;
    if (broadcast.checkMessage((unsigned char*)buffer, n, &ip))
    {
      if (isIPValid(ip))
      {
        gamecontroller_ip = ip;
        buffer[n] = '\0';
        _gamedata.update_from_message(buffer,n);
      }
    }

    // Answering referee at 1hz
    if (_myId != 0 && last.elapsed_time() > 100 && gamecontroller_ip != "")
    {
      last=TimeStamp();
      RoboCupGameControlReturnData m;
      m.teamNum = _myTeamId;
      m.playerNum = _myId;
      m.fallen=0;
      // other field to set: ball / pose / balle age
    
      send_unicast.send(gamecontroller_ip, _portSend, (unsigned char *)&m, sizeof(m));
    }
    tick(); // subclass tick call
  }
}

void RefereeClient::setState(uint8_t teamId, uint8_t myId, uint8_t message)
{
  _myTeamId = teamId;
  _myId = myId;
  _message = message ;
}

GCMsg& RefereeClient::getGameState()
{
  return _gamedata;
}

}  // namespace robocup_referee
