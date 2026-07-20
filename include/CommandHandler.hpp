#ifndef COMMANDHANDLER_HPP
#define COMMANDHANDLER_HPP
#include <string>
#include <vector>
#include <map>
#include "GameSystem.hpp"

class CommandHandler {
public:
    CommandHandler(GameSystem* system_);
    void handle(const std::string& command);
private:
    GameSystem* system;
    std::vector<std::string> tokenize(const std::string& command);
    bool isEnclosedInQuotes(const std::string& token);
    std::string stripQuotes(const std::string& token);
    std::map<std::string, std::string> makeArguments(const std::vector<std::string>& tokens);
    bool areAllValuesQuoted(const std::map<std::string, std::string>& args);
    bool isNumber(const std::string& token) const;
    std::string handleRegister(const std::vector<std::string>& tokens, const std::map<std::string, std::string>& args);
    std::string handleLogin(const std::vector<std::string>& tokens, const std::map<std::string, std::string>& args);
    std::string handleLogout(const std::vector<std::string>& tokens, const std::map<std::string, std::string>& args);
    std::string handleCasualReady(const std::vector<std::string>& tokens, const std::map<std::string, std::string>& args);
    std::string handleCasualOpponents(const std::vector<std::string>& tokens, const std::map<std::string, std::string>& args);
    std::string handleRankedOpponents(const std::vector<std::string>& tokens, const std::map<std::string, std::string>& args);
    std::string handleInvitation(const std::vector<std::string>& tokens, const std::map<std::string, std::string>& args);
    std::string handleStartMatch(const std::vector<std::string>& tokens, const std::map<std::string, std::string>& args);
    std::string handleRejectInvitation(const std::vector<std::string>& tokens, const std::map<std::string, std::string>& args);
    std::string handleAction(const std::vector<std::string>& tokens, const std::map<std::string, std::string>& args);
    std::string handleMatchStatus(const std::vector<std::string>& tokens, const std::map<std::string, std::string>& args);
    std::string handleReport(const std::vector<std::string>& tokens, const std::map<std::string, std::string>& args);
    std::string handleProfile(const std::vector<std::string>& tokens, const std::map<std::string, std::string>& args);
    std::string handleReceivedInvitations(const std::vector<std::string>& tokens, const std::map<std::string, std::string>& args);
    std::string handleReports(const std::vector<std::string>& tokens, const std::map<std::string, std::string>& args);
    std::string handleBlock(const std::vector<std::string>& tokens, const std::map<std::string, std::string>& args);
    std::string handlePenalty(const std::vector<std::string>& tokens, const std::map<std::string, std::string>& args);
    std::string handleDismissReport(const std::vector<std::string>& tokens, const std::map<std::string, std::string>& args);
    void resultOutput(const std::string& result);
};

#endif