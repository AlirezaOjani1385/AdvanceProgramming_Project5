#include <iomanip>
#include <sstream>
#include "Match.hpp"

using namespace std;

const int MIN_XP_REWARD = 5;
const int BASE_XP_REWARD = 50;
const double XP_DIFF_MULTIPLIER = 0.1;
const int NUMBER_OF_SPACES = 20;
const int DELTA_RP_BRONZE = 75;
const int DELTA_RP_SILVER = 100;
const int DELTA_RP_GOLD = 125;
const int DELTA_RP_PLATINUM = 150;
const int COEFFICIENT_HEALTH_BONUS = 25;

Match::Match(const shared_ptr<Player>& p1, const shared_ptr<Player>& p2, const string& matchType_) {
    player1 = p1;
    player2 = p2;
    hasPlayer1Played = false;
    hasPlayer2Played = false;
    turn = 1;
    action1 = Action::PENDING;
    action2 = Action::PENDING;
    isFinished = false;
    matchType = matchType_;
    if (matchType_ == "ranked") {
        bullets1 = 3;
        bullets2 = 3;
        health1 = 3;
        health2 = 3;
    }
    else {
        bullets1 = 1;
        bullets2 = 1;
    }
    updatePenalties();
}

void Match::updatePenalties() {
    if (matchType == "ranked") {
        if (player1->hasBulletPenalties()) bullets1 -= player1->getBulletPenalty();
        if (player1->hasHealthPenalties()) health1 -= player1->getHealthPenalty();
        if (player2->hasBulletPenalties()) bullets2 -= player2->getBulletPenalty();
        if (player2->hasHealthPenalties()) health2 -= player2->getHealthPenalty();
    }
}

string Match::convertActionToString(Action action) const {
    if (action == Action::RELOAD) return "reload";
    else if (action == Action::SHOOT) return "shoot";
    else if (action == Action::DEFEND) return "defend";
    return "pending";
}

Action Match::convertStringToAction(const string& action) const {
    if (action == "reload") return Action::RELOAD;
    if (action == "shoot")  return Action::SHOOT;
    if (action == "defend") return Action::DEFEND;
    return Action::PENDING;
}

bool Match::isPlayerMatch(const shared_ptr<Player>& player) const {
    if (player->getUserName() == player1->getUserName() || player->getUserName() == player2->getUserName()) return true;
    return false;
}

bool Match::isPlayerTurn(const shared_ptr<Player>& player) const {
    if ((player->getUserName() == player1->getUserName() && hasPlayer1Played) || 
    (player->getUserName() == player2->getUserName() && hasPlayer2Played)) return false;
    return true;
}

bool Match::hasPlayerBullet(const shared_ptr<Player> &player) const {
    if ((player->getUserName() == player1->getUserName() && bullets1 == 0) || 
    (player->getUserName() == player2->getUserName() && bullets2 == 0)) return false;
    return true;
}

void Match::registerActions(const shared_ptr<Player> &player, const string &action) {
    if (player->getUserName() == player1->getUserName()) {
        action1 = convertStringToAction(action);
        hasPlayer1Played = true;
    }
    else {
        action2 = convertStringToAction(action);
        hasPlayer2Played = true;
    }

    if (hasPlayer1Played && hasPlayer2Played) {
        if (matchType == "casual") resolveCasualTurn();
        else resolveRankedTurn();
    }
}

void Match::resolveCasualTurn() {
    history.push_back({action1, action2});

    if (action1 == Action::SHOOT && action2 == Action::RELOAD) {
        finishCasualMatch(player1, player2);
        return;
    }
    else if (action2 == Action::SHOOT && action1 == Action::RELOAD) {
        finishCasualMatch(player2, player1);
        return;
    }

    if (action1 == Action::RELOAD) bullets1 += 1;
    else if (action1 == Action::SHOOT) bullets1 -= 1;

    if (action2 == Action::RELOAD) bullets2 += 1;
    else if (action2 == Action::SHOOT) bullets2 -= 1;

    hasPlayer1Played = false;
    hasPlayer2Played = false;

    action1 = Action::PENDING;
    action2 = Action::PENDING;
    turn++;
}

void Match::finishCasualMatch(shared_ptr<Player>& winner, shared_ptr<Player>& losser) {
    isFinished = true;

    winner->playerWinner();
    losser->playerLoser();

    int delta = max(MIN_XP_REWARD, static_cast<int>(BASE_XP_REWARD -
        (XP_DIFF_MULTIPLIER * (winner->getXPUser() - losser->getXPUser()))));

    winner->updateXP(delta, true);
    losser->updateXP(delta, false);

    winner->setPlayerFinishMatch();
    losser->setPlayerFinishMatch();
}

void Match::resolveRankedTurn() {
    history.push_back({action1, action2});
    
    if (action1 == Action::SHOOT && action2 == Action::RELOAD) {
        health2--;
        if (health2 == 0) {
            finishRankedMatch(player1, player2);
            return;
        }
    }

    else if (action2 == Action::SHOOT && action1 == Action::RELOAD) {
        health1--;
        if (health1 == 0) {
            finishRankedMatch(player2, player1);
            return;
        }
    }

    if (action1 == Action::RELOAD) bullets1 += 1;
    else if (action1 == Action::SHOOT) bullets1 -= 1;

    if (action2 == Action::RELOAD) bullets2 += 1;
    else if (action2 == Action::SHOOT) bullets2 -= 1;

    hasPlayer1Played = false;
    hasPlayer2Played = false;

    action1 = Action::PENDING;
    action2 = Action::PENDING;
    turn++;
}

void Match::finishRankedMatch(shared_ptr<Player>& winner, shared_ptr<Player>& losser) {
    isFinished = true;

    winner->playerWinner();
    losser->playerLoser();

    int delta;
    if (winner->rankOfPlayer() == Rank::BRONZE) delta = DELTA_RP_BRONZE;
    else if (winner->rankOfPlayer() == Rank::SILVER) delta = DELTA_RP_SILVER;
    else if (winner->rankOfPlayer() == Rank::GOLD) delta = DELTA_RP_GOLD;
    else delta = DELTA_RP_PLATINUM;

    int healthBonus = (health1 == 0) ? health2 * COEFFICIENT_HEALTH_BONUS : health1 * COEFFICIENT_HEALTH_BONUS;

    winner->updateRP(delta, healthBonus, true);
    losser->updateRP(delta, healthBonus, false);

    winner->setPlayerFinishMatch();
    losser->setPlayerFinishMatch();
}

string Match::matchStatus(const shared_ptr<Player> &player) const {
    stringstream status;

    bool isPlayer1 = (player == player1);
    Action myAction = isPlayer1 ? action1 : action2;
    Action opponentAction = isPlayer1 ? action2 : action1;
    int myBullets = isPlayer1 ? bullets1 : bullets2;
    int myHealth = isPlayer1 ? health1 : health2;

    status << "Turn " << turn << '\n';
    status << "You: " << convertActionToString(myAction) << '\n';
    status << "Your opponent: " << (opponentAction == Action::PENDING ? "pending" : "played") << '\n';
    status << "History:" << '\n';
    status << "Opponent's moves:   Your moves:" << '\n';
    for (const auto& move : history) {
        Action myMove = isPlayer1 ? move.player1Action : move.player2Action;
        Action opponentMove = isPlayer1 ? move.player2Action : move.player1Action;
        status << left << setw(NUMBER_OF_SPACES) << convertActionToString(opponentMove)
        << convertActionToString(myMove) << '\n';
    }
    status << "Your remaining bullets: " << myBullets;
    if (matchType == "ranked") status << "\nYour remaining health: " << myHealth;

    return status.str();
}