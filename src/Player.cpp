#include <sstream>
#include <algorithm>
#include "Player.hpp"

using namespace std;

Player::Player(const std::string &name, const std::string &pass, int XPUser, int RPUser)
    : User(name, pass) {
    XP = XPUser;
    RP = RPUser;
    wins = 0;
    losses = 0;
    inMatch = false;
    isReady = false;
    healthPenalty = {0, 0};
    bulletPenalty = {0, 0};
}

bool Player::isObserver() const {
    return false;
}

void  Player::setReady(bool status) {
    isReady = status;
}

void Player::setPlayerInMatch() {
    inMatch = true;
}

void Player::setPlayerFinishMatch() {
    inMatch = false;
}

void Player::playerWinner() {
    wins += 1;
}

void Player::playerLoser() {
    losses += 1;
}

void Player::updateXP(int delta, bool isWinner) {
    if (isWinner) XP += delta;
    else XP -= delta;
}

void Player::updateRP(int delta, int healthBonus, bool isWinner) {
    if (isWinner) RP += (delta + healthBonus);
    else RP -= delta;
}

Rank Player::rankOfPlayer() const {
    if (RP < 1400) return Rank::BRONZE;
    else if (RP < 1750) return Rank::SILVER;
    else if (RP < 2250) return Rank::GOLD;
    else return Rank::PLATINUM;
}

bool Player::isBlokedByPlayer(const string& blockerUsername) const {
    auto it = find(blockersUsername.begin(), blockersUsername.end(), blockerUsername);
    if (it != blockersUsername.end()) return true;
    return false;
}

void Player::blockByPlayer(const string& blockerUsername) {
    auto it = find(blockersUsername.begin(), blockersUsername.end(), blockerUsername);
    if (it == blockersUsername.end()) blockersUsername.push_back(blockerUsername);
}

void Player::unblockByPlayer(const string& blockerUsername) {
    auto it = find(blockersUsername.begin(), blockersUsername.end(), blockerUsername);
    if (it != blockersUsername.end()) blockersUsername.erase(it);
}

string Player::convertLevelToString() const {
    Rank level = rankOfPlayer();
    if (level == Rank::BRONZE) return "Bronze";
    else if (level == Rank::SILVER) return "Silver";
    else if (level == Rank::GOLD) return "Golden";
    else return "Platinum";
}

void Player::setHealthPenalty(int numberOfMatches, int numberOfPenalties) {
    healthPenalty = {numberOfMatches, numberOfPenalties};
}

void Player::setBulletPenalty(int numberOfMatches, int numberOfPenalties) {
    bulletPenalty = {numberOfMatches, numberOfPenalties};
}

bool Player::hasBulletPenalties() {
    if (bulletPenalty.first == 0) return false;
    bulletPenalty.first -= 1;
    return true;
}

bool Player::hasHealthPenalties() {
    if (healthPenalty.first == 0) return false;
    healthPenalty.first -= 1;
    return true;
}

string Player::casualOpponentDetail(int index) const {
    stringstream detail;
    detail << index << ". " << username << " with " << XP << " XP";
    return detail.str();
}

string Player::rankedOpponentDetail(int index) const {
    stringstream detail;
    detail << index << ". " << username << " with " << RP << " RP";
    return detail.str();
}

string Player::profile() const {
    stringstream profile;
    profile << "username: " << '"' << username << '"' << '\n';
    profile << "Level: " << convertLevelToString() << '\n';
    profile << "RP: " << RP << '\n';
    profile << "XP: " << XP << '\n';
    profile << "Total wins: " << wins << '\n';
    profile << "Total losses: " << losses;
    return profile.str();
}