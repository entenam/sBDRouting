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

package node.communication.routing.sBDRouting;

simple SBDRouting like node.communication.routing.iRouting {
 parameters:
	bool collectTraceInfo = default (false);
	int maxNetFrameSize = default (0);				// bytes, 0 means no limit
	int netBufferSize = default (32);				// number of messages
	int netDataFrameOverhead = default (14);			// bytes
	int segmentsPerSector = default (3);				// number of Segments in Each Sector 
	int nodesPerSegment = default(3);				// number of nodes per segment

	int packetsPerMemberNode = default (0);				// 0 means no limit	

	double sectorSlotLength = default(4000);			// ms, length of Inter-Sector time slot
	int totalRound = default(1);					

	double MemberNodeSelectionRoundTime = default(20);		//ms, During this phase SH selects its member nodes
	double segmentationTime = default(6000);
	double sendBeaconTime = default(2000); 
	double TDMASchedulingTime = default(6000);
	double joinSectorTime = default(4000);
 gates:
	output toCommunicationModule;
	output toMacModule;
	input fromCommunicationModule;
	input fromMacModule;
	input fromCommModuleResourceMgr;
}

