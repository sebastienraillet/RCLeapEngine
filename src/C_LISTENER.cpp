#include "C_LISTENER.h"

// Coordinate define used for the map function
#define COORDINATE_MIN_SPEED_LEAP 50
#define COORDINATE_MAX_SPEED_LEAP -50
#define COORDINATE_MIN_DIRECTION_LEAP -50
#define COORDINATE_MAX_DIRECTION_LEAP 50
#define COORDINATE_MIN_RC 0
#define COORDINATE_MAX_RC 100

// Constraint define for the constraint function
#define CONSTRAINT_MIN_SPEED -50
#define CONSTRAINT_MAX_SPEED 50
#define CONSTRAINT_MIN_DIRECTION -50
#define CONSTRAINT_MAX_DIRECTION 50

C_LISTENER::C_LISTENER():
  Listener()
  ,m_socket()
  ,m_previous_speed(0)
  ,m_previous_direction(0)
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
    if ( false == m_socket.Bind() )
    {
      std::cout << "Impossible to bind the socket" << std::endl;
    }
    else
    {
      std::cout << "The socket is binded" << std::endl;
      if ( false == m_socket.Listen() )
      {
        std::cout << "Impossible to listen the socket" << std::endl;
      }
      else
      {
        std::cout << "The socket wait for a new connection..." << std::endl;
        if ( false == m_socket.WaitIncomingConnection() )
        {
          std::cout << "Problem with the new client" << std::endl;
        }
        else
        {
          m_socket.PrintConnectedClient();
        }
      }
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
  float l_thumb_y_position, l_pinky_y_position, l_difference_thumb_pinky, l_hand_position = 0;
  int l_speed, l_direction = 0;
  const Frame frame = p_controller.frame();

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
    l_hand_position = hand.palmPosition().z;
    //std::cout << "Hand position z axis : " << hand.palmPosition().z << std::endl;

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
        //std::cout << "Thumb distal bone y position : " << l_thumb_y_position << std::endl;
      }
      else if ( Finger::TYPE_PINKY == finger.type() )
      {
        Bone::Type boneType = static_cast<Bone::Type>(Bone::TYPE_DISTAL);
        Bone bone = finger.bone(boneType);
        l_pinky_y_position = bone.nextJoint().y;
        //std::cout << "Pinky distal bone y position : " << l_pinky_y_position << std::endl;
      }
    }
    
    if(hand.isRight()){ // Traitement main droite
			//Calculate the difference between the thumb and the pinky to get the direction
			l_difference_thumb_pinky = l_thumb_y_position - l_pinky_y_position;
			//std::cout << "Main droite" << std::endl;
			
		}
	else{ // Traitement main gauche
			
			l_difference_thumb_pinky = l_pinky_y_position - l_thumb_y_position;
			//std::cout << "Main gauche" << std::endl;
		}
  }
  
  //m_socket.Send(CreateMessage(l_hand_position, l_difference_thumb_pinky));
  MapAndConstraint(l_hand_position, l_difference_thumb_pinky, l_speed, l_direction);
  if ( (l_direction >= m_previous_direction+2) || (l_direction <= m_previous_direction-2) )
  {
    m_previous_direction = l_direction;
    m_socket.Send(CreateMessage(0, l_direction));
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

void C_LISTENER::MapAndConstraint(float p_speed_in, float p_direction_in, int& p_speed_out, int& p_direction_out)
{
  // Constraint and map the speed and direction value
  p_speed_out = Map(Constraint(static_cast<int>(p_speed_in), CONSTRAINT_MIN_SPEED, CONSTRAINT_MAX_SPEED), 
                    COORDINATE_MIN_SPEED_LEAP, 
                    COORDINATE_MAX_SPEED_LEAP, 
                    COORDINATE_MIN_RC, 
                    COORDINATE_MAX_RC
                    );
  p_direction_out = Map(Constraint(static_cast<int>(p_direction_in), CONSTRAINT_MIN_DIRECTION, CONSTRAINT_MAX_DIRECTION), 
                        COORDINATE_MIN_DIRECTION_LEAP, 
                        COORDINATE_MAX_DIRECTION_LEAP, 
                        COORDINATE_MIN_RC, 
                        COORDINATE_MAX_RC
                        );
}

string C_LISTENER::CreateMessage(int p_speed, int p_direction)
{
  ostringstream l_string;
  l_string << "{\"commande\":{\"vitesse\":" \
           << p_speed \
           << ",\"direction\":" \
           << p_direction\
           << "}}\n";
  std::cout << l_string.str() << std::endl;
  return l_string.str();
}

int C_LISTENER::Map(int p_x, int p_in_min, int p_in_max, int p_out_min, int p_out_max)
{
  return (p_x - p_in_min) * (p_out_max - p_out_min) / (p_in_max - p_in_min) + p_out_min;
}

int C_LISTENER::Constraint(int p_value, int p_constraint_min, int p_constraint_max)
{
  if ( p_value < p_constraint_min )
  {
    return p_constraint_min ;
  }
  else if ( p_value > p_constraint_max )
  {
    return p_constraint_max ;
  }
  else
  {
    return p_value;
  }
}
