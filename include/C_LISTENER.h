#pragma once

#include "Leap.h"
#include "C_SOCKET.h"

#include <string>

using namespace Leap;
using namespace std;

const string fingerNames[] = {"Thumb", "Index", "Middle", "Ring", "Pinky"};
const string boneNames[] = {"Metacarpal", "Proximal", "Middle", "Distal"};

class C_LISTENER : public Listener
{
  public:
    //Constructor and destructor 
    C_LISTENER();
    virtual ~C_LISTENER();

    //Override Members functions
    virtual void onInit(const Controller &p_controller);
    virtual void onConnect(const Controller &p_controller);
    virtual void onDisconnect(const Controller &p_controller);
    virtual void onExit(const Controller &p_controller);
    virtual void onFrame(const Controller &p_controller);
    virtual void onFocusGained(const Controller &p_controller);
    virtual void onFocusLost(const Controller &p_controller);
    virtual void onDeviceChange(const Controller &p_controller);
    virtual void onServiceConnect(const Controller &p_controller);
    virtual void onServiceDisconnect(const Controller &p_controller);

  private:
    C_SOCKET m_socket;
    int m_previous_speed;
    int m_previous_direction;
    string CreateMessage(int p_speed, int p_direction);
    void MapAndConstraint(float p_speed_in, float p_direction_in, int& p_speed_out, int& p_direction_out);
 
    int Map(int p_x, int p_in_min, int p_in_max, int p_out_min, int p_out_max);
    int Constraint(int p_value, int p_constraint_min, int p_constraint_max);
};
