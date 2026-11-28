/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_CINEMATIC_CAMERA_MODEL_H
#define SKYFIRE_CINEMATIC_CAMERA_MODEL_H

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstring>

namespace Skyfire
{
namespace Cinematics
{
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
    /*0x00*/ float Min[3];
    /*0x0C*/ float Max[3];
    /*0x18*/ float Radius;
};

struct ModelHeader
{
    char id[4];
    std::uint8_t Version[4];
    std::uint32_t nameLength;
    std::uint32_t nameOfs;
    std::uint32_t GlobalModelFlags;      // 1: tilt x, 2: tilt y, 4:, 8: add another field in header, 16: ; (no other flags as of 3.1.1);

    std::uint32_t nGlobalSequences;      // AnimationRelated
    std::uint32_t ofsGlobalSequences;    // A list of timestamps.
    std::uint32_t nAnimations;           // AnimationRelated
    std::uint32_t ofsAnimations;         // Information about the animations in the model.
    std::uint32_t nAnimationLookup;      // AnimationRelated
    std::uint32_t ofsAnimationLookup;    // Mapping of global IDs to the entries in the Animation sequences block.
    //std::uint32_t nD;
    //std::uint32_t ofsD;
    std::uint32_t nBones;                // BonesAndLookups
    std::uint32_t ofsBones;              // Information about the bones in this model.
    std::uint32_t nKeyBoneLookup;        // BonesAndLookups
    std::uint32_t ofsKeyBoneLookup;      // Lookup table for key skeletal bones.

    std::uint32_t nVertices;             // GeometryAndRendering
    std::uint32_t ofsVertices;           // Vertices of the model.
    std::uint32_t nViews;                // GeometryAndRendering
    //std::uint32_t ofsViews;            // Views (LOD) are now in .skins.

    std::uint32_t nColors;               // ColorsAndTransparency
    std::uint32_t ofsColors;             // Color definitions.

    std::uint32_t nTextures;             // TextureAndTheifAnimation
    std::uint32_t ofsTextures;           // Textures of this model.

    std::uint32_t nTransparency;         // H,  ColorsAndTransparency
    std::uint32_t ofsTransparency;       // Transparency of textures.
    //std::uint32_t nI;                  // always unused ?
    //std::uint32_t ofsI;
    std::uint32_t nTexAnims;             // J, TextureAndTheifAnimation
    std::uint32_t ofsTexAnims;
    std::uint32_t nTexReplace;           // TextureAndTheifAnimation
    std::uint32_t ofsTexReplace;         // Replaceable Textures.

    std::uint32_t nTexFlags;             // Render Flags
    std::uint32_t ofsTexFlags;           // Blending modes / render flags.
    std::uint32_t nBoneLookup;           // BonesAndLookups
    std::uint32_t ofsBoneLookup;         // A bone lookup table.

    std::uint32_t nTexLookup;            // TextureAndTheifAnimation
    std::uint32_t ofsTexLookup;          // The same for textures.

    std::uint32_t nTexUnitLookup;        // L, TextureAndTheifAnimation, seems gone after Cataclysm
    std::uint32_t ofsTexUnitLookup;      // And texture units. Somewhere they have to be too.
    std::uint32_t nTransparencyLookup;   // M, ColorsAndTransparency
    std::uint32_t ofsTransparencyLookup; // Everything needs its lookup. Here are the transparencies.
    std::uint32_t nTexAnimLookup;        // TextureAndTheifAnimation
    std::uint32_t ofsTexAnimLookup;      // Wait. Do we have animated Textures? Wasn't ofsTexAnims deleted? oO

    struct Sphere CollisionSphere;
    struct Sphere BoundSphere;

    std::uint32_t nBoundingTriangles;    // Miscellaneous
    std::uint32_t ofsBoundingTriangles;
    std::uint32_t nBoundingVertices;     // Miscellaneous
    std::uint32_t ofsBoundingVertices;
    std::uint32_t nBoundingNormals;      // Miscellaneous
    std::uint32_t ofsBoundingNormals;

    std::uint32_t nAttachments;          // O, Miscellaneous
    std::uint32_t ofsAttachments;        // Attachments are for weapons etc.
    std::uint32_t nAttachLookup;         // P, Miscellaneous
    std::uint32_t ofsAttachLookup;       // Of course with a lookup.
    std::uint32_t nEvents;               //
    std::uint32_t ofsEvents;             // Used for playing sounds when dying and a lot else.
    std::uint32_t nLights;               // R
    std::uint32_t ofsLights;             // Lights are mainly used in loginscreens but in wands and some doodads too.
    std::uint32_t CameraCount;           // S, Miscellaneous
    std::uint32_t CameraOffset;          // The cameras are present in most models for having a model in the Character-Tab.
    std::uint32_t nCameraLookup;         // Miscellaneous
    std::uint32_t ofsCameraLookup;       // And lookup-time again, unit16
    std::uint32_t nRibbonEmitters;       // U, Effects
    std::uint32_t ofsRibbonEmitters;     // Things swirling around. See the CoT-entrance for light-trails.
    std::uint32_t nParticleEmitters;     // V, Effects
    std::uint32_t ofsParticleEmitters;   // Spells and weapons, doodads and loginscreens use them. Blood dripping of a blade? Particles.
};

struct Pair
{
    std::uint32_t Number;
    std::uint32_t Offset;
};

struct AnimationBlock
{
    std::int16_t interpolation_type;
    std::int16_t global_sequence_id;
    std::uint32_t FramePairsCount;
    std::uint32_t FramePairsOffset;
    std::uint32_t TransPairsCount;
    std::uint32_t TransPairsOffset;
};

struct Camera
{
    std::int32_t Type;
    float   fov;
    float   far_clipping;
    float   near_clipping;
    struct  AnimationBlock SourceTranslations;      // Vector
    float   SourcePosition[3];
    struct  AnimationBlock target_translations;     // Vector
    float   target_position[3];
    struct  AnimationBlock rotations;               // Quaternion
};

struct CameraV10
{
    std::int32_t Type;
    float   far_clipping;
    float   near_clipping;
    struct  AnimationBlock SourceTranslations;      // Vector
    float   SourcePosition[3];
    struct  AnimationBlock target_translations;     // Vector
    float   target_position[3];
    struct  AnimationBlock rotations;               // Quaternion
    struct  AnimationBlock unknown_block_4;         // Float
};
#pragma pack(pop)

struct CameraModelView
{
    AnimationBlock const* Animation = nullptr;
    float SourcePosition[3] = {};
    std::size_t BlockSize = 0;
    std::uint32_t KeyFramesCount = 0;
};

// Subtraction and division avoid overflow in offset + count * stride.
inline bool Contains(std::size_t size, std::uint32_t offset, std::uint32_t count, std::size_t stride)
{
    return stride && offset <= size && count <= (size - offset) / stride;
}

inline bool ValidateCameraModel(char const* data, std::size_t size, CameraModelView& result)
{
    result = CameraModelView();
    if (!data || size < sizeof(ModelHeader) || std::memcmp(data, "MD20", 4))
        return false;

    ModelHeader const* header = reinterpret_cast<ModelHeader const*>(data);
    std::size_t cameraSize = header->Version[0] <= 9 ? sizeof(Camera) : sizeof(CameraV10);
    if (!header->CameraCount || !header->CameraOffset ||
        !Contains(size, header->CameraOffset, header->CameraCount, cameraSize))
        return false;

    CameraModelView view;
    for (std::uint32_t i = 0; i < header->CameraCount; ++i)
    {
        char const* cameraData = data + header->CameraOffset + std::size_t(i) * cameraSize;
        if (header->Version[0] <= 9)
        {
            Camera const* camera = reinterpret_cast<Camera const*>(cameraData);
            if (camera->Type != -1)
                continue;
            view.Animation = &camera->SourceTranslations;
            std::memcpy(view.SourcePosition, camera->SourcePosition, sizeof(view.SourcePosition));
        }
        else
        {
            CameraV10 const* camera = reinterpret_cast<CameraV10 const*>(cameraData);
            if (camera->Type != -1)
                continue;
            view.Animation = &camera->SourceTranslations;
            std::memcpy(view.SourcePosition, camera->SourcePosition, sizeof(view.SourcePosition));
        }
        break;
    }

    AnimationBlock const* animation = view.Animation;
    if (!animation || !animation->FramePairsCount ||
        animation->FramePairsCount != animation->TransPairsCount ||
        !animation->FramePairsOffset || !animation->TransPairsOffset ||
        animation->interpolation_type < INTERPOLATION_NONE || animation->interpolation_type > INTERPOLATION_BEZIER ||
        !Contains(size, animation->FramePairsOffset, animation->FramePairsCount, sizeof(Pair)) ||
        !Contains(size, animation->TransPairsOffset, animation->TransPairsCount, sizeof(Pair)))
        return false;

    view.BlockSize = 3 * sizeof(float);
    if (animation->interpolation_type == INTERPOLATION_HERMITE || animation->interpolation_type == INTERPOLATION_BEZIER)
        view.BlockSize *= 3;

    Pair const* frames = reinterpret_cast<Pair const*>(data + animation->FramePairsOffset);
    Pair const* translations = reinterpret_cast<Pair const*>(data + animation->TransPairsOffset);
    for (std::uint32_t i = 0; i < animation->FramePairsCount; ++i)
    {
        if (frames[i].Number != translations[i].Number ||
            (frames[i].Number && (!frames[i].Offset || !translations[i].Offset)) ||
            !Contains(size, frames[i].Offset, frames[i].Number, sizeof(std::uint32_t)) ||
            !Contains(size, translations[i].Offset, translations[i].Number, view.BlockSize))
            return false;
        view.KeyFramesCount = std::max(view.KeyFramesCount, frames[i].Number);
    }
    if (!view.KeyFramesCount)
        return false;
    result = view;
    return true;
}
}
}

#endif
