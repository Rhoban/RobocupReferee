#include <stdlib.h>
#include <unistd.h>

#include <robocup_referee/referee_client.h>
#include "rhoban_utils/timing/time_stamp.h"

using namespace std;
using namespace robocup_referee;

/**
 * This just dumps the informations from the referee client
 */
int main()
{
  rhoban_utils::UDPBroadcast broadcast(3838, 3939);
  std::map<std::string, rhoban_utils::TimeStamp> referees;

  while (true)
  {
    char buffer[1024];
    size_t n = 1024;
    std::string ip;

    if (broadcast.checkMessage((unsigned char*)buffer, n, &ip))
    {
      referees[ip] = rhoban_utils::TimeStamp::now();

      system("clear");
      std::cout << "Game controllers:" << std::endl;
      for (auto& entry : referees)
      {
        double elapsed = diffMs(referees[ip], rhoban_utils::TimeStamp::now());
        std::cout << entry.first << " (last packet: " << elapsed << ")" << std::endl;
      }
    }
  }
}
