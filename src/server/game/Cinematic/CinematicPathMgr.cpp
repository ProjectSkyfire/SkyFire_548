#include "CinematicPathMgr.h"
#include "DBCStores.h"
#include "World.h"
#include <fstream>
#include <algorithm>
#include <stdint.h>
#include "G3D/platform.h"
#include "G3D/CoordinateFrame.h"
#include "G3D/Quat.h"
#include "G3D/Matrix4.h"
#include "G3D/Box.h"
#include "G3D/AABox.h"
#include "G3D/Sphere.h"
#include "G3D/Triangle.h"
#include "G3D/Ray.h"
#include "G3D/Capsule.h"
#include "G3D/Cylinder.h"
#include "G3D/UprightFrame.h"
#include "G3D/Any.h"
#include "G3D/stringutils.h"
#include "G3D/PhysicsFrame.h"
#include "G3D/UprightFrame.h"
#include <ObjectMgr.h>

enum
{
    INTERPOLATION_NONE,
    INTERPOLATION_LINEAR,
    INTERPOLATION_HERMITE,
    INTERPOLATION_BEZIER,
};

#pragma pack(push, 1)

struct Sphere
{
    ///*0x00*/ float Min[3];
    ///*0x0C*/ float Max[3];
    /*0x18*/ float Radius;
};

struct ModelHeader
{
    char id[4];
    uint8_t Version[4];
    uint32_t nameLength;
    uint32_t nameOfs;
    uint32_t GlobalModelFlags;      // 1: tilt x, 2: tilt y, 4:, 8: add another field in header, 16: ; (no other flags as of 3.1.1);

    uint32_t nGlobalSequences;      // AnimationRelated
    uint32_t ofsGlobalSequences;    // A list of timestamps.
    uint32_t nAnimations;           // AnimationRelated
    uint32_t ofsAnimations;         // Information about the animations in the model.
    uint32_t nAnimationLookup;      // AnimationRelated
    uint32_t ofsAnimationLookup;    // Mapping of global IDs to the entries in the Animation sequences block.
    //uint32_t nD;
    //uint32_t ofsD;
    uint32_t nBones;                // BonesAndLookups
    uint32_t ofsBones;              // Information about the bones in this model.
    uint32_t nKeyBoneLookup;        // BonesAndLookups
    uint32_t ofsKeyBoneLookup;      // Lookup table for key skeletal bones.

    uint32_t nVertices;             // GeometryAndRendering
    uint32_t ofsVertices;           // Vertices of the model.
    uint32_t nViews;                // GeometryAndRendering
    //uint32_t ofsViews;            // Views (LOD) are now in .skins.

    uint32_t nColors;               // ColorsAndTransparency
    uint32_t ofsColors;             // Color definitions.

    uint32_t nTextures;             // TextureAndTheifAnimation
    uint32_t ofsTextures;           // Textures of this model.

    uint32_t nTransparency;         // H,  ColorsAndTransparency
    uint32_t ofsTransparency;       // Transparency of textures.
    //uint32_t nI;                  // always unused ?
    //uint32_t ofsI;
    uint32_t nTexAnims;     	    // J, TextureAndTheifAnimation
    uint32_t ofsTexAnims;
    uint32_t nTexReplace;           // TextureAndTheifAnimation
    uint32_t ofsTexReplace;         // Replaceable Textures.

    uint32_t nTexFlags;             // Render Flags
    uint32_t ofsTexFlags;           // Blending modes / render flags.
    uint32_t nBoneLookup;           // BonesAndLookups
    uint32_t ofsBoneLookup;         // A bone lookup table.

    uint32_t nTexLookup;            // TextureAndTheifAnimation
    uint32_t ofsTexLookup;          // The same for textures.

    uint32_t nTexUnitLookup;		// L, TextureAndTheifAnimation, seems gone after Cataclysm
    uint32_t ofsTexUnitLookup;      // And texture units. Somewhere they have to be too.
    uint32_t nTransparencyLookup;   // M, ColorsAndTransparency
    uint32_t ofsTransparencyLookup; // Everything needs its lookup. Here are the transparencies.
    uint32_t nTexAnimLookup;        // TextureAndTheifAnimation
    uint32_t ofsTexAnimLookup;      // Wait. Do we have animated Textures? Wasn't ofsTexAnims deleted? oO

    struct Sphere CollisionSphere;
    struct Sphere BoundSphere;

    uint32_t nBoundingTriangles;    // Miscellaneous
    uint32_t ofsBoundingTriangles;
    uint32_t nBoundingVertices;     // Miscellaneous
    uint32_t ofsBoundingVertices;
    uint32_t nBoundingNormals;      // Miscellaneous
    uint32_t ofsBoundingNormals;

    uint32_t nAttachments;          // O, Miscellaneous
    uint32_t ofsAttachments;        // Attachments are for weapons etc.
    uint32_t nAttachLookup;         // P, Miscellaneous
    uint32_t ofsAttachLookup;       // Of course with a lookup.
    uint32_t nEvents;               // 
    uint32_t ofsEvents;             // Used for playing sounds when dying and a lot else.
    uint32_t nLights;               // R
    uint32_t ofsLights;             // Lights are mainly used in loginscreens but in wands and some doodads too.
    uint32_t CameraCount;           // S, Miscellaneous
    uint32_t CameraOffset;          // The cameras are present in most models for having a model in the Character-Tab.
    uint32_t nCameraLookup;         // Miscellaneous
    uint32_t ofsCameraLookup;       // And lookup-time again, unit16
    uint32_t nRibbonEmitters;       // U, Effects
    uint32_t ofsRibbonEmitters;     // Things swirling around. See the CoT-entrance for light-trails.
    uint32_t nParticleEmitters;     // V, Effects
    uint32_t ofsParticleEmitters;   // Spells and weapons, doodads and loginscreens use them. Blood dripping of a blade? Particles.
};

struct Pair
{
    uint32_t Number;
    uint32_t Offset;
};

struct AnimationBlock
{
    int16_t interpolation_type;
    //int16_t global_sequence_id;
    uint32_t FramePairsCount;
    uint32_t FramePairsOffset;
    uint32_t TransPairsCount;
    uint32_t TransPairsOffset;
};

struct Camera
{
    int32_t Type;
    //float   fov;
    //float   far_clipping;
    //float   near_clipping;
    struct  AnimationBlock SourceTranslations;      // Vector
    float   SourcePosition[3];
    //struct  AnimationBlock target_translations;     // Vector
    //float   target_position[3];
    //struct  AnimationBlock rotations;               // Quaternion
};

struct CameraV10
{
    int32_t Type;
    float   far_clipping;
    float   near_clipping;
    struct  AnimationBlock SourceTranslations;      // Vector
    float   SourcePosition[3];
    struct  AnimationBlock target_translations;     // Vector
    float   target_position[3];
    struct  AnimationBlock rotations;               // Quaternion
    //struct  AnimationBlock unknown_block_4;         // Float
};
#pragma pack(pop)

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Destructor
CinematicSequenceMgr::~CinematicSequenceMgr()
{
    for (std::map<uint32, CinematicSequence*>::iterator l_It = m_Sequences.begin(); l_It != m_Sequences.end(); ++l_It)
        delete l_It->second;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Load all sequence
size_t CinematicSequenceMgr::Load()
{
    uint32 oldMSTime = getMSTime();
    uint32 count = 0;
    m_Sequences.clear();

    for (uint32 l_SequenceID = 0; l_SequenceID < sCinematicSequencesStore.GetNumRows(); ++l_SequenceID)
    {
        CinematicSequencesEntry const* l_SequenceInfo = sCinematicSequencesStore.LookupEntry(l_SequenceID);

        if (!l_SequenceInfo || !l_SequenceInfo->cinematicCamera)
            continue;

        CinematicCameraEntry const* l_CameraEntry = sCinematicCameraStore.LookupEntry(l_SequenceInfo->cinematicCamera);
        if (!l_CameraEntry || !l_CameraEntry->filename[sObjectMgr->GetDBCLocaleIndex()])
            continue;

        std::string l_ModelFileName = l_CameraEntry->filename;
        l_ModelFileName = l_ModelFileName.substr(l_ModelFileName.find('\\') + 1);
        l_ModelFileName = l_ModelFileName.substr(0, l_ModelFileName.find("mdx"));
        l_ModelFileName += "m2";
        l_ModelFileName = sWorld->GetDataPath() + "cameras/" + l_ModelFileName;

        std::transform(l_ModelFileName.begin(), l_ModelFileName.end(), l_ModelFileName.begin(), ::tolower);

        FILE * l_ModelFile = fopen(l_ModelFileName.c_str(), "rb");
        if (!l_ModelFile)
        {
            SF_LOG_INFO("server.loading", "Cinematic camera '%s': does not exist!", l_ModelFileName.c_str());
            continue;
        }

        size_t  l_FileSize = 0;
        char *  l_FileBuffer = 0;
        char    l_MagicHeader[4];

        // Check magic
        fread(l_MagicHeader, 1, sizeof(l_MagicHeader), l_ModelFile);

        if (0 != memcmp(l_MagicHeader, "MD20", sizeof(l_MagicHeader)))
        {
            SF_LOG_INFO("server.loading", "Cinematic camera '%s': invalid magic header!", l_ModelFileName.c_str());
            continue;
        }

        // Get file size
        fseek(l_ModelFile, 0, SEEK_END);
        l_FileSize = ftell(l_ModelFile);
        fseek(l_ModelFile, 0, SEEK_SET);

        // Map file into memory
        l_FileBuffer = (char*)malloc(l_FileSize);
        fread(l_FileBuffer, 1, l_FileSize, l_ModelFile);
        fclose(l_ModelFile);

        // Check number of cameras
        ModelHeader* l_Header = (ModelHeader*)l_FileBuffer;

        if (!l_Header->CameraCount || !l_Header->CameraOffset)
        {
            free(l_FileBuffer);

            SF_LOG_INFO("server.loading", "Cinematic camera '%s': no cameras!", l_ModelFileName.c_str());
            continue;
        }

        AnimationBlock* l_Translations;
        float           l_SourcePosition[3];

        if (l_Header->Version[0] <= 9)
        {
            Camera* l_Camera = (Camera*)(l_FileBuffer + l_Header->CameraOffset);
            Camera* l_EndCamera = l_Camera + l_Header->CameraCount;

            for (; l_Camera != l_EndCamera; ++l_Camera)
            {
                if (l_Camera->Type != -1)
                    continue;

                l_Translations = &l_Camera->SourceTranslations;
                memcpy(l_SourcePosition, l_Camera->SourcePosition, sizeof(l_SourcePosition));

                break;
            }
        }
        else
        {
            CameraV10* l_Camera = (CameraV10*)(l_FileBuffer + l_Header->CameraOffset);
            CameraV10* l_EndCamera = l_Camera + l_Header->CameraCount;

            for (; l_Camera != l_EndCamera; ++l_Camera)
            {
                if (l_Camera->Type != -1)
                    continue;

                l_Translations = &l_Camera->SourceTranslations;
                memcpy(l_SourcePosition, l_Camera->SourcePosition, sizeof(l_SourcePosition));

                break;
            }
        }

        if (!l_Translations ||
            !l_Translations->FramePairsOffset || !l_Translations->TransPairsOffset
            || l_Translations->FramePairsCount != l_Translations->TransPairsCount)
        {
            SF_LOG_INFO("server.loading", "Cinematic camera '%s': no translations!", l_ModelFileName.c_str());
            free(l_FileBuffer);
            continue;
        }

        CinematicSequence * l_Sequence = new CinematicSequence();

        l_Sequence->Duration = 0;
        l_Sequence->KeyFramesCount = 0;

        size_t l_BlockSize = 3 * sizeof(float);

        if (l_Translations->interpolation_type == INTERPOLATION_HERMITE || l_Translations->interpolation_type == INTERPOLATION_BEZIER)
            l_BlockSize *= 3;

        G3D::Matrix3 l_Mat = G3D::Matrix3::identity();
        l_Mat = l_Mat.fromAxisAngle(G3D::Vector3(0, 0, 1), l_CameraEntry->GlobalRotation);

        if (l_Translations->FramePairsOffset)
        {
            size_t l_Index, l_SubIndex;

            Pair *l_FramePairs = (struct Pair*)(l_FileBuffer + l_Translations->FramePairsOffset);
            Pair *l_TransPairs = (struct Pair*)(l_FileBuffer + l_Translations->TransPairsOffset);

            l_Sequence->KeyFramesCount = l_FramePairs->Number;
            l_Sequence->KeyFrames = new CinematicKeyFrame[l_Sequence->KeyFramesCount];

            for (l_Index = 0; l_Index < l_Translations->FramePairsCount; l_Index += 1)
            {
                Pair * l_FramePair = l_FramePairs + l_Index;
                Pair * l_TransPair = l_TransPairs + l_Index;

                for (l_SubIndex = 0; l_SubIndex < l_FramePair->Number; l_SubIndex += 1)
                {
                    l_Sequence->KeyFrames[l_SubIndex].Frame = ((uint32_t*)(l_FileBuffer + l_FramePair->Offset))[l_SubIndex];
                    l_Sequence->Duration = std::max(l_Sequence->Duration, l_Sequence->KeyFrames[l_SubIndex].Frame);

                    memcpy(l_Sequence->KeyFrames[l_SubIndex].Position, (l_FileBuffer + l_TransPair->Offset) + l_SubIndex * l_BlockSize, sizeof(l_Sequence->KeyFrames[l_SubIndex].Position));
                    l_Sequence->KeyFrames[l_SubIndex].Position[0] = l_SourcePosition[0] + l_Sequence->KeyFrames[l_SubIndex].Position[0];
                    l_Sequence->KeyFrames[l_SubIndex].Position[1] = l_SourcePosition[1] + l_Sequence->KeyFrames[l_SubIndex].Position[1];
                    l_Sequence->KeyFrames[l_SubIndex].Position[2] = l_SourcePosition[2] + l_Sequence->KeyFrames[l_SubIndex].Position[2];

                    G3D::Vector3 l_Pt(l_Sequence->KeyFrames[l_SubIndex].Position[0], l_Sequence->KeyFrames[l_SubIndex].Position[1], l_Sequence->KeyFrames[l_SubIndex].Position[2]);
                    G3D::Vector3 l_Result = l_Mat * l_Pt;

                    l_Sequence->KeyFrames[l_SubIndex].Position[0] = l_CameraEntry->EndPosX + l_Result.x;
                    l_Sequence->KeyFrames[l_SubIndex].Position[1] = l_CameraEntry->EndPosY + l_Result.y;
                    l_Sequence->KeyFrames[l_SubIndex].Position[2] = l_CameraEntry->EndPosZ + l_Result.z;
                }
            }
        }

        m_Sequences[l_SequenceID] = l_Sequence;

        free(l_FileBuffer);

        count++;
    }

    SF_LOG_INFO("server.loading", ">> Loaded %u Cinematic cameras in %u ms", count, GetMSTimeDiffToNow(oldMSTime));

    return m_Sequences.size();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Get a sequence by his id
CinematicSequence const* CinematicSequenceMgr::GetSequence(uint32 p_SequenceID)
{
    std::map<uint32, CinematicSequence*>::iterator l_It = m_Sequences.find(p_SequenceID);

    return (l_It != m_Sequences.end()) ? l_It->second : NULL;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Constructor
CinematicSequence::CinematicSequence()
{
    KeyFrames = NULL;
}
/// Destructor
CinematicSequence::~CinematicSequence()
{
    if (KeyFrames)
        delete[] KeyFrames;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Get position at time
void CinematicSequence::GetPositionAtTime(uint32 p_Time, float* p_X, float* p_Y, float* p_Z) const
{
    *p_X = 0;
    *p_Y = 0;
    *p_Z = 0;

    for (CinematicKeyFrame* l_KeyFrame = KeyFrames; l_KeyFrame != KeyFrames + KeyFramesCount; ++l_KeyFrame)
    {
        CinematicKeyFrame* l_NextKeyFrame = (l_KeyFrame + 1 != KeyFrames + KeyFramesCount) ? l_KeyFrame + 1 : NULL;

        if (!l_NextKeyFrame || l_KeyFrame->Frame == p_Time)
        {
            *p_X = l_KeyFrame->Position[0];
            *p_Y = l_KeyFrame->Position[1];
            *p_Z = l_KeyFrame->Position[2];

            return;
        }
        else if (l_NextKeyFrame && l_KeyFrame->Frame < p_Time && l_NextKeyFrame->Frame > p_Time)
        {
            /// no divide by 0
            if (l_NextKeyFrame->Frame == l_KeyFrame->Frame)
            {
                *p_X = l_KeyFrame->Position[0];
                *p_Y = l_KeyFrame->Position[1];
                *p_Z = l_KeyFrame->Position[2];
                return;
            }

            float l_Progress = (float(p_Time) - float(l_KeyFrame->Frame)) / (float(l_NextKeyFrame->Frame) - float(l_KeyFrame->Frame));

            *p_X = l_KeyFrame->Position[0] + (l_Progress * (l_NextKeyFrame->Position[0] - l_KeyFrame->Position[0]));
            *p_Y = l_KeyFrame->Position[1] + (l_Progress * (l_NextKeyFrame->Position[1] - l_KeyFrame->Position[1]));
            *p_Z = l_KeyFrame->Position[2] + (l_Progress * (l_NextKeyFrame->Position[2] - l_KeyFrame->Position[2]));

            return;
        }
    }

    CinematicKeyFrame * l_LastGood = 0;

    for (CinematicKeyFrame* l_KeyFrame = KeyFrames; l_KeyFrame != KeyFrames + KeyFramesCount; ++l_KeyFrame)
    {
        if (l_KeyFrame->Frame < p_Time)
            l_LastGood = l_KeyFrame;
    }

    if (l_LastGood)
    {
        *p_X = l_LastGood->Position[0];
        *p_Y = l_LastGood->Position[1];
        *p_Z = l_LastGood->Position[2];
    }
}