/*******************************************************************************
 *  Copyright: RMIT University,  2012 - 2013		                       *
 *  Author(s): Khandakar Entenam Unayes Ahmed		                       *
 *  This file is distributed under the terms in the attached LICENSE file.     *
 *  If you do not find this file, copies can be found by writing to:           *
 *                                                                             *
 *      Khandakar Entenam Unayes Ahmed, SECE, RMIT University                  *
 *      e-mail: khandakar.ahmed@rmit.edu.au			               *
 *      Attention:  License Inquiry.                                           *
 *                                                                             *  
 *******************************************************************************/

#ifndef _SBDROUTING_H_
#define _SBDROUTING_H_

#include "VirtualRouting.h"
#include "SBDRouting_m.h"

using namespace std;

enum SBDTimers {
	START_ROUND = 1,
	MEMBER_NODE_SELECTION_ROUND = 2,
	SEND_BEACON = 3,
	JOIN_SECTOR = 4,
	TDMA_SCHEDULING = 5,
	TRANSMISSION_SLOT = 6,
	SECTOR_SLOT_SELECTION_ROUND = 7,
	SEGMENTATION_ROUND = 8,
};

enum packetTypes{
	REMOTE_UPDATE = 1,
	RANGE_QUERY = 2,
};

struct routingRecord {
	int destSectorID;
	int hopCount;
	int nextHopID;
	bool update;
};

struct nodeInfo{
	int src;
	double rssi;
};

struct segmentNode{
	vector <int> ID;	
	vector <double> rssi;
	vector <int> tally;
};

struct storageInMemberNode{
	int sourceSectorID;
	int seqNum;
	int memberNodeID;
	int tally;
};


class SBDRouting: public VirtualRouting {
 private:
	bool isSectorHead;
	int sectorHeadID;
	int parentSectorID;
	double nodeStartTimeout;
	double topologyPacketTimeout;
	int packetsPerChildNode;
	int packetsPerMemberNode;

	vector <nodeInfo> SHCandidates;
	vector <int> sectorMembers;
	vector <nodeInfo> sectorMembersList;
	vector <segmentNode> segmentList;
	vector<int> sectorSlotArray;
	vector <storageInMemberNode> storageInMemberNodeTable;

	int tmpDestSectorID;
	int tmpHopCount;
	int tmpNextHopID;
	double transmissionLength;
	double sectorSlotLength;
	int sectorSize;

	string sectorHeadAddress;	
	string memberNodeAddress;
	string segmentAddress;
	string nextHopAddress;
	string selfSegmentID;

	int numberOfTracks;
	int numberOfSectors;
	int roundCounter;
	int segmentsPerSector;
	int nodesPerSegment;

	int totalRound;
	double MemberNodeSelectionRoundTime;
	double segmentationTime;
	double sendBeaconTime;
	double TDMASchedulingTime;
	double joinSectorTime;

	double roundLength;
	double learningPhaseLength;
	double currentRoundStartTime;
	double txInterval_total;
	double queryTimeLength;


	int addPadRemUp2Segment;
	int addPadRangeQuery;
	int addPadKNNQuery;
	int addPadSkyLineQuery;
	int addPadDecider;

 protected:
	void startup();
	void sendTopologySetupPacket(int,int);
	void fromApplicationLayer(cPacket *, const char *);
	void fromMacLayer(cPacket *, int, double, double);
	void timerFiredCallback(int);
	bool updateRoutingTable(int,int,int,double,double);
	int searchRoutingTable(int);
	int searchNextHop(int);
	void finishSpecific();
	void transmitBufferedPacket();
	void transmitAggregatedBufferedPacket();
	void parseStringParams(void);
	int destin4MemberNode(int);
	int sortSegmentMemberNode(int);
 public:
	int getSegmentsPerSector(void);
};
bool cmpRSSI(nodeInfo a, nodeInfo b);
bool cmpTALLY(storageInMemberNode a, storageInMemberNode b);
#endif				//SBDROUTING
