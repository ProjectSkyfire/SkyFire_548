/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
#ifndef SKYFIRE_SOCIAL_SNAPSHOT_H
#define SKYFIRE_SOCIAL_SNAPSHOT_H
#include "SocialPersistence.h"
#include <boost/json.hpp>
#include <stdexcept>

namespace Skyfire::Chat
{
    struct SocialSnapshot
    {
        static constexpr std::size_t MaxBytes = 4 * 1024 * 1024, MaxRecords = 8192;
        std::map<std::string, SocialRecord> Records;
        std::uint64_t Cursor = 0;
        std::size_t Bytes = 0;
        static std::uint64_t Number(boost::json::value const& value)
        {
            std::uint64_t result;
            if (value.is_uint64()) result = value.as_uint64();
            else if (value.is_int64() && value.as_int64() >= 0) result = std::uint64_t(value.as_int64());
            else throw std::runtime_error("Invalid social revision");
            if (result > 0x7fffffffffffffffULL) throw std::runtime_error("Social revision overflow");
            return result;
        }
        void Apply(boost::json::object const& item, std::uint64_t head)
        {
            auto const& raw = item.at("key").as_string();
            std::string key(raw.data(), raw.size());
            auto revision = Number(item.at("revision"));
            auto const& doc = item.at("document");
            if (key.empty() || key.size() > 192 || !Cluster::ValidUtf8(key) || !revision || revision > head ||
                (!doc.is_null() && !doc.is_object())) throw std::runtime_error("Invalid social record");
            auto size = boost::json::serialize(doc).size();
            if (size > 256 * 1024) throw std::runtime_error("Oversized social record");
            auto old = Records.find(key);
            if (old != Records.end() && old->second.Revision >= revision) return;
            auto used = Bytes + key.size() + size;
            if (old != Records.end()) used -= key.size() + boost::json::serialize(old->second.Document).size();
            if (used > MaxBytes || (old == Records.end() && Records.size() >= MaxRecords))
                throw std::runtime_error("Social cache limit exceeded");
            Records[key] = {revision, doc}; Bytes = used;
        }
        std::size_t LoadPage(boost::json::value const& page, std::string& after)
        {
            auto const& object = page.as_object();
            auto head = Number(object.at("head"));
            if (head < Cursor) throw std::runtime_error("Social snapshot regressed");
            auto const& records = object.at("records").as_array();
            if (records.size() > 32) throw std::runtime_error("Oversized social snapshot page");
            for (auto const& item : records)
            {
                auto const& raw = item.as_object().at("key").as_string();
                std::string key(raw.data(), raw.size());
                if (key <= after) throw std::runtime_error("Unordered social snapshot page");
                Apply(item.as_object(), head); after = std::move(key);
            }
            return records.size();
        }
        std::size_t Events(boost::json::value const& page)
        {
            auto const& object = page.as_object();
            auto head = Number(object.at("head"));
            if (head < Cursor) throw std::runtime_error("Social event head regressed");
            auto const& events = object.at("events").as_array();
            if (events.size() > 32) throw std::runtime_error("Oversized social event page");
            for (auto const& item : events)
            {
                auto revision = Number(item.as_object().at("revision"));
                if (revision != Cursor + 1) throw std::runtime_error("Missing social events");
                Apply(item.as_object(), head); Cursor = revision;
            }
            if (events.size() < 32 && Cursor != head) throw std::runtime_error("Incomplete social event history");
            return events.size();
        }
    };
}
#endif
