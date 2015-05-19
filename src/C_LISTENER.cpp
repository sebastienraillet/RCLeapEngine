#include "C_LISTENER.h"

C_LISTENER::C_LISTENER():
  Listener()
  ,m_socket()
{}

C_LISTENER::~C_LISTENER()
{}

void C_LISTENER::onInit(const Controller &p_controller)
{}

void C_LISTENER::onConnect(const Controller &p_controller)
{
  if ( false == m_socket.Init() )
  {
    std::cout << "Impossible to initialize the socket" << std::endl;
  }
  else
  {
    std::cout << "The socket is initialized" << std::endl;
    if ( false == m_socket.Connect() )
    {
      std::cout << "Impossible to connect the socket" << std::endl;
    }
    else
    {
      std::cout << "The socket is connected" << std::endl;
    }
  }
}

void C_LISTENER::onDisconnect(const Controller &p_controller)
{
  m_socket.Disconnect();
}

void C_LISTENER::onExit(const Controller &p_controller)
{}

void C_LISTENER::onFrame(const Controller &p_controller)
{
  float l_thumb_y_position, l_pinky_y_position = 0;
  const Frame frame = p_controller.frame();
  std::string l_string = "{\"vitesse\":50,\"direction\":50}";
  m_socket.Send(l_string); 

  /*std::cout << "Frame id: " << frame.id()
    << ", timestamp: " << frame.timestamp()
    << ", hands: " << frame.hands().count()
    << ", extended fingers: " << frame.fingers().extended().count()
    << ", tools: " << frame.tools().count()
    << ", gestures: " << frame.gestures().count() << std::endl;*/

  HandList hands = frame.hands();
  for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl) 
  {
    // Get the first hand
    const Hand hand = *hl;
    std::cout << "Hand position z axis : " << hand.palmPosition().z << std::endl;

    // Get fingers
    const FingerList fingers = hand.fingers();
    for (FingerList::const_iterator fl = fingers.begin(); fl != fingers.end(); ++fl) 
    {
      const Finger finger = *fl;
      if ( Finger::TYPE_THUMB == finger.type() )
      {
        Bone::Type boneType = static_cast<Bone::Type>(Bone::TYPE_DISTAL);
        Bone bone = finger.bone(boneType);
        l_thumb_y_position = bone.nextJoint().y;
        std::cout << "Thumb distal bone y position : " << l_thumb_y_position << std::endl;
      }
      else if ( Finger::TYPE_PINKY == finger.type() )
      {
        Bone::Type boneType = static_cast<Bone::Type>(Bone::TYPE_DISTAL);
        Bone bone = finger.bone(boneType);
        l_pinky_y_position = bone.nextJoint().y;
        std::cout << "Pinky distal bone y position : " << l_pinky_y_position << std::endl;
      }
    }
  }
}

void C_LISTENER::onFocusGained(const Controller &p_controller)
{}

void C_LISTENER::onFocusLost(const Controller &p_controller)
{}

void C_LISTENER::onDeviceChange(const Controller &p_controller)
{}

void C_LISTENER::onServiceConnect(const Controller &p_controller)
{}

void C_LISTENER::onServiceDisconnect(const Controller &p_controller)
{}

