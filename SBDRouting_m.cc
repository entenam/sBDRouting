//
// Generated file, do not edit! Created by opp_msgc 4.1 from src/node/communication/routing/sBDRouting/SBDRouting.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "SBDRouting_m.h"

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// Another default rule (prevents compiler from choosing base class' doPacking())
template<typename T>
void doPacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doPacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}

template<typename T>
void doUnpacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doUnpacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}




EXECUTE_ON_STARTUP(
    cEnum *e = cEnum::find("SBDRoutingPacketDef");
    if (!e) enums.getInstance()->add(e = new cEnum("SBDRoutingPacketDef"));
    e->insert(SBD_DATA_PACKET_LOCAL_UPDATE, "SBD_DATA_PACKET_LOCAL_UPDATE");
    e->insert(SBD_DATA_PACKET_REMOTE_UPDATE, "SBD_DATA_PACKET_REMOTE_UPDATE");
    e->insert(SBD_BEACON_PACKET, "SBD_BEACON_PACKET");
    e->insert(SBD_JOIN_PACKET, "SBD_JOIN_PACKET");
    e->insert(SBD_TDMA_SCHEDULE_PACKET, "SBD_TDMA_SCHEDULE_PACKET");
    e->insert(SBD_RANGE_QUERY_PACKET, "SBD_RANGE_QUERY_PACKET");
    e->insert(SBD_RANGE_LOCAL_QUERY, "SBD_RANGE_LOCAL_QUERY");
);

Register_Class(SBDRoutingPacket);

SBDRoutingPacket::SBDRoutingPacket(const char *name, int kind) : RoutingPacket(name,kind)
{
    this->SBDRoutingPacketKind_var = 0;
    this->destSectorHeadID_var = 0;
    slot_arraysize = 0;
    this->slot_var = 0;
    segmentAddress_arraysize = 0;
    this->segmentAddress_var = 0;
    this->nextHop_var = 0;
    this->prevHop_var = 0;
    this->sectorSlotNumber_var = 0;
}

SBDRoutingPacket::SBDRoutingPacket(const SBDRoutingPacket& other) : RoutingPacket()
{
    setName(other.getName());
    slot_arraysize = 0;
    this->slot_var = 0;
    segmentAddress_arraysize = 0;
    this->segmentAddress_var = 0;
    operator=(other);
}

SBDRoutingPacket::~SBDRoutingPacket()
{
    delete [] slot_var;
    delete [] segmentAddress_var;
}

SBDRoutingPacket& SBDRoutingPacket::operator=(const SBDRoutingPacket& other)
{
    if (this==&other) return *this;
    RoutingPacket::operator=(other);
    this->SBDRoutingPacketKind_var = other.SBDRoutingPacketKind_var;
    this->destSectorHeadID_var = other.destSectorHeadID_var;
    delete [] this->slot_var;
    this->slot_var = (other.slot_arraysize==0) ? NULL : new int[other.slot_arraysize];
    slot_arraysize = other.slot_arraysize;
    for (unsigned int i=0; i<slot_arraysize; i++)
        this->slot_var[i] = other.slot_var[i];
    delete [] this->segmentAddress_var;
    this->segmentAddress_var = (other.segmentAddress_arraysize==0) ? NULL : new int[other.segmentAddress_arraysize];
    segmentAddress_arraysize = other.segmentAddress_arraysize;
    for (unsigned int i=0; i<segmentAddress_arraysize; i++)
        this->segmentAddress_var[i] = other.segmentAddress_var[i];
    this->nextHop_var = other.nextHop_var;
    this->prevHop_var = other.prevHop_var;
    this->sectorSlotNumber_var = other.sectorSlotNumber_var;
    return *this;
}

void SBDRoutingPacket::parsimPack(cCommBuffer *b)
{
    RoutingPacket::parsimPack(b);
    doPacking(b,this->SBDRoutingPacketKind_var);
    doPacking(b,this->destSectorHeadID_var);
    b->pack(slot_arraysize);
    doPacking(b,this->slot_var,slot_arraysize);
    b->pack(segmentAddress_arraysize);
    doPacking(b,this->segmentAddress_var,segmentAddress_arraysize);
    doPacking(b,this->nextHop_var);
    doPacking(b,this->prevHop_var);
    doPacking(b,this->sectorSlotNumber_var);
}

void SBDRoutingPacket::parsimUnpack(cCommBuffer *b)
{
    RoutingPacket::parsimUnpack(b);
    doUnpacking(b,this->SBDRoutingPacketKind_var);
    doUnpacking(b,this->destSectorHeadID_var);
    delete [] this->slot_var;
    b->unpack(slot_arraysize);
    if (slot_arraysize==0) {
        this->slot_var = 0;
    } else {
        this->slot_var = new int[slot_arraysize];
        doUnpacking(b,this->slot_var,slot_arraysize);
    }
    delete [] this->segmentAddress_var;
    b->unpack(segmentAddress_arraysize);
    if (segmentAddress_arraysize==0) {
        this->segmentAddress_var = 0;
    } else {
        this->segmentAddress_var = new int[segmentAddress_arraysize];
        doUnpacking(b,this->segmentAddress_var,segmentAddress_arraysize);
    }
    doUnpacking(b,this->nextHop_var);
    doUnpacking(b,this->prevHop_var);
    doUnpacking(b,this->sectorSlotNumber_var);
}

int SBDRoutingPacket::getSBDRoutingPacketKind() const
{
    return SBDRoutingPacketKind_var;
}

void SBDRoutingPacket::setSBDRoutingPacketKind(int SBDRoutingPacketKind_var)
{
    this->SBDRoutingPacketKind_var = SBDRoutingPacketKind_var;
}

int SBDRoutingPacket::getDestSectorHeadID() const
{
    return destSectorHeadID_var;
}

void SBDRoutingPacket::setDestSectorHeadID(int destSectorHeadID_var)
{
    this->destSectorHeadID_var = destSectorHeadID_var;
}

void SBDRoutingPacket::setSlotArraySize(unsigned int size)
{
    int *slot_var2 = (size==0) ? NULL : new int[size];
    unsigned int sz = slot_arraysize < size ? slot_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        slot_var2[i] = this->slot_var[i];
    for (unsigned int i=sz; i<size; i++)
        slot_var2[i] = 0;
    slot_arraysize = size;
    delete [] this->slot_var;
    this->slot_var = slot_var2;
}

unsigned int SBDRoutingPacket::getSlotArraySize() const
{
    return slot_arraysize;
}

int SBDRoutingPacket::getSlot(unsigned int k) const
{
    if (k>=slot_arraysize) throw cRuntimeError("Array of size %d indexed by %d", slot_arraysize, k);
    return slot_var[k];
}

void SBDRoutingPacket::setSlot(unsigned int k, int slot_var)
{
    if (k>=slot_arraysize) throw cRuntimeError("Array of size %d indexed by %d", slot_arraysize, k);
    this->slot_var[k]=slot_var;
}

void SBDRoutingPacket::setSegmentAddressArraySize(unsigned int size)
{
    int *segmentAddress_var2 = (size==0) ? NULL : new int[size];
    unsigned int sz = segmentAddress_arraysize < size ? segmentAddress_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        segmentAddress_var2[i] = this->segmentAddress_var[i];
    for (unsigned int i=sz; i<size; i++)
        segmentAddress_var2[i] = 0;
    segmentAddress_arraysize = size;
    delete [] this->segmentAddress_var;
    this->segmentAddress_var = segmentAddress_var2;
}

unsigned int SBDRoutingPacket::getSegmentAddressArraySize() const
{
    return segmentAddress_arraysize;
}

int SBDRoutingPacket::getSegmentAddress(unsigned int k) const
{
    if (k>=segmentAddress_arraysize) throw cRuntimeError("Array of size %d indexed by %d", segmentAddress_arraysize, k);
    return segmentAddress_var[k];
}

void SBDRoutingPacket::setSegmentAddress(unsigned int k, int segmentAddress_var)
{
    if (k>=segmentAddress_arraysize) throw cRuntimeError("Array of size %d indexed by %d", segmentAddress_arraysize, k);
    this->segmentAddress_var[k]=segmentAddress_var;
}

const char * SBDRoutingPacket::getNextHop() const
{
    return nextHop_var.c_str();
}

void SBDRoutingPacket::setNextHop(const char * nextHop_var)
{
    this->nextHop_var = nextHop_var;
}

const char * SBDRoutingPacket::getPrevHop() const
{
    return prevHop_var.c_str();
}

void SBDRoutingPacket::setPrevHop(const char * prevHop_var)
{
    this->prevHop_var = prevHop_var;
}

int SBDRoutingPacket::getSectorSlotNumber() const
{
    return sectorSlotNumber_var;
}

void SBDRoutingPacket::setSectorSlotNumber(int sectorSlotNumber_var)
{
    this->sectorSlotNumber_var = sectorSlotNumber_var;
}

class SBDRoutingPacketDescriptor : public cClassDescriptor
{
  public:
    SBDRoutingPacketDescriptor();
    virtual ~SBDRoutingPacketDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(SBDRoutingPacketDescriptor);

SBDRoutingPacketDescriptor::SBDRoutingPacketDescriptor() : cClassDescriptor("SBDRoutingPacket", "RoutingPacket")
{
}

SBDRoutingPacketDescriptor::~SBDRoutingPacketDescriptor()
{
}

bool SBDRoutingPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<SBDRoutingPacket *>(obj)!=NULL;
}

const char *SBDRoutingPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int SBDRoutingPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 7+basedesc->getFieldCount(object) : 7;
}

unsigned int SBDRoutingPacketDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<7) ? fieldTypeFlags[field] : 0;
}

const char *SBDRoutingPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "SBDRoutingPacketKind",
        "destSectorHeadID",
        "slot",
        "segmentAddress",
        "nextHop",
        "prevHop",
        "sectorSlotNumber",
    };
    return (field>=0 && field<7) ? fieldNames[field] : NULL;
}

int SBDRoutingPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='S' && strcmp(fieldName, "SBDRoutingPacketKind")==0) return base+0;
    if (fieldName[0]=='d' && strcmp(fieldName, "destSectorHeadID")==0) return base+1;
    if (fieldName[0]=='s' && strcmp(fieldName, "slot")==0) return base+2;
    if (fieldName[0]=='s' && strcmp(fieldName, "segmentAddress")==0) return base+3;
    if (fieldName[0]=='n' && strcmp(fieldName, "nextHop")==0) return base+4;
    if (fieldName[0]=='p' && strcmp(fieldName, "prevHop")==0) return base+5;
    if (fieldName[0]=='s' && strcmp(fieldName, "sectorSlotNumber")==0) return base+6;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *SBDRoutingPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
        "int",
        "int",
        "string",
        "string",
        "int",
    };
    return (field>=0 && field<7) ? fieldTypeStrings[field] : NULL;
}

const char *SBDRoutingPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 0:
            if (!strcmp(propertyname,"enum")) return "SBDRoutingPacketDef";
            return NULL;
        default: return NULL;
    }
}

int SBDRoutingPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    SBDRoutingPacket *pp = (SBDRoutingPacket *)object; (void)pp;
    switch (field) {
        case 2: return pp->getSlotArraySize();
        case 3: return pp->getSegmentAddressArraySize();
        default: return 0;
    }
}

std::string SBDRoutingPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    SBDRoutingPacket *pp = (SBDRoutingPacket *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getSBDRoutingPacketKind());
        case 1: return long2string(pp->getDestSectorHeadID());
        case 2: return long2string(pp->getSlot(i));
        case 3: return long2string(pp->getSegmentAddress(i));
        case 4: return oppstring2string(pp->getNextHop());
        case 5: return oppstring2string(pp->getPrevHop());
        case 6: return long2string(pp->getSectorSlotNumber());
        default: return "";
    }
}

bool SBDRoutingPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    SBDRoutingPacket *pp = (SBDRoutingPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setSBDRoutingPacketKind(string2long(value)); return true;
        case 1: pp->setDestSectorHeadID(string2long(value)); return true;
        case 2: pp->setSlot(i,string2long(value)); return true;
        case 3: pp->setSegmentAddress(i,string2long(value)); return true;
        case 4: pp->setNextHop((value)); return true;
        case 5: pp->setPrevHop((value)); return true;
        case 6: pp->setSectorSlotNumber(string2long(value)); return true;
        default: return false;
    }
}

const char *SBDRoutingPacketDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
    };
    return (field>=0 && field<7) ? fieldStructNames[field] : NULL;
}

void *SBDRoutingPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    SBDRoutingPacket *pp = (SBDRoutingPacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


