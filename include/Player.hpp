#ifndef PLAYER_HPP
#define PLAYER_HPP
#include <string>
#include <vector>
#include "User.hpp"
#include "Types.hpp"

class Player : public User {
public:
    Player(const std::string & name, const std::string & pass, int XPUser, int RPUser);
    bool isObserver() const override;
    void setReady(bool status);
    bool isReadyForMatch() const { return isReady; }
    int getXPUser() const { return XP; }
    int getRPUser() const { return RP; }
    bool isInMatch() const { return inMatch; }
    void setPlayerInMatch();
    void setPlayerFinishMatch();
    void playerWinner();
    void playerLoser();
    void updateXP(int delta, bool isWinner);
    void updateRP(int delta, int healthBonus, bool isWinner);
    Rank rankOfPlayer() const;
    bool isBlokedByPlayer(const std::string& blockerUsername) const;
    void blockByPlayer(const std::string& blockerUsername);
    void unblockByPlayer(const std::string& blockerUsername);
    std::string convertLevelToString() const;
    void setHealthPenalty(int numberOfMatches, int numberOfPenalties);
    void setBulletPenalty(int numberOfMatches, int numberOfPenalties);
    bool hasBulletPenalties();
    bool hasHealthPenalties();
    int getBulletPenalty() const { return bulletPenalty.second; }
    int getHealthPenalty() const { return healthPenalty.second; }
    std::string casualOpponentDetail(int index) const;
    std::string rankedOpponentDetail(int index) const;
    std::string profile() const;
private:
    int XP;
    int RP;
    int wins;
    int losses;
    bool inMatch;
    bool isReady;
    std::vector<std::string> blockersUsername;
    std::pair<int, int> healthPenalty;
    std::pair<int, int> bulletPenalty;
};

#endif