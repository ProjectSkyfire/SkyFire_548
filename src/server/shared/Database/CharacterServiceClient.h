/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
#ifndef SKYFIRE_CHARACTER_SERVICE_CLIENT_H
#define SKYFIRE_CHARACTER_SERVICE_CLIENT_H
#include <cstdint>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>
class PreparedStatement;
class Transaction;
struct CharacterServiceRows
{
    std::vector<std::uint32_t> Types;
    std::vector<std::vector<std::optional<std::string>>> Rows;
};
class CharacterServiceClient
{
public:
    CharacterServiceClient();
    ~CharacterServiceClient();
    bool Open(std::map<std::uint32_t, std::string> const& catalog);
    void Execute(char const* sql);
    void Execute(PreparedStatement const* statement);
    void Execute(Transaction const& transaction);
    CharacterServiceRows Query(char const* sql);
    CharacterServiceRows Query(PreparedStatement const* statement);
    static bool Enabled();
    static unsigned long Escape(char* to, char const* from, unsigned long length);
private:
    struct State;
    std::unique_ptr<State> _state;
    static std::vector<std::uint8_t> Encode(PreparedStatement const* statement);
    static std::vector<std::uint8_t> Encode(char const* sql);
    std::vector<std::uint8_t> Request(std::uint8_t operation, std::vector<std::uint8_t> body);
    CharacterServiceRows Rows(std::vector<std::uint8_t> const& bytes);
};
#endif
