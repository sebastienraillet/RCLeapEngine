#include "C_LISTENER.h"
#include "Leap.h"

int main( int argc, char** argv )
{
  C_LISTENER l_listener;
  Controller l_controller;

  l_controller.addListener(l_listener);

  // Keep this process running until Enter is pressed
  std::cout << "Press Enter to quit..." << std::endl;
  std::cin.get();

  // Remove the sample listener when done
  l_controller.removeListener(l_listener);

  return 0;
  
}
