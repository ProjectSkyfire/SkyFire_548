/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "CinematicPathMgr.h"
#include "CinematicCameraModel.h"
#include "DBCStores.h"
#include "G3D/AABox.h"
#include "G3D/Any.h"
#include "G3D/Box.h"
#include "G3D/Capsule.h"
#include "G3D/CoordinateFrame.h"
#include "G3D/Cylinder.h"
#include "G3D/Matrix4.h"
#include "G3D/PhysicsFrame.h"
#include "G3D/platform.h"
#include "G3D/Quat.h"
#include "G3D/Ray.h"
#include "G3D/Sphere.h"
#include "G3D/stringutils.h"
#include "G3D/Triangle.h"
#include "G3D/UprightFrame.h"
#include "G3D/UprightFrame.h"
#include "World.h"
#include <algorithm>
#include <fstream>
#include <memory>
#include <new>
#include <ObjectMgr.h>
#include <stdint.h>

using namespace Skyfire::Cinematics;

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
    for (auto const& sequence : m_Sequences)
        delete sequence.second;
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

        FILE* modelFile = fopen(modelFileName.c_str(), "rb");
        if (!modelFile)
        {
            SF_LOG_INFO("server.loading", "Cinematic camera '%s': does not exist!", modelFileName.c_str());
            continue;
        }

        std::unique_ptr<FILE, decltype(&fclose)> file(modelFile, fclose);
        char magicHeader[4];
        if (fread(magicHeader, 1, sizeof(magicHeader), file.get()) != sizeof(magicHeader) ||
            memcmp(magicHeader, "MD20", sizeof(magicHeader)))
        {
            SF_LOG_INFO("server.loading", "Cinematic camera '%s': invalid magic header!", modelFileName.c_str());
            continue;
        }

        if (fseek(file.get(), 0, SEEK_END) != 0)
        {
            SF_LOG_INFO("server.loading", "Cinematic camera '%s': cannot seek!", modelFileName.c_str());
            continue;
        }
        long length = ftell(file.get());
        if (length < 0 || static_cast<unsigned long>(length) < sizeof(ModelHeader) ||
            fseek(file.get(), 0, SEEK_SET) != 0)
        {
            SF_LOG_INFO("server.loading", "Cinematic camera '%s': invalid file size!", modelFileName.c_str());
            continue;
        }

        size_t fileSize = static_cast<size_t>(length);
        std::unique_ptr<char, decltype(&free)> buffer(static_cast<char*>(malloc(fileSize)), free);
        if (!buffer)
        {
            SF_LOG_INFO("server.loading", "Cinematic camera '%s': allocation failed!", modelFileName.c_str());
            continue;
        }
        size_t bytesRead = fread(buffer.get(), 1, fileSize, file.get());
        bool readError = ferror(file.get()) != 0;
        file.reset();
        if (readError || bytesRead != fileSize)
        {
            SF_LOG_INFO("server.loading", "Cinematic camera '%s': incomplete read, skipped!", modelFileName.c_str());
            continue;
        }

        char const* fileBuffer = buffer.get();
        CameraModelView model;
        if (!ValidateCameraModel(fileBuffer, fileSize, model))
        {
            SF_LOG_INFO("server.loading", "Cinematic camera '%s': invalid camera data, skipped!", modelFileName.c_str());
            continue;
        }
        AnimationBlock const* animationBlock = model.Animation;
        float const* sourcePosition = model.SourcePosition;
        size_t blockSize = model.BlockSize;
        std::unique_ptr<CinematicSequence> cinematicSequence(new (std::nothrow) CinematicSequence());
        if (!cinematicSequence)
        {
            SF_LOG_INFO("server.loading", "Cinematic camera '%s': sequence allocation failed!", modelFileName.c_str());
            continue;
        }
        cinematicSequence->KeyFramesCount = model.KeyFramesCount;
        cinematicSequence->KeyFrames = new (std::nothrow) CinematicKeyFrame[model.KeyFramesCount]();
        if (!cinematicSequence->KeyFrames)
        {
            SF_LOG_INFO("server.loading", "Cinematic camera '%s': keyframe allocation failed!", modelFileName.c_str());
            continue;
        }

        G3D::Matrix3 matrix = G3D::Matrix3::identity();
        matrix = matrix.fromAxisAngle(G3D::Vector3(0, 0, 1), cinematicCamera->GlobalRotation);

        if (animationBlock->FramePairsOffset)
        {
            Pair const* framePairs = reinterpret_cast<Pair const*>(fileBuffer + animationBlock->FramePairsOffset);
            Pair const* transPairs = reinterpret_cast<Pair const*>(fileBuffer + animationBlock->TransPairsOffset);

            for (uint32 i = 0; i < animationBlock->FramePairsCount; i++)
            {
                Pair const* framePair = framePairs + i;
                Pair const* transPair = transPairs + i;

                for (uint32 j = 0; j < framePair->Number; j++)
                {
                    memcpy(&cinematicSequence->KeyFrames[j].Frame, fileBuffer + framePair->Offset + size_t(j) * sizeof(uint32), sizeof(uint32));
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

        m_Sequences[itr] = cinematicSequence.release();

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
