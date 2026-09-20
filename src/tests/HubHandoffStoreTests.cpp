/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifdef _WIN32
#include <winsock2.h>
#endif
#include <mysql.h>
#include "HubHandoffStore.h"
#include <chrono>
#include <cstdlib>
#include <future>
#include <iostream>
#include <stdexcept>
#include <thread>

using namespace Skyfire::Cluster::Handoff;
namespace { void Require(bool ok, char const* message) { if (!ok) throw std::runtime_error(message); } }

int main()
{
    auto configured = std::getenv("SKYFIRE_HANDOFF_TEST_DB");
    if (!configured) { std::cout << "Requires a disposable database integration fixture.\n"; return 77; }
    std::string connection = configured;
    auto split = connection.rfind(';');
    if (split == std::string::npos || connection.substr(split + 1).find("sf_handoff_test_") != 0) return 1;
    if (mysql_library_init(0, nullptr, nullptr)) return 1;
    int status = 0;
    try
    {
        HubHandoffStore first, second;
        Require(first.Open(connection) && second.Open(connection), "Cannot open integration stores");
        Binding binding; binding.Account = 123; binding.Address = "127.0.0.1";
        binding.Destination = "authnet"; binding.Evidence = "-";
        std::string token, found;
        Require(first.Issue(binding, 60, 999999999, token) == Result::Ok, "Issue failed");
        first.Close();
        Require(first.Open(connection) && first.Inspect(binding, 0, found) == Result::Ok && found == token,
            "Grant did not survive reconnect or used caller's monotonic clock");
        auto wrong = binding; wrong.Address = "127.0.0.2";
        Require(second.Consume(token, wrong, 0) == Result::Denied, "Wrong binding accepted");
        auto consume = [&](HubHandoffStore& store)
        {
            mysql_thread_init(); auto result = store.Consume(token, binding, 0); mysql_thread_end(); return result;
        };
        auto one = std::async(std::launch::async, [&] { return consume(first); });
        auto two = std::async(std::launch::async, [&] { return consume(second); });
        auto a = one.get(), b = two.get();
        Require((a == Result::Ok && b == Result::Replay) || (b == Result::Ok && a == Result::Replay),
            "Concurrent stores did not consume exactly once");
        second.Close();
        Require(second.Open(connection) && second.Consume(token, binding, 0) == Result::Replay,
            "Consumed grant revived after reconnect");
        std::string previous = token;
        Require(first.Issue(binding, 60, 0, token) == Result::Ok, "Replacement issue failed");
        Require(second.Consume(previous, binding, 0) == Result::Replay, "Old token was not retained as replay");
        Require(second.Revoke(token, binding, 0) == Result::Ok && first.Consume(token, binding, 0) == Result::Replay,
            "Revocation not shared");
        Require(first.Issue(binding, 1, 0, token) == Result::Ok, "Short grant failed");
        std::this_thread::sleep_for(std::chrono::milliseconds(1100));
        Require(second.Consume(token, binding, 0) == Result::Expired, "Database TTL was not enforced");
        first.Cleanup(0);
        Require(second.Consume(token, binding, 0) == Result::Missing, "Expired row was not removed");
        Require(first.Issue(binding, 901, 0, token) == Result::Invalid && token.empty(), "Invalid TTL accepted");
        std::cout << "PASS durable store reconnect, cross-store replay, concurrent consume, binding, revoke and TTL.\n";
    }
    catch (std::exception const& error) { std::cerr << error.what() << '\n'; status = 1; }
    mysql_library_end();
    return status;
}
