#include "PacketIdentification.h"

const int PLAYER_INPUT = 4;
const int PLAYER_STATE_UPDATE = 5;

//packet id for packets containing information about the other players connected to the server, does not contain info about the user's player
const int PLAYER_INTERPOLATION_INFO = 6;

extern const int PLAYER_GUNFIRE = 10;

//empty packets exchanged to test latency between server and client
const int LATENCY_TEST = 7;

//empty packets used to attempt to connect to the server, if a response is received then client is connected
//response to this connection will be another connection attempt
const int CONNECTION_ATTEMPT = 8;

const int WORLD_STATE_UPDATE = 9;
