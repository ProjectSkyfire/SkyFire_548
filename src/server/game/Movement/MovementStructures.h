/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef SF_MOVEMENT_STRUCTURES_H
#define SF_MOVEMENT_STRUCTURES_H

#include "Opcodes.h"
#include "Object.h"

class ByteBuffer;
class Unit;

enum MovementStatusElements
{
    MSEForcesCount,
    MSEHasCounter,
    MSEHasGuidByte0,
    MSEHasGuidByte1,
    MSEHasGuidByte2,
    MSEHasGuidByte3,
    MSEHasGuidByte4,
    MSEHasGuidByte5,
    MSEHasGuidByte6,
    MSEHasGuidByte7,
    MSEHasMountDisplayId,
    MSEHasMovementFlags,
    MSEHasMovementFlags2,
    MSEHasTimestamp,
    MSEHasOrientation,
    MSEHasTransportData,
    MSEHasTransportGuidByte0,
    MSEHasTransportGuidByte1,
    MSEHasTransportGuidByte2,
    MSEHasTransportGuidByte3,
    MSEHasTransportGuidByte4,
    MSEHasTransportGuidByte5,
    MSEHasTransportGuidByte6,
    MSEHasTransportGuidByte7,
    MSEHasTransportTime2,
    MSEHasTransportTime3,
    MSEHasTransportVehicleId,
    MSEHasPitch,
    MSEHasFallData,
    MSEHasFallDirection,
    MSEHasSplineElevation,
    MSEHasSpline,

    MSEForces,
    MSECount,
    MSECounter,
    MSEGuidByte0,
    MSEGuidByte1,
    MSEGuidByte2,
    MSEGuidByte3,
    MSEGuidByte4,
    MSEGuidByte5,
    MSEGuidByte6,
    MSEGuidByte7,
    MSEMountDisplayIdWithCheck,
    MSEMountDisplayIdWithoutCheck,
    MSEMovementFlags,
    MSEMovementFlags2,
    MSETimestamp,
    MSEPositionX,
    MSEPositionY,
    MSEPositionZ,
    MSEOrientation,
    MSETransportGuidByte0,
    MSETransportGuidByte1,
    MSETransportGuidByte2,
    MSETransportGuidByte3,
    MSETransportGuidByte4,
    MSETransportGuidByte5,
    MSETransportGuidByte6,
    MSETransportGuidByte7,
    MSETransportPositionX,
    MSETransportPositionY,
    MSETransportPositionZ,
    MSETransportOrientation,
    MSETransportSeat,
    MSETransportTime,
    MSETransportTime2,
    MSETransportTime3,
    MSETransportVehicleId,
    MSEPitch,
    MSEFallTime,
    MSEFallVerticalSpeed,
    MSEFallCosAngle,
    MSEFallSinAngle,
    MSEFallHorizontalSpeed,
    MSESplineElevation,

    MSEUintCount,

    MSEAckCount,
    MSEFlushBits,

    // Special
    MSEZeroBit,         // writes bit value 1 or skips read bit
    MSEOneBit,          // writes bit value 0 or skips read bit
    MSEEnd,             // marks end of parsing
    MSEExtraElement,    // Used to signalize reading into ExtraMovementStatusElement, element sequence inside it is declared as separate array
                        // Allowed internal elements are: GUID markers (not transport), MSEExtraFloat, MSEExtraInt8
    MSEExtraFloat,
    MSEExtraFloat2,
    MSEExtraInt8,
    MSEExtraInt32,
    MSEExtra2Bits,
};

namespace Movement
{
    class PacketSender;

    class ExtraMovementStatusElement
    {
        friend class PacketSender;

    public:
        ExtraMovementStatusElement(MovementStatusElements const* elements) : _elements(elements), _index(0) { }

        void ReadNextElement(ByteBuffer& packet);
        void WriteNextElement(ByteBuffer& packet);

        struct
        {
            ObjectGuid guid;
            int8  byteData;
            int32 extraInt32Data;
            uint32 extra2BitsData;
            float floatData;
            float floatData2;
        } Data;

    protected:
        void ResetIndex() { _index = 0; }

    private:
        MovementStatusElements const* _elements;
        uint32 _index;
    };

    class PacketSender
    {
    public:
        PacketSender(Unit* unit, Opcodes serverControl, Opcodes playerControl, Opcodes broadcast = SMSG_PLAYER_MOVE, ExtraMovementStatusElement* extras = NULL);

        void Send() const;

    private:
        ExtraMovementStatusElement* _extraElements;
        Unit* _unit;
        Opcodes _selfOpcode;
        Opcodes _broadcast;
    };

    bool PrintInvalidSequenceElement(MovementStatusElements element, char const* function);
}

MovementStatusElements const* GetMovementStatusElementsSequence(Opcodes opcode);

#endif
