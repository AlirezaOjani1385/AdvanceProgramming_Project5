#ifndef MATCH_HPP
#define MATCH_HPP
#include <vector>
#include <memory>
#include "Player.hpp"
#include "Types.hpp"

class Match {
public:
    Match(const std::shared_ptr<Player>& p1, const std::shared_ptr<Player>& p2, const std::string& matchType_);
    bool isPlayerMatch(const std::shared_ptr<Player>& player) const;
    bool isPlayerTurn(const std::shared_ptr<Player>& player) const;
    bool hasPlayerBullet(const std::shared_ptr<Player>& player) const;
    void registerActions(const std::shared_ptr<Player>& player, const std::string& action);
    bool isFinishedMatch() const { return isFinished; };
    std::string matchStatus(const std::shared_ptr<Player> &player) const;
private:
    std::shared_ptr<Player> player1;
    std::shared_ptr<Player> player2;
    bool hasPlayer1Played;
    bool hasPlayer2Played;
    int turn;
    int bullets1;
    int bullets2;
    Action action1;
    Action action2;
    int health1;
    int health2;
    bool isFinished;
    std::string matchType;
    std::vector<TurnRecord> history;
    void updatePenalties();
    std::string convertActionToString(Action action) const;
    Action convertStringToAction(const std::string& action) const;
    void resolveCasualTurn();
    void finishCasualMatch(std::shared_ptr<Player>& winner, std::shared_ptr<Player>& losser);
    void resolveRankedTurn();
    void finishRankedMatch(std::shared_ptr<Player>& winner, std::shared_ptr<Player>& losser);
};

#endif