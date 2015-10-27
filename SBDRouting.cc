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

#include "SBDRouting.h"
#include "algorithm"

#define NEXT_HOP_ADDRESS nextHopAddress.c_str()
#define SECTOR_HEAD_ADDRESS sectorHeadAddress.c_str()
#define MEMBER_NODE_ADDRESS memberNodeAddress.c_str()
#define SEGMENT_ADDRESS segmentAddress.c_str()
#define SELF_SEGMENT_ADDRESS selfSegmentID.c_str();

Define_Module(SBDRouting);

/* Application layer sends a packet together with a dest network layer address.
 * Network layer is responsible to route that packet by selecting an appropriate
 * MAC address.
 */
void SBDRouting::startup()
{
	parseStringParams();	//read the params from omnet.ini and initialize the variables
	transmissionLength = 0;	//Length of Intra-Sector time slot (Transmission length of a member node)
	roundCounter = 0;	
	SHCandidates.clear();	
	segmentList.clear();
	storageInMemberNodeTable.clear();
	/** 
		The events for Learning stage is set in three steps. 
		Hence, Learning Phase Length is the accumulation of these 3
		2.0 sec is added to finish TDMAScheduling
	**/

	learningPhaseLength = MemberNodeSelectionRoundTime + segmentationTime + TDMASchedulingTime + 2.0; 

	/**
		############
	**/
	for(int i=0;i<numberOfTracks;i++){
		for(int j=0;j<numberOfSectors;j++){
			sectorSlotArray.push_back(-1);
		}
	}

	roundLength = 4 * sectorSlotLength + learningPhaseLength + queryTimeLength + 5.0; //2 sec = roundSpacing, keeps a safe space between two rounds
	txInterval_total = (roundLength * totalRound);
	trace()<< "Simulation Time: " << strtod(ev.getConfig()->getConfigValue("sim-time-limit"),NULL) << " TX Interval Time: " << txInterval_total << " Round Length: " << roundLength << " Total Round: " << totalRound << " SectorSlotLength: " << sectorSlotLength;
	if (strtod(ev.getConfig()->getConfigValue("sim-time-limit"), NULL) < txInterval_total) {
		trace() << "ERROR: Total sim time should be at least = " << txInterval_total;
		opp_error("\nError: simulation time not large enough for the Sector Based Distance Routing\n");
	}

	addPadRemUp2Segment = 100;	// ??
	addPadRangeQuery = 200;		// ??
	addPadKNNQuery = 300;		// ??
	addPadSkyLineQuery = 400;	// ??
	addPadDecider = 100;		// ??

	setTimer(START_ROUND, roundLength*roundCounter);
}

void SBDRouting::timerFiredCallback(int index){
	double timer = uniform(0,1);
	switch(index){
		case START_ROUND:{
			if(roundCounter < totalRound){
				SHCandidates.clear();
				sectorMembers.clear();
				sectorMembersList.clear();
				segmentList.clear();
				roundCounter++;
				currentRoundStartTime = (roundCounter-1)*roundLength;
				if(isSectorHead)
					setTimer(SECTOR_SLOT_SELECTION_ROUND, currentRoundStartTime);
					trace() << "From START_ROUND: " << roundCounter << " Total Round: " << totalRound << " Round Length: " << roundLength << "currentStartTime " << currentRoundStartTime << "Child Selection Round " << currentRoundStartTime+MemberNodeSelectionRoundTime << "Segmentation Time" << segmentationTime << "Round Start Time " << roundCounter*roundLength;
				setTimer(MEMBER_NODE_SELECTION_ROUND,MemberNodeSelectionRoundTime);
				setTimer(START_ROUND, roundLength);
			}
		}
		/**
			Implies Grid Coloring Algorithm (GCA) to assign conflict free TDMA time slot to each sector (reuses only 4 slots)
		**/	
		case SECTOR_SLOT_SELECTION_ROUND:{
			int m = numberOfTracks;
			int n = numberOfSectors;
			for(int j=1;j<=m;j+=2){
				for(int i=(j-1)*n;i<j*n-1;i+=2){
					if(i<n*j){
						sectorSlotArray[i]=1;
					}
					if(i+1<n*j){
						sectorSlotArray[i+1]=2;
					}
					if(i+n<m*n){
						sectorSlotArray[i+n]=3;
					}	
					if(i+n+1<m*n){
						sectorSlotArray[i+n+1]=4;
					}	
				}
			}
			break;
		}		
		/**
			Sector Head (SH)-
				#schedule to Broadcast 'beaconframe' at currentRoundStartTime + 2.0 sec
				#schedule to execute SEGMENTATION_ROUND at currentRoundStartTime+ 5.0 sec
			Member Node(or Non Sector Head Node)-
				#schedule to executes JOIN_SECTOR at currentRoundStartTime + 4.0 sec 
		**/
		case MEMBER_NODE_SELECTION_ROUND:{
			if(isSectorHead){
				setTimer(SEND_BEACON,sendBeaconTime);
				setTimer(SEGMENTATION_ROUND, segmentationTime);
			}
			else{
				setTimer(JOIN_SECTOR,joinSectorTime);
			}	
			break;
		}	
		/**
			Each SH broadcasts beacon frames
		**/
		case SEND_BEACON:{
			SBDRoutingPacket *ctrlPkt = new SBDRoutingPacket("Sector Head Announcement Packet", NETWORK_LAYER_PACKET);
			ctrlPkt->setSBDRoutingPacketKind(SBD_BEACON_PACKET);
			ctrlPkt->setSource(SELF_NETWORK_ADDRESS);
			toMacLayer(ctrlPkt, BROADCAST_MAC_ADDRESS);
			break;
		}
		/**
			After receiving beacon frames from SHs, Member Nodes send joining request to SH, from which they receive the frame with highest RSSI
		**/
		case JOIN_SECTOR:{
			if(SHCandidates.size()!=0){
				sort(SHCandidates.begin(),SHCandidates.end(),cmpRSSI);		
				SBDRoutingPacket *ctrlPkt = new SBDRoutingPacket("Sector Member Joining Packet", NETWORK_LAYER_PACKET);		
				ctrlPkt->setSBDRoutingPacketKind(SBD_JOIN_PACKET);
				ctrlPkt->setSource(SELF_NETWORK_ADDRESS);
				nodeInfo info = (nodeInfo)SHCandidates.back();
				stringstream convert;
				convert << info.src;
				sectorHeadAddress = convert.str();
				ctrlPkt->setDestination(SECTOR_HEAD_ADDRESS);
				toMacLayer(ctrlPkt, resolveNetworkAddress(SECTOR_HEAD_ADDRESS));
			}
			break;
		}
		/**
			Once the association between SH and member nodes done, SH applies Segmentation. Segmentation is optional to be used. The overhead is zero as
			the process doesn't include any extra transmission and reception. The segmentation is useful for Application Layer 		
		**/
		case SEGMENTATION_ROUND:{
			sort(sectorMembersList.begin(),sectorMembersList.end(),cmpRSSI);
			int index=0;
			//trace()<< "SegmentMemberListSize: " << sectorMembersList.size() << "SegmentsPerSector: " << segmentsPerSector;
			nodesPerSegment = ceil((double)sectorMembersList.size()/(double)segmentsPerSector);
			//trace() << "Nodes Per Segment: " << nodesPerSegment;
			int segmentCounter = 0;
			while(index<sectorMembersList.size() && segmentCounter<segmentsPerSector){
				segmentNode rec;
				for(int i=index;i<nodesPerSegment*(segmentCounter+1) && i<sectorMembersList.size();i++){
					(rec.ID).push_back(sectorMembersList[i].src);
					(rec.rssi).push_back(sectorMembersList[i].rssi);
					(rec.tally).push_back(0);
				}
				segmentList.push_back(rec);
				index = (++segmentCounter)*nodesPerSegment;
			}
			segmentsPerSector = segmentList.size();
			//trace() << "Segments Per Sector after Segmentation: " << segmentsPerSector;
			/*for(int i=0;i<(int)segmentList.size();i++){
				trace()<< "Segment: " << i;
				for(int j=0;j<(segmentList[i].ID).size();j++)
					trace()<< "Segment Members: " << segmentList[i].ID[j];
			}*/
			setTimer(TDMA_SCHEDULING,TDMASchedulingTime);
			break;
		}
		/**
			Each SH broadcasts its 'Inter-Sector Slot Number', 'Member Array', 'The Segment Number for each member  node'
			Also determines its own slot of transmission
		**/
		case TDMA_SCHEDULING:{
			transmissionLength = sectorSlotLength/sectorMembers.size();
			if(sectorMembers.size()!=0){
				SBDRoutingPacket *ctrlPkt = new SBDRoutingPacket("TDMA Scheduling Packet", NETWORK_LAYER_PACKET);
				ctrlPkt->setSBDRoutingPacketKind(SBD_TDMA_SCHEDULE_PACKET);
				ctrlPkt->setSource(SELF_NETWORK_ADDRESS);
				ctrlPkt->setDestination(BROADCAST_NETWORK_ADDRESS);
				sectorSize = sectorMembers.size();
				ctrlPkt->setSlotArraySize(sectorSize);
				ctrlPkt->setSegmentAddressArraySize(sectorSize);
				ctrlPkt->setSectorSlotNumber(sectorSlotArray[atoi(SELF_NETWORK_ADDRESS)]);
				for(int i=0;i<sectorSize;i++){
					ctrlPkt->setSlot(i,sectorMembers[i]);
					for(int k=0;k<(int)segmentList.size();k++){
						for(int j=0;j<(segmentList[k].ID).size();j++){
							if(segmentList[k].ID[j]==sectorMembers[i]){
								ctrlPkt->setSegmentAddress(i,k);
							}		
						}
					}
				}
				toMacLayer(ctrlPkt, BROADCAST_MAC_ADDRESS);
				double slot = (sectorSlotArray[atoi(SELF_NETWORK_ADDRESS)]) * sectorSlotLength + learningPhaseLength +timer;
				setTimer(TRANSMISSION_SLOT,slot);
			}
			else{
				double slot = (sectorSlotArray[atoi(SELF_NETWORK_ADDRESS)]) * sectorSlotLength + transmissionLength + learningPhaseLength + timer;
				setTimer(TRANSMISSION_SLOT,slot);
			}
			break;
		}
		case TRANSMISSION_SLOT:{
			transmitBufferedPacket();
			break;
		}
	}
}

void SBDRouting::fromApplicationLayer(cPacket * pkt, const char *destination)
{
	string dst(destination);
	/**
		Member Nodes Receive Sensed Events from Application Layer
		Encapsualte into Network Layer Packet
		Push into TXBuffer
	**/
	if(!isSectorHead){
		SBDRoutingPacket *netPacket = new SBDRoutingPacket("Local Data Update Packet", NETWORK_LAYER_PACKET);
		if(atoi(destination)<0){
			//trace()<< "Destination Supposed --------------------!!!!!!!!!!!!!!!!From IF: " << atoi(destination);
			netPacket->setSBDRoutingPacketKind(SBD_DATA_PACKET_LOCAL_UPDATE);
		}
		else{
			//trace()<< "Destination Supposed --------------------!!!!!!!!!!!!!!!!From ELSE: " << atoi(destination);
			netPacket->setSBDRoutingPacketKind(SBD_RANGE_LOCAL_QUERY);
		}
		netPacket->setSource(SELF_NETWORK_ADDRESS);
		//netPacket->setSBDRoutingPacketKind(SBD_DATA_PACKET_LOCAL_UPDATE);
		netPacket->setDestination(SECTOR_HEAD_ADDRESS);
		encapsulatePacket(netPacket,pkt);
		if((int)TXBuffer.size() >= netBufferSize){
			TXBuffer.pop();
		}
		else
			bufferPacket(netPacket);
	}
	/**
		SH processes following packets received from Application Layer when destination address is negative (#addPadDecider is used to decide the type of packet 			based on Destination from Application Layer)
			- Remote Storage Update
			- Query Packets
		If the destination is negative, SH looks for next hop address and push it down to the MAC Layer
	**/
	else{	
		if(atoi(destination) == atoi(SELF_NETWORK_ADDRESS)){	// Push Back the packets destined for itself
			toApplicationLayer(pkt);	
		}
		else if(atoi(destination)<0){	
			SBDRoutingPacket *netPacket = new SBDRoutingPacket("Remote Data Update Packet", NETWORK_LAYER_PACKET);
			stringstream convert;
			switch(-atoi(destination)/addPadDecider){
				case REMOTE_UPDATE:{
					netPacket->setSBDRoutingPacketKind(SBD_DATA_PACKET_REMOTE_UPDATE);
					convert << destin4MemberNode(-atoi(destination)%addPadRemUp2Segment);
					memberNodeAddress = convert.str();
					netPacket->setDestination(MEMBER_NODE_ADDRESS);
					encapsulatePacket(netPacket, pkt);
					toMacLayer(netPacket, resolveNetworkAddress(MEMBER_NODE_ADDRESS));	
					break;
				}
				case RANGE_QUERY:{
					netPacket->setSBDRoutingPacketKind(SBD_RANGE_QUERY_PACKET);
					convert << ((-atoi(destination)%addPadRangeQuery)-1)%segmentList.size();
					segmentAddress = convert.str();
					netPacket->setDestination(SEGMENT_ADDRESS);
					trace() << "########################" << SEGMENT_ADDRESS;
					encapsulatePacket(netPacket, pkt);
					toMacLayer(netPacket, BROADCAST_MAC_ADDRESS);
					break;
				}
			}
		}
		/**
			Search a neighbor SH as a next hop and encapsulate the packet into network layer packet. Store into buffer
		**/
		else{	
			int nextHop = searchNextHop(atoi(destination));
			stringstream out;
	 		out << nextHop;
			nextHopAddress = out.str();
			SBDRoutingPacket *netPacket = new SBDRoutingPacket("Remote Data Update Packet", NETWORK_LAYER_PACKET);
			netPacket->setSource(SELF_NETWORK_ADDRESS);
			netPacket->setSBDRoutingPacketKind(SBD_DATA_PACKET_REMOTE_UPDATE);
			netPacket->setDestination(destination);
			netPacket->setPrevHop(SELF_NETWORK_ADDRESS);
			netPacket->setNextHop(NEXT_HOP_ADDRESS);
			encapsulatePacket(netPacket, pkt);
			bufferPacket(netPacket);
		}
	}
}

void SBDRouting::fromMacLayer(cPacket * pkt, int srcMacAddress, double rssi, double lqi)
{
	SBDRoutingPacket *netPacket = dynamic_cast <SBDRoutingPacket*>(pkt);
	string destination(netPacket->getDestination());
	if (!netPacket) {
		return;
	}
	switch(netPacket->getSBDRoutingPacketKind()){
		// send packets received from member nodes to application layer
		case SBD_DATA_PACKET_LOCAL_UPDATE:{
			if(isSectorHead){
				if(destination.compare(SELF_NETWORK_ADDRESS) == 0){
					toApplicationLayer(decapsulatePacket(pkt));
				}
			}
			break;
		}
		case SBD_RANGE_LOCAL_QUERY:{
			if(isSectorHead){
				if(destination.compare(SELF_NETWORK_ADDRESS) == 0){
					toApplicationLayer(decapsulatePacket(pkt));
				}
			}
			break;
		}
		/**
			SH receives a REMOTE_UPDATE packet either
				a) For itself (pushes up to application layer)
				b) or as a relaying node. Look for next hop, change the next hop address and push back to the MAC Layer
			A Member node receives REMOTE_UPDATE packet to store
		**/
		case SBD_DATA_PACKET_REMOTE_UPDATE:{
			string thisHop(netPacket->getNextHop());
			if(isSectorHead){				
				if(destination.compare(SELF_NETWORK_ADDRESS)==0 && thisHop.compare(SELF_NETWORK_ADDRESS)==0){
					toApplicationLayer(decapsulatePacket(pkt));
				}
				else if(thisHop.compare(SELF_NETWORK_ADDRESS)==0){
					int nextHop = searchNextHop(atoi(netPacket->getDestination()));
					stringstream out;
					out << nextHop;
					string nextHopAddress = out.str();
					SBDRoutingPacket *dataPkt = netPacket->dup();
					dataPkt->setSBDRoutingPacketKind(SBD_DATA_PACKET_REMOTE_UPDATE);
					dataPkt->setSource(netPacket->getSource());
					dataPkt->setDestination(netPacket->getDestination());
					dataPkt->setPrevHop(SELF_NETWORK_ADDRESS);
					dataPkt->setNextHop(NEXT_HOP_ADDRESS);
					toMacLayer(dataPkt, resolveNetworkAddress(NEXT_HOP_ADDRESS));
				}
			}
			else if(destination.compare(SELF_NETWORK_ADDRESS)==0 && srcMacAddress == atoi(sectorHeadAddress.c_str())){
					toApplicationLayer(decapsulatePacket(pkt));
			}
			break;
		}
		case SBD_RANGE_QUERY_PACKET:{
			if(!isSectorHead){
				if(atoi(selfSegmentID.c_str())==atoi(netPacket->getDestination()) && srcMacAddress == atoi(sectorHeadAddress.c_str()))
				{ 					
					toApplicationLayer(decapsulatePacket(pkt));
				}
			}
			break;
		}
		/**
			Member nodes includes the source of a Beacon Frame as a possibe candidate of its parent (SH)
		**/
		case SBD_BEACON_PACKET:{
			if(!isSectorHead){
				nodeInfo rec;
				rec.src = atoi(netPacket->getSource());
				rec.rssi = rssi;
				SHCandidates.push_back(rec);
			}				
			break;
		}
		/**
			SH adds a member to its member list after receiving a joining request from it
		**/
		case SBD_JOIN_PACKET:{
			string dst(netPacket->getDestination());
			if(isSectorHead && dst.compare(SELF_NETWORK_ADDRESS)==0){
				sectorMembers.push_back(atoi(netPacket->getSource()));
				nodeInfo rec;
				storageInMemberNode tempRec;
				rec.src=tempRec.memberNodeID = atoi(netPacket->getSource());
				rec.rssi = rssi;
				tempRec.tally = 0;
				storageInMemberNodeTable.push_back(tempRec);
				sectorMembersList.push_back(rec);
			}
			break;
		}
		/**
			Member nodes receive TDMA_SCHEDULING_PACKET from SH and shedules their conflict free scheduling insided the sector
		**/
		case SBD_TDMA_SCHEDULE_PACKET:{
			double timer = uniform(0,1);
			if(!isSectorHead){
				if(atoi(netPacket->getSource()) == atoi(SECTOR_HEAD_ADDRESS)){
					sectorSize = netPacket->getSlotArraySize();
					transmissionLength = sectorSlotLength/sectorSize;
					for(int i=0;i<sectorSize;i++)
					{
						if(netPacket->getSlot(i) == atoi(SELF_NETWORK_ADDRESS)){
							send(createRadioCommand(SET_STATE,SLEEP),"toMacModule");
							double slot = ((netPacket->getSectorSlotNumber()-1) * sectorSlotLength) + (i * transmissionLength) + learningPhaseLength +timer;
							//trace() << "transmission Length: " << transmissionLength << "Slot:" << slot;
							stringstream convert;
							convert << (int)netPacket->getSegmentAddress(i);
							selfSegmentID = convert.str();
							trace() << "Segment Address: " << SELF_SEGMENT_ADDRESS;
							setTimer(TRANSMISSION_SLOT, slot);
							break;
						}
					}
				}
			}
			break;
		}
	}
}

void SBDRouting::finishSpecific()
{
	/*for(int i=0;i< (int)sectorMembers.size();i++){
		trace() << "Sector: " << SELF_NETWORK_ADDRESS << "Members: " << sectorMembers[i];
		trace() << "Sector: " << SELF_NETWORK_ADDRESS << "Members: " << sectorMembersList[i].src << "RSSI" << sectorMembersList[i].rssi;		
	}*/
	/*for(int i=0;i< (int)sectorSlotArray.size();i++){
		trace() << "Sector Number: " << i << "Slot Number: " << sectorSlotArray[i];
	}*/
	for(int i=0;i<(int)segmentList.size();i++){
		trace()<< "Segment: " << i;
		for(int j=0;j<(segmentList[i].ID).size();j++)
			trace()<< "Segment Members: " << segmentList[i].ID[j];
	}
}

/**
	Searches Next Hop
**/

int SBDRouting::searchNextHop(int destination){
	int destCol, destRow, currCol, currRow, nextHopCol, nextHopRow;	
	destCol = destination%4;
	destRow = destination/4;
	currCol = nextHopCol = atoi(SELF_NETWORK_ADDRESS)%4;
	currRow = nextHopRow = atoi(SELF_NETWORK_ADDRESS)/4;
	if(currCol<destCol){
		nextHopCol = nextHopCol + 1;
		return nextHopRow * 4 + nextHopCol;
	}
	else if(currCol > destCol){
		nextHopCol = nextHopCol - 1;
		return nextHopRow * 4 + nextHopCol;
	}
	else if(currCol == destCol){
		if(currRow < destRow){
			nextHopRow = nextHopRow  + 1;
			return nextHopRow * 4 + nextHopCol;
		}
		else if(currRow > destRow){
			nextHopRow = nextHopRow  - 1;
			return nextHopRow * 4 + nextHopCol;
		}
	}
	return -1;
}

/**
	The packets stored in a buffer are pushed into MAC layer.
**/

void SBDRouting::transmitBufferedPacket(){
	int numberOfPacketsSent = 0;
	while(!TXBuffer.empty()){
		SBDRoutingPacket *netPacket = check_and_cast < SBDRoutingPacket * >(TXBuffer.front());
		if(!isSectorHead){
			switch(netPacket->getSBDRoutingPacketKind()){
				case SBD_RANGE_LOCAL_QUERY:{
					netPacket->setDestination(SECTOR_HEAD_ADDRESS);
					toMacLayer(netPacket,resolveNetworkAddress(SECTOR_HEAD_ADDRESS));
					break;
				}
				case SBD_DATA_PACKET_LOCAL_UPDATE:{
					if(packetsPerMemberNode!=0){
						if(numberOfPacketsSent < packetsPerMemberNode){
							trace() << "Number of Packets: " << packetsPerChildNode;
							netPacket->setDestination(SECTOR_HEAD_ADDRESS);
							toMacLayer(netPacket,resolveNetworkAddress(SECTOR_HEAD_ADDRESS));
						}
					}
					else{
						netPacket->setDestination(SECTOR_HEAD_ADDRESS);
						toMacLayer(netPacket,resolveNetworkAddress(SECTOR_HEAD_ADDRESS));
					}

					break;
				}
				
			}
		}
		else{
			int nextHop = atoi(netPacket->getNextHop());		
			stringstream out;
			out << nextHop;
			string nextHopAddress = out.str();
			toMacLayer(netPacket,resolveNetworkAddress(NEXT_HOP_ADDRESS));
		}
		TXBuffer.pop();
		numberOfPacketsSent++;
	}	
}

void SBDRouting::transmitAggregatedBufferedPacket(){
	// Not Used Yet
}

/**
	Based on the 'destinationSegmentPivotIndex', Finds the candidate member node where the event to be stored
**/

int SBDRouting::destin4MemberNode(int destinationSegmentPivotIndex){
	int index=0;
	trace() << "The Size of Segment: " << segmentList.size();
	if(destinationSegmentPivotIndex <= (int)segmentList.size())
		index = destinationSegmentPivotIndex-1;
	else
		index = segmentList.size()-1;
	sortSegmentMemberNode(index);
	int memberNodeID = segmentList[index].ID[0];
	trace() << "Segment: " << index << "Member ID: " << memberNodeID << "Tally: " << segmentList[index].tally[0];
	segmentList[index].tally[0]++;
	return memberNodeID;
}

/**
	Sort the member nodes inside a segment in ascending order based on the number of packets are stored on them
**/

int SBDRouting::sortSegmentMemberNode(int index){
	int k,j,temp_tally, temp_ID;
	double temp_rssi;
	for (int i = 1; i < (segmentList[index].ID).size(); i++) {
		temp_tally = segmentList[index].tally[i];
		temp_ID = segmentList[index].ID[i];
		temp_rssi = segmentList[index].rssi[i]; 
		j = i;
 		while (j>0 && segmentList[index].tally[j-1] > temp_tally){
			segmentList[index].tally[j] = segmentList[index].tally[j-1];
			segmentList[index].ID[j] = segmentList[index].ID[j-1];	
			segmentList[index].rssi[j] = segmentList[index].rssi[j-1];			
			j--;
		}
		segmentList[index].tally[j] = temp_tally;
		segmentList[index].ID[j] = temp_ID;	
		segmentList[index].rssi[j] = temp_rssi;
	}
}

void SBDRouting::parseStringParams(void){
	/****
		Scan the deployment layout defined in ini file.
	****/
	cStringTokenizer gridTokenizer(getParentModule()->getParentModule()->getParentModule()->par("deployment"), ";");
	int iteration = 1;
	while (gridTokenizer.hasMoreTokens()){
		string token(gridTokenizer.nextToken());
		string::size_type posA = token.find("->");
		token = token.substr(posA+2);
		string::size_type posB = token.find("x");
		string tokA = token.substr(0, posB);
		string tokB = token.substr(posB+1);
		if(iteration == 1){
			numberOfTracks = (double)atof(tokA.c_str());
			numberOfSectors = (double)atof(tokB.c_str());
		} 	
		iteration++;
	}	
	totalRound = (int)par("totalRound");
	sendBeaconTime = (double)par("sendBeaconTime") / 1000;
	TDMASchedulingTime = (double)par("TDMASchedulingTime") / 1000;
	joinSectorTime = (double)par("joinSectorTime") / 1000;
	MemberNodeSelectionRoundTime = (double)par("MemberNodeSelectionRoundTime") / 1000;
	segmentationTime = (double)par("segmentationTime") / 1000;
	sectorSlotLength = (double)par("sectorSlotLength") / 1000;
	segmentsPerSector = (int)par("segmentsPerSector");
	nodesPerSegment = (int)par("nodesPerSegment");
	packetsPerMemberNode = (int)par("packetsPerMemberNode");
	/***
		Scan params value from application layer.
	***/
	cModule *appModule = getParentModule()->getParentModule()->getSubmodule("Application");
	queryTimeLength = appModule->par("queryTimeLength");	// queryTimeLength is scanned to calculate the length of a round
	if (appModule->hasPar("isSectorHead"))
		isSectorHead = appModule->par("isSectorHead");
	else
		opp_error("\nSector Based Distance Routing Must Have Sector Head");
}

bool cmpRSSI(nodeInfo a, nodeInfo b){
	return (a.rssi < b.rssi);
}

bool cmpTALLY(storageInMemberNode a, storageInMemberNode b){
	return (a.tally < b.tally);
}

int SBDRouting::getSegmentsPerSector(void){
	return segmentsPerSector;
}
