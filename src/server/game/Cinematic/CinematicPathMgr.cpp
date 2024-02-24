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
    uint8 Version[4];
    uint32 nameLength;
    uint32 nameOfs;
    uint32 GlobalModelFlags;      // 1: tilt x, 2: tilt y, 4:, 8: add another field in header, 16: ; (no other flags as of 3.1.1);

    uint32 nGlobalSequences;      // AnimationRelated
    uint32 ofsGlobalSequences;    // A list of timestamps.
    uint32 nAnimations;           // AnimationRelated
    uint32 ofsAnimations;         // Information about the animations in the model.
    uint32 nAnimationLookup;      // AnimationRelated
    uint32 ofsAnimationLookup;    // Mapping of global IDs to the entries in the Animation sequences block.
    //uint32 nD;
    //uint32 ofsD;
    uint32 nBones;                // BonesAndLookups
    uint32 ofsBones;              // Information about the bones in this model.
    uint32 nKeyBoneLookup;        // BonesAndLookups
    uint32 ofsKeyBoneLookup;      // Lookup table for key skeletal bones.

    uint32 nVertices;             // GeometryAndRendering
    uint32 ofsVertices;           // Vertices of the model.
    uint32 nViews;                // GeometryAndRendering
    //uint32 ofsViews;            // Views (LOD) are now in .skins.

    uint32 nColors;               // ColorsAndTransparency
    uint32 ofsColors;             // Color definitions.

    uint32 nTextures;             // TextureAndTheifAnimation
    uint32 ofsTextures;           // Textures of this model.

    uint32 nTransparency;         // H,  ColorsAndTransparency
    uint32 ofsTransparency;       // Transparency of textures.
    //uint32 nI;                  // always unused ?
    //uint32 ofsI;
    uint32 nTexAnims;     	    // J, TextureAndTheifAnimation
    uint32 ofsTexAnims;
    uint32 nTexReplace;           // TextureAndTheifAnimation
    uint32 ofsTexReplace;         // Replaceable Textures.

    uint32 nTexFlags;             // Render Flags
    uint32 ofsTexFlags;           // Blending modes / render flags.
    uint32 nBoneLookup;           // BonesAndLookups
    uint32 ofsBoneLookup;         // A bone lookup table.

    uint32 nTexLookup;            // TextureAndTheifAnimation
    uint32 ofsTexLookup;          // The same for textures.

    uint32 nTexUnitLookup;		// L, TextureAndTheifAnimation, seems gone after Cataclysm
    uint32 ofsTexUnitLookup;      // And texture units. Somewhere they have to be too.
    uint32 nTransparencyLookup;   // M, ColorsAndTransparency
    uint32 ofsTransparencyLookup; // Everything needs its lookup. Here are the transparencies.
    uint32 nTexAnimLookup;        // TextureAndTheifAnimation
    uint32 ofsTexAnimLookup;      // Wait. Do we have animated Textures? Wasn't ofsTexAnims deleted? oO

    struct Sphere CollisionSphere;
    struct Sphere BoundSphere;

    uint32 nBoundingTriangles;    // Miscellaneous
    uint32 ofsBoundingTriangles;
    uint32 nBoundingVertices;     // Miscellaneous
    uint32 ofsBoundingVertices;
    uint32 nBoundingNormals;      // Miscellaneous
    uint32 ofsBoundingNormals;

    uint32 nAttachments;          // O, Miscellaneous
    uint32 ofsAttachments;        // Attachments are for weapons etc.
    uint32 nAttachLookup;         // P, Miscellaneous
    uint32 ofsAttachLookup;       // Of course with a lookup.
    uint32 nEvents;               // 
    uint32 ofsEvents;             // Used for playing sounds when dying and a lot else.
    uint32 nLights;               // R
    uint32 ofsLights;             // Lights are mainly used in loginscreens but in wands and some doodads too.
    uint32 CameraCount;           // S, Miscellaneous
    uint32 CameraOffset;          // The cameras are present in most models for having a model in the Character-Tab.
    uint32 nCameraLookup;         // Miscellaneous
    uint32 ofsCameraLookup;       // And lookup-time again, unit16
    uint32 nRibbonEmitters;       // U, Effects
    uint32 ofsRibbonEmitters;     // Things swirling around. See the CoT-entrance for light-trails.
    uint32 nParticleEmitters;     // V, Effects
    uint32 ofsParticleEmitters;   // Spells and weapons, doodads and loginscreens use them. Blood dripping of a blade? Particles.
};

struct Pair
{
    uint32_t Number;
    uint32_t Offset;
};

struct AnimationBlock
{
    int16 interpolation_type;
    //int16 global_sequence_id;
    uint32 FramePairsCount;
    uint32 FramePairsOffset;
    uint32 TransPairsCount;
    uint32 TransPairsOffset;
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
    int32 Type;
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
    for (std::map<uint32, CinematicSequence*>::iterator itr = m_Sequences.begin(); itr != m_Sequences.end(); ++itr)
        delete itr->second;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Load all sequence
size_t CinematicSequenceMgr::Load()
{
    uint32 oldMSTime = getMSTime();
    uint32 count = 0;
    m_Sequences.clear();

    for (uint32 itr = 0; itr < sCinematicSequencesStore.GetNumRows(); itr++)
    {
        CinematicSequencesEntry const* cinematicSequecenes = sCinematicSequencesStore.LookupEntry(itr);

        if (!cinematicSequecenes || !cinematicSequecenes->cinematicCamera)
            continue;

        CinematicCameraEntry const* cinematicCamera = sCinematicCameraStore.LookupEntry(cinematicSequecenes->cinematicCamera);
        if (!cinematicCamera || !cinematicCamera->filename[sObjectMgr->GetDBCLocaleIndex()])
            continue;

        std::string modelFileName = cinematicCamera->filename;
        modelFileName = modelFileName.substr(modelFileName.find('\\') + 1);
        modelFileName = modelFileName.substr(0, modelFileName.find("mdx"));
        modelFileName += "m2";
        modelFileName = sWorld->GetDataPath() + "cameras/" + modelFileName;

        std::transform(modelFileName.begin(), modelFileName.end(), modelFileName.begin(), ::tolower);

        FILE * modelFile = fopen(modelFileName.c_str(), "rb");
        if (!modelFile)
        {
            SF_LOG_INFO("server.loading", "Cinematic camera '%s': does not exist!", modelFileName.c_str());
            continue;
        }

        size_t  fileSize = 0;
        char *  fileBuffer = 0;
        char    magicHeader[4];

        // Check magic
        fread(magicHeader, 1, sizeof(magicHeader), modelFile);

        if (0 != memcmp(magicHeader, "MD20", sizeof(magicHeader)))
        {
            SF_LOG_INFO("server.loading", "Cinematic camera '%s': invalid magic header!", modelFileName.c_str());
            continue;
        }

        // Get file size
        fseek(modelFile, 0, SEEK_END);
        fileSize = ftell(modelFile);
        fseek(modelFile, 0, SEEK_SET);

        // Map file into memory
        fileBuffer = (char*)malloc(fileSize);
        fread(fileBuffer, 1, fileSize, modelFile);
        fclose(modelFile);

        // Check number of cameras
        ModelHeader* modelHeader = (ModelHeader*)fileBuffer;

        if (!modelHeader->CameraCount || !modelHeader->CameraOffset)
        {
            free(fileBuffer);

            SF_LOG_INFO("server.loading", "Cinematic camera '%s': no cameras!", modelFileName.c_str());
            continue;
        }

        AnimationBlock* animationBlock;
        float           sourcePosition[3];

        if (modelHeader->Version[0] <= 9)
        {
            Camera* camera = (Camera*)(fileBuffer + modelHeader->CameraOffset);
            Camera* endCamera = camera + modelHeader->CameraCount;

            for (; camera != endCamera; ++camera)
            {
                if (camera->Type != -1)
                    continue;

                animationBlock = &camera->SourceTranslations;
                memcpy(sourcePosition, camera->SourcePosition, sizeof(sourcePosition));

                break;
            }
        }
        else
        {
            CameraV10* cameraV10 = (CameraV10*)(fileBuffer + modelHeader->CameraOffset);
            CameraV10* endCameraV10 = cameraV10 + modelHeader->CameraCount;

            for (; cameraV10 != endCameraV10; ++cameraV10)
            {
                if (cameraV10->Type != -1)
                    continue;

                animationBlock = &cameraV10->SourceTranslations;
                memcpy(sourcePosition, cameraV10->SourcePosition, sizeof(sourcePosition));

                break;
            }
        }

        if (!animationBlock ||
            !animationBlock->FramePairsOffset || !animationBlock->TransPairsOffset
            || animationBlock->FramePairsCount != animationBlock->TransPairsCount)
        {
            SF_LOG_INFO("server.loading", "Cinematic camera '%s': no translations!", modelFileName.c_str());
            free(fileBuffer);
            continue;
        }

        CinematicSequence * cinematicSequence = new CinematicSequence();

        cinematicSequence->Duration = 0;
        cinematicSequence->KeyFramesCount = 0;

        size_t blockSize = 3 * sizeof(float);

        if (animationBlock->interpolation_type == INTERPOLATION_HERMITE || animationBlock->interpolation_type == INTERPOLATION_BEZIER)
            blockSize *= 3;

        G3D::Matrix3 matrix = G3D::Matrix3::identity();
        matrix = matrix.fromAxisAngle(G3D::Vector3(0, 0, 1), cinematicCamera->GlobalRotation);

        if (animationBlock->FramePairsOffset)
        {
            Pair* framePairs = (struct Pair*)(fileBuffer + animationBlock->FramePairsOffset);
            Pair* transPairs = (struct Pair*)(fileBuffer + animationBlock->TransPairsOffset);

            cinematicSequence->KeyFramesCount = framePairs->Number;
            cinematicSequence->KeyFrames = new CinematicKeyFrame[cinematicSequence->KeyFramesCount];

            for (uint32 i = 0; i < animationBlock->FramePairsCount; i++)
            {
                Pair * framePair = framePairs + i;
                Pair * transPair = transPairs + i;

                for (uint32 j = 0; j < framePair->Number; j++)
                {
                    cinematicSequence->KeyFrames[j].Frame = ((uint32*)(fileBuffer + framePair->Offset))[j];
                    cinematicSequence->Duration = std::max(cinematicSequence->Duration, cinematicSequence->KeyFrames[j].Frame);

                    memcpy(cinematicSequence->KeyFrames[j].Position, (fileBuffer + transPair->Offset) + j * blockSize, sizeof(cinematicSequence->KeyFrames[j].Position));
                    cinematicSequence->KeyFrames[j].Position[0] = sourcePosition[0] + cinematicSequence->KeyFrames[j].Position[0];
                    cinematicSequence->KeyFrames[j].Position[1] = sourcePosition[1] + cinematicSequence->KeyFrames[j].Position[1];
                    cinematicSequence->KeyFrames[j].Position[2] = sourcePosition[2] + cinematicSequence->KeyFrames[j].Position[2];

                    G3D::Vector3 pt(cinematicSequence->KeyFrames[j].Position[0], cinematicSequence->KeyFrames[j].Position[1], cinematicSequence->KeyFrames[j].Position[2]);
                    G3D::Vector3 result = matrix * pt;

                    cinematicSequence->KeyFrames[j].Position[0] = cinematicCamera->EndPosX + result.x;
                    cinematicSequence->KeyFrames[j].Position[1] = cinematicCamera->EndPosY + result.y;
                    cinematicSequence->KeyFrames[j].Position[2] = cinematicCamera->EndPosZ + result.z;
                }
            }
        }

        m_Sequences[itr] = cinematicSequence;

        free(fileBuffer);

        count++;
    }

    SF_LOG_INFO("server.loading", ">> Loaded %u Cinematic cameras in %u ms", count, GetMSTimeDiffToNow(oldMSTime));

    return m_Sequences.size();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Get a sequence by his id
CinematicSequence* CinematicSequenceMgr::GetSequence(uint32 sequenceID)
{
    std::map<uint32, CinematicSequence*>::iterator itr = m_Sequences.find(sequenceID);

    return (itr != m_Sequences.end()) ? itr->second : NULL;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Destructor
CinematicSequence::~CinematicSequence()
{
    if (KeyFrames)
        delete[] KeyFrames;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Get position at time
void CinematicSequence::GetPositionAtTime(uint32 time, float* x, float* y, float* z) const
{
    *x = 0;
    *y = 0;
    *z = 0;

    for (CinematicKeyFrame* cinematicKeyFrame = KeyFrames; cinematicKeyFrame != KeyFrames + KeyFramesCount; ++cinematicKeyFrame)
    {
        CinematicKeyFrame* nextCinematicKeyFrame = (cinematicKeyFrame + 1 != KeyFrames + KeyFramesCount) ? cinematicKeyFrame + 1 : NULL;

        if (!nextCinematicKeyFrame || cinematicKeyFrame->Frame == time)
        {
            *x = cinematicKeyFrame->Position[0];
            *y = cinematicKeyFrame->Position[1];
            *z = cinematicKeyFrame->Position[2];

            return;
        }
        else if (nextCinematicKeyFrame && cinematicKeyFrame->Frame < time && nextCinematicKeyFrame->Frame > time)
        {
            /// no divide by 0
            if (nextCinematicKeyFrame->Frame == cinematicKeyFrame->Frame)
            {
                *x = cinematicKeyFrame->Position[0];
                *y = cinematicKeyFrame->Position[1];
                *z = cinematicKeyFrame->Position[2];
                return;
            }

            float l_Progress = (float(time) - float(cinematicKeyFrame->Frame)) / (float(nextCinematicKeyFrame->Frame) - float(cinematicKeyFrame->Frame));

            *x = cinematicKeyFrame->Position[0] + (l_Progress * (nextCinematicKeyFrame->Position[0] - cinematicKeyFrame->Position[0]));
            *y = cinematicKeyFrame->Position[1] + (l_Progress * (nextCinematicKeyFrame->Position[1] - cinematicKeyFrame->Position[1]));
            *z = cinematicKeyFrame->Position[2] + (l_Progress * (nextCinematicKeyFrame->Position[2] - cinematicKeyFrame->Position[2]));

            return;
        }
    }

    CinematicKeyFrame* lastGood = 0;

    for (CinematicKeyFrame* cinematicKeyFrame = KeyFrames; cinematicKeyFrame != KeyFrames + KeyFramesCount; ++cinematicKeyFrame)
    {
        if (cinematicKeyFrame->Frame < time)
            lastGood = cinematicKeyFrame;
    }

    if (lastGood)
    {
        *x = lastGood->Position[0];
        *y = lastGood->Position[1];
        *z = lastGood->Position[2];
    }
}