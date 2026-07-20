#ifndef GAMESYSTEM_HPP
#define GAMESYSTEM_HPP
#include <memory>
#include <vector>
#include <string>
#include "Invitation.hpp"
#include "Match.hpp"
#include "Observer.hpp"
#include "Player.hpp"
#include "Report.hpp"

class GameSystem {
public:
    GameSystem(const std::string& playersFileName, const std::string& observersFileName);
    void registerPlayer(const std::string& username, const std::string& password);
    void logInUser(const std::string& username, const std::string& password);
    void logOutUser();
    void getStatus(const std::string& status) const;
    std::string casualMatchOpponents(const std::string& sortType) const;
    std::string rankedMatchOpponents(const std::string& sortType) const;
    void invitation(const std::string& receiverName, const std::string& matchType);
    void startMatch(int id);
    void rejectInvitation(int id);
    void action(const std::string& action);
    std::string matchStatus() const;
    void report(const std::string& username, const std::string& message);
    std::string profile(const std::string& username);
    std::string receivedInvitation() const;
    std::string getReports() const;
    void block(const std::string& username, const std::string& status);
    void penalty(int id, const std::string& type, int amount, int numberOfMatches);
    void dismissReport(int id);
private:
    std::vector<std::shared_ptr<Player>> players;
    std::vector<std::shared_ptr<Observer>> observers;
    std::shared_ptr<User> loggedInUser;
    std::vector<std::shared_ptr<Invitation>> invitations;
    int nextInvitationId;
    std::vector<std::shared_ptr<Match>> matches;
    std::vector<std::shared_ptr<Report>> reports;
    int nextReportId;
    void getPlayers(const std::string& playersFileName);
    void getObservers(const std::string& observersFileName);
    std::shared_ptr<Player> findLoggedInPlayer() const;
    std::shared_ptr<Match> findMatchOfPlayer(const std::shared_ptr<Player>& player) const;
};

#endif