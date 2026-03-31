#include <stdlib.h>
#include <unistd.h>

#include <robocup_referee/referee_client.h>

using namespace std;
using namespace robocup_referee;

/**
 * This just dumps the informations from the referee client
 */
int main(int argc, char** argv)
{
  RefereeClient client;
  int teamId = 11;
  int id = 1;
  client.start();
  if (argc>1) teamId = atoi(argv[1]);
  if (argc>2) id = atoi(argv[2]);

  while (true)
  {
    if (system("clear"))
    {
      std::cerr << "Failed to clear terminal" << std::endl;
      exit(EXIT_FAILURE);
    }
    client.setState(teamId, id, 0);
    std::cout << client.getGameState().to_string() << std::endl;
    sleep(1);
  }

  return 0;
}
