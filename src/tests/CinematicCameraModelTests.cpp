/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "CinematicCameraModel.h"
#include <iostream>
#include <limits>
#include <vector>

using namespace Skyfire::Cinematics;

namespace
{
    struct Fixture
    {
        std::vector<char> Data;
        std::uint32_t CameraOffset = sizeof(ModelHeader);
        std::uint32_t AnimationOffset;
        std::uint32_t FramesOffset;
        std::uint32_t TranslationsOffset;

        Fixture(bool modern, int interpolation = INTERPOLATION_LINEAR)
        {
            std::uint32_t cameraSize = modern ? sizeof(CameraV10) : sizeof(Camera);
            AnimationOffset = CameraOffset + (modern ? offsetof(CameraV10, SourceTranslations) : offsetof(Camera, SourceTranslations));
            FramesOffset = CameraOffset + cameraSize;
            TranslationsOffset = FramesOffset + 2 * sizeof(Pair);
            std::uint32_t timesOffset = TranslationsOffset + 2 * sizeof(Pair);
            std::uint32_t positionsOffset = timesOffset + 3 * sizeof(std::uint32_t);
            std::uint32_t stride = (interpolation >= INTERPOLATION_HERMITE ? 9 : 3) * sizeof(float);
            Data.resize(positionsOffset + 3 * stride);

            ModelHeader header = {};
            std::memcpy(header.id, "MD20", 4);
            header.Version[0] = modern ? 10 : 9;
            header.CameraCount = 1;
            header.CameraOffset = CameraOffset;
            Put(0, header);
            Put(CameraOffset, std::int32_t(-1));
            AnimationBlock animation = {};
            animation.interpolation_type = interpolation;
            animation.FramePairsCount = animation.TransPairsCount = 2;
            animation.FramePairsOffset = FramesOffset;
            animation.TransPairsOffset = TranslationsOffset;
            Put(AnimationOffset, animation);
            // The second track exceeds the first: allocation must cover both.
            Put(FramesOffset, Pair{1, timesOffset});
            Put(FramesOffset + sizeof(Pair), Pair{3, timesOffset});
            Put(TranslationsOffset, Pair{1, positionsOffset});
            Put(TranslationsOffset + sizeof(Pair), Pair{3, positionsOffset});
        }

        template<class T> void Put(std::size_t offset, T const& value)
        {
            std::memcpy(Data.data() + offset, &value, sizeof(T));
        }

        bool Valid() const
        {
            CameraModelView view;
            return ValidateCameraModel(Data.data(), Data.size(), view) && view.KeyFramesCount == 3;
        }
    };
}

int main()
{
    unsigned failures = 0;
    auto check = [&failures](bool passed, char const* description)
    {
        if (!passed)
        {
            std::cerr << description << '\n';
            ++failures;
        }
    };

    CameraModelView view;
    check(!ValidateCameraModel(nullptr, 0, view), "Empty input accepted");
    for (bool modern : {false, true})
    {
        for (int interpolation = INTERPOLATION_NONE; interpolation <= INTERPOLATION_BEZIER; ++interpolation)
        {
            Fixture valid(modern, interpolation);
            check(valid.Valid(), "Valid camera or larger subsequent track rejected");
            check(ValidateCameraModel(valid.Data.data(), valid.Data.size(), view) &&
                view.BlockSize == std::size_t(interpolation >= INTERPOLATION_HERMITE ? 9 : 3) * sizeof(float),
                "Incorrect translation stride");
        }

        Fixture valid(modern);
        for (std::size_t length = 0; length < valid.Data.size(); ++length)
            check(!ValidateCameraModel(valid.Data.data(), length, view), "Truncated model accepted");

        Fixture bad = valid;
        bad.Data[0] = 'X';
        check(!bad.Valid(), "Invalid magic accepted");
        bad = valid;
        bad.Put(bad.CameraOffset, std::int32_t(0));
        check(!bad.Valid(), "Model without cinematic camera accepted");
        bad = valid;
        bad.Put(offsetof(ModelHeader, CameraOffset), std::numeric_limits<std::uint32_t>::max());
        check(!bad.Valid(), "Out-of-range camera offset accepted");
        bad = valid;
        bad.Put(offsetof(ModelHeader, CameraCount), std::numeric_limits<std::uint32_t>::max());
        check(!bad.Valid(), "Overflowing camera count accepted");
        bad = valid;
        bad.Put(bad.AnimationOffset + offsetof(AnimationBlock, TransPairsCount), std::uint32_t(1));
        check(!bad.Valid(), "Mismatched track counts accepted");
        bad = valid;
        bad.Put(bad.AnimationOffset + offsetof(AnimationBlock, FramePairsOffset), std::numeric_limits<std::uint32_t>::max());
        check(!bad.Valid(), "Out-of-range track table accepted");
        bad = valid;
        bad.Put(bad.TranslationsOffset + sizeof(Pair), Pair{2, 0});
        check(!bad.Valid(), "Mismatched key counts accepted");
        bad = valid;
        bad.Put(bad.FramesOffset, Pair{1, std::numeric_limits<std::uint32_t>::max()});
        check(!bad.Valid(), "Out-of-range timestamps accepted");
        bad = valid;
        bad.Put(bad.TranslationsOffset, Pair{1, std::numeric_limits<std::uint32_t>::max()});
        check(!bad.Valid(), "Out-of-range translations accepted");
        bad = valid;
        bad.Put(bad.AnimationOffset + offsetof(AnimationBlock, interpolation_type), std::int16_t(4));
        check(!bad.Valid(), "Unknown interpolation accepted");
        bad = valid;
        bad.Put(bad.FramesOffset, Pair{std::numeric_limits<std::uint32_t>::max(), 1});
        bad.Put(bad.TranslationsOffset, Pair{std::numeric_limits<std::uint32_t>::max(), 1});
        check(!bad.Valid(), "Overflowing key count accepted");
        bad = valid;
        for (int i = 0; i < 2; ++i)
        {
            bad.Put(bad.FramesOffset + i * sizeof(Pair), Pair{0, 0});
            bad.Put(bad.TranslationsOffset + i * sizeof(Pair), Pair{0, 0});
        }
        check(!bad.Valid(), "Empty animation accepted");
    }
    return failures ? 1 : 0;
}
