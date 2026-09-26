/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
#include "SocialSnapshot.h"
#include <boost/json/src.hpp>
#include <stdexcept>

int main()
{
    using namespace Skyfire::Chat;
    auto check = [](bool value) { if (!value) throw std::runtime_error("Social snapshot invariant failed"); };
    auto rejects = [&](auto action) { bool failed = false; try { action(); } catch (std::exception const&) { failed = true; } check(failed); };
    SocialSnapshot snapshot; snapshot.Cursor = 1;
    std::string after;
    // A later snapshot page may already include changes after the first head.
    snapshot.LoadPage(boost::json::parse(R"({"head":3,"records":[{"key":"a","revision":3,"document":{"announce":false}}]})"), after);
    snapshot.Events(boost::json::parse(R"({"head":3,"events":[{"key":"a","revision":2,"document":{"announce":true}},{"key":"a","revision":3,"document":{"announce":false}}]})"));
    check(snapshot.Cursor == 3 && !snapshot.Records.at("a").Document.as_object().at("announce").as_bool());
    snapshot.Events(boost::json::parse(R"({"head":4,"events":[{"key":"a","revision":4,"document":null}]})"));
    check(snapshot.Records.at("a").Revision == 4 && snapshot.Records.at("a").Document.is_null());
    rejects([&] { snapshot.Events(boost::json::parse(R"({"head":6,"events":[{"key":"a","revision":6,"document":null}]})")); });
    rejects([&] { snapshot.Events(boost::json::parse(R"({"head":5,"events":[]})")); });
    rejects([&] { snapshot.Events(boost::json::parse(R"({"head":3,"events":[]})")); });
    rejects([&] { snapshot.LoadPage(boost::json::parse(R"({"head":4,"records":[{"key":"a","revision":4,"document":null}]})"), after); });
    rejects([&] { SocialSnapshot::Number(boost::json::value(true)); });
    rejects([&] { SocialSnapshot::Number(boost::json::value(-1)); });
    rejects([&] { SocialSnapshot::Number(boost::json::value(1.5)); });
    SocialSnapshot full;
    std::string content(256 * 1024, 'x');
    rejects([&] { full.Apply({{"key", "large"}, {"revision", 1}, {"document", {{"text", content}}}}, 1); });
    for (std::uint64_t index = 1; index <= SocialSnapshot::MaxRecords; ++index)
        full.Apply({{"key", std::to_string(index)}, {"revision", index}, {"document", nullptr}}, index);
    rejects([&] { full.Apply({{"key", "overflow"}, {"revision", 9000}, {"document", nullptr}}, 9000); });
    return 0;
}
