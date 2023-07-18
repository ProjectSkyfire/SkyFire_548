#ifndef _CINEMATIC_SEQUENCE_MGR
#define _CINEMATIC_SEQUENCE_MGR

#include "Common.h"
#include "Config.h"
#include "Object.h"
#include <ace/Singleton.h>

/// Cinematic sequence key frame
struct CinematicKeyFrame
{
    uint32 Frame;
    float Position[3];
};

/// Cinematic sequence
class CinematicSequence
{
public:
    /// Constructor
    CinematicSequence();
    /// Destructor
    ~CinematicSequence();

    /// Get position at time
    void GetPositionAtTime(uint32 p_Time, float * p_X, float * p_Y, float * p_Z) const;

    /// Sequence durations
    uint32 Duration;
    /// Key Frame count
    size_t KeyFramesCount;
    /// Key frames
    CinematicKeyFrame* KeyFrames;
};

/// Cinematic sequence Mgr
class CinematicSequenceMgr
{
    /// Singleton access
    friend class ACE_Singleton < CinematicSequenceMgr, ACE_Null_Mutex > ;
    /// Constructor
    CinematicSequenceMgr();
    /// Destructor
    ~CinematicSequenceMgr();

public:
    /// Load all sequence
    size_t Load();

    /// Get a sequence by his id
    CinematicSequence const* GetSequence(uint32 p_SequenceID);

private:
    std::map<uint32, CinematicSequence*> m_Sequences;
};

/// Singleton
#define sCinematicSequenceMgr ACE_Singleton<CinematicSequenceMgr, ACE_Null_Mutex>::instance()

#endif /* MIGHT4_CINEMATIC_SEQUENCE_MGR */