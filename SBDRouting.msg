//*******************************************************************************
//*  Copyright: RMIT University,  2012 - 2013		                        *
//*  Author(s): Khandakar Entenam Unayes Ahmed		                        *
//*  This file is distributed under the terms in the attached LICENSE file.     *
//*  If you do not find this file, copies can be found by writing to:           *
//*                                                                             *
//*      Khandakar Entenam Unayes Ahmed, SECE, RMIT University                  *
//*      e-mail: khandakar.ahmed@rmit.edu.au			                *
//*      Attention:  License Inquiry.                                           *
//*                                                                             *  
//*******************************************************************************/


cplusplus {{
#include "RoutingPacket_m.h"
}}

class RoutingPacket;

enum SBDRoutingPacketDef {
	SBD_DATA_PACKET_LOCAL_UPDATE = 1;
	SBD_DATA_PACKET_REMOTE_UPDATE = 2;
	SBD_BEACON_PACKET = 3;
	SBD_JOIN_PACKET = 4;
	SBD_TDMA_SCHEDULE_PACKET = 5;
	SBD_RANGE_QUERY_PACKET = 6;
	SBD_RANGE_LOCAL_QUERY = 7;
}

packet SBDRoutingPacket extends RoutingPacket {
	int SBDRoutingPacketKind enum (SBDRoutingPacketDef);
	int destSectorHeadID;
	int slot[];
	int segmentAddress[];
	string nextHop;
	string prevHop;
	int sectorSlotNumber;
}

