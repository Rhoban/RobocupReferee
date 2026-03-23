#include <stdlib.h>
#include <unistd.h>

#include <robocup_referee/referee_client.h>

using namespace std;
using namespace robocup_referee;

/**
 * This just dumps the informations from the referee client
 */
int main()
{
  RefereeClient client;
  client.start();

  while (true)
  {
    if (system("clear"))
    {
      std::cerr << "Failed to clear terminal" << std::endl;
      exit(EXIT_FAILURE);
    }
    client.setState(11, 1, 0);
    std::cout << client.getGameState().to_string() << std::endl;
    sleep(1);
  }

  return 0;
}
