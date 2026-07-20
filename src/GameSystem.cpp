#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "GameSystem.hpp"
#include "Exceptions.hpp"

using namespace std;

GameSystem::GameSystem(const std::string& playersFileName, const std::string& observersFileName) {
    getPlayers(playersFileName);
    getObservers(observersFileName);
    loggedInUser = nullptr;
    nextInvitationId = 1;
    nextReportId = 1;
}

void GameSystem::getPlayers(const std::string& playersFileName) {
    ifstream file(playersFileName);

    if (!file.is_open()) {
        cerr << "Unable to open file: " << playersFileName << endl;
        return;
    }

    string line;
    int lineNum = 0;

    while (getline(file, line)) {
        lineNum++;
        if (lineNum == 1) continue;
        stringstream lineStream(line);
        string playerName;
        string password;
        string XP;
        string RP;
        getline(lineStream, playerName, ',');
        getline(lineStream, password, ',');
        getline(lineStream, XP, ',');
        getline(lineStream, RP);
        players.push_back(make_shared<Player>(playerName, password, stoi(XP), stoi(RP)));
    }

    file.close();
}

void GameSystem::getObservers(const std::string& observersFileName) {
    ifstream file(observersFileName);

    if (!file.is_open()) {
        cerr << "Unable to open file: " << observersFileName << endl;
        return;
    }

    string line;
    int lineNum = 0;

    while (getline(file, line)) {
        lineNum++;
        if (lineNum == 1) continue;
        stringstream lineStream(line);
        string observerName;
        string password;
        getline(lineStream, observerName, ',');
        getline(lineStream, password);
        observers.push_back(make_shared<Observer>(observerName, password));
    }

    file.close();
}

shared_ptr<Player> GameSystem::findLoggedInPlayer() const {
    for (const shared_ptr<Player>& player : players) {
        if (player->getUserName() == loggedInUser->getUserName()) return player; 
    }
    return nullptr;
}

void GameSystem::registerPlayer(const string& username, const string& password) {
    if (loggedInUser != nullptr) throw PermissionDeniedException();

    for (const shared_ptr<Player>& player : players) {
        if (player->getUserName() == username) throw BadRequestException();
    }

    for (const shared_ptr<Observer>& observer : observers) {
        if (observer->getUserName() == username) throw BadRequestException();
    }

    players.push_back(make_shared<Player>(username, password, 500, 1200));
    loggedInUser = make_shared<Player>(username, password, 500, 1200);
}

void GameSystem::logInUser(const string& username, const string& password) {
    if (loggedInUser != nullptr) throw PermissionDeniedException();

    for (const shared_ptr<Player>& player : players) {
        if (player->getUserName() == username) {
            if (player->checkPassword(password)) {
                loggedInUser = player;
                return;
            }
            else throw PermissionDeniedException();
        }
    }

    for (const shared_ptr<Observer>& observer : observers) {
        if (observer->getUserName() == username) {
            if (observer->checkPassword(password)) {
                loggedInUser = observer;
                return;
            }
            else throw PermissionDeniedException();
        }
    }

    throw NotFoundException();
}

void GameSystem::logOutUser() {
    if (loggedInUser == nullptr) throw PermissionDeniedException();
    loggedInUser = nullptr;
}

void GameSystem::getStatus(const string& status) const {
    if (loggedInUser == nullptr || loggedInUser->isObserver()) throw PermissionDeniedException();

    if (!((status == "true") || (status == "false"))) throw BadRequestException();

    shared_ptr<Player> player = findLoggedInPlayer();
    if (player != nullptr) player->setReady((status == "true"));
}

string GameSystem::casualMatchOpponents(const string& sortType) const {
    if (loggedInUser == nullptr || loggedInUser->isObserver()) throw PermissionDeniedException();
    
    if (!(sortType == "desc" || sortType == "asc")) throw BadRequestException();

    vector<shared_ptr<Player>> readyPlayers;
    for (const shared_ptr<Player>& player : players) {
        if (player->isReadyForMatch()) readyPlayers.push_back(player);
    }

    erase_if(readyPlayers, [this](const shared_ptr<Player>& player)
        { return player->getUserName() == loggedInUser->getUserName(); });

    if (readyPlayers.empty()) throw EmptyException();

    sort(readyPlayers.begin(), readyPlayers.end(), [sortType](const shared_ptr<Player>& a, const shared_ptr<Player>& b) {
        if (a->getXPUser() != b->getXPUser()) return ((sortType == "asc") ? a->getXPUser() < b->getXPUser() : a->getXPUser() > b->getXPUser());

        int minLen = min(a->getUserName().length(), b->getUserName().length());

        for (int i=0; i<minLen; i++) {
            unsigned char charA = tolower(a->getUserName()[i]);
            unsigned char charB = tolower(b->getUserName()[i]);
            if (charA != charB) return charA < charB;
        }

        return a->getUserName().length() < b->getUserName().length();
    });

    stringstream details;
    for (size_t i = 0; i < readyPlayers.size(); i++) details << readyPlayers[i]->casualOpponentDetail(i + 1) << '\n';
    return details.str();
}

string GameSystem::rankedMatchOpponents(const string& sortType) const {
    if (loggedInUser == nullptr || loggedInUser->isObserver()) throw PermissionDeniedException();

    if (!(sortType == "desc" || sortType == "asc")) throw BadRequestException();

    shared_ptr<Player> loggedInPlayer = findLoggedInPlayer();
    if (loggedInPlayer == nullptr) return "Error";

    vector<shared_ptr<Player>> competitors;
    for (const shared_ptr<Player>& player : players) {
        if (player->rankOfPlayer() == loggedInPlayer->rankOfPlayer()) competitors.push_back(player);
    }

    erase_if(competitors, [this](const shared_ptr<Player>& player)
        { return player->getUserName() == loggedInUser->getUserName(); });

    if (competitors.empty()) throw EmptyException();

    sort(competitors.begin(), competitors.end(), [sortType](const shared_ptr<Player>& a, const shared_ptr<Player>& b) {
        if (a->getRPUser() != b->getRPUser()) return ((sortType == "asc") ? a->getRPUser() < b->getRPUser() : a->getRPUser() > b->getRPUser());

        int minLen = min(a->getUserName().length(), b->getUserName().length());
        for (int i=0; i<minLen; i++) {
            unsigned char charA = tolower(a->getUserName()[i]);
            unsigned char charB = tolower(b->getUserName()[i]);
            if (charA != charB) return charA < charB;
        }
        return a->getUserName().length() < b->getUserName().length();
    });

    stringstream details;
    for (size_t i = 0; i < competitors.size(); i++) details << competitors[i]->rankedOpponentDetail(i + 1) << '\n';
    return details.str();
}

void GameSystem::invitation(const string& receiverName, const string& matchType) {
    if (loggedInUser == nullptr || loggedInUser->isObserver()) throw PermissionDeniedException();

    if (!(matchType == "casual" || matchType == "ranked")) throw BadRequestException();

    auto receiver1 = find_if(observers.begin(), observers.end(), [receiverName]
        (const shared_ptr<Observer>& observer) { return observer->getUserName() == receiverName; });
    if (receiver1 != observers.end()) throw PermissionDeniedException();

    auto receiver2 = find_if(players.begin(), players.end(), [receiverName]
        (const shared_ptr<Player>& player) { return player->getUserName() == receiverName; });
    if (receiver2 == players.end()) throw NotFoundException();

    shared_ptr<Player> sender = findLoggedInPlayer();
    if (sender != nullptr) {
        if (sender->isBlokedByPlayer(receiverName)) throw NotFoundException();
        invitations.push_back(make_shared<Invitation>(sender, *receiver2, nextInvitationId, matchType));
        nextInvitationId++;
    }
}

void GameSystem::startMatch(int id) {
    if (loggedInUser == nullptr || loggedInUser->isObserver()) throw PermissionDeniedException();

    auto targetInvitation = find_if(invitations.begin(), invitations.end(),
        [id](const shared_ptr<Invitation>& a) { return a->getInvitationId() == id; });
    if (targetInvitation == invitations.end()) throw NotFoundException();

    shared_ptr<Player> starter = findLoggedInPlayer();
    if (starter != nullptr) {
        if ((*targetInvitation)->arePlayersInMatch()) throw PermissionDeniedException();

        if (!(*targetInvitation)->checkReceiverInvitation(starter)) throw PermissionDeniedException();
        
        if ((*targetInvitation)->getMatchType() == "ranked") {
            if ((*targetInvitation)->getSender()->rankOfPlayer() != starter->rankOfPlayer()) {
                invitations.erase(targetInvitation);
                throw LevelMismatchException();
            }
        }

        matches.push_back(make_shared<Match>((*targetInvitation)->getSender(), starter, (*targetInvitation)->getMatchType()));
        starter->setPlayerInMatch();
        (*targetInvitation)->getSender()->setPlayerInMatch();
        (*targetInvitation)->acceptInvitation();
        invitations.erase(targetInvitation);
    }
}

void GameSystem::rejectInvitation(int id) {
    if (loggedInUser == nullptr || loggedInUser->isObserver()) throw PermissionDeniedException();

    auto targetInvitation = find_if(invitations.begin(), invitations.end(),
        [id](const shared_ptr<Invitation>& a) { return a->getInvitationId() == id; });
    if (targetInvitation == invitations.end()) throw NotFoundException();

    shared_ptr<Player> starter = findLoggedInPlayer();
    if (starter != nullptr) {
        if (!(*targetInvitation)->checkReceiverInvitation(starter)) throw PermissionDeniedException();

        (*targetInvitation)->rejectInvitation();
        invitations.erase(targetInvitation);
    }
}

shared_ptr<Match> GameSystem::findMatchOfPlayer(const shared_ptr<Player>& player) const {
    for (const shared_ptr<Match>& m : matches) {
        if (m->isPlayerMatch(player)) return m;
    }
    return nullptr;
}

void GameSystem::action(const string& action) {
    if (loggedInUser == nullptr || loggedInUser->isObserver()) throw PermissionDeniedException();

    if (!((action == "shoot") || (action == "defend") || (action == "reload"))) throw BadRequestException();

    shared_ptr<Player> player = findLoggedInPlayer();
    if (player != nullptr) {
        if (!player->isInMatch()) throw NotFoundException();

        shared_ptr<Match> match = findMatchOfPlayer(player);
        if (match == nullptr) throw NotFoundException();

        if (!match->isPlayerTurn(player)) throw PermissionDeniedException();

        if (action == "shoot" && (!match->hasPlayerBullet(player))) throw BadRequestException();

        match->registerActions(player, action);
        if (match->isFinishedMatch()) erase(matches, match);
    }
}

string GameSystem::matchStatus() const {
    if (loggedInUser == nullptr || loggedInUser->isObserver()) throw PermissionDeniedException();

    shared_ptr<Player> player = findLoggedInPlayer();
    if (player != nullptr) {
        if (!player->isInMatch()) throw NotFoundException();

        shared_ptr<Match> match = findMatchOfPlayer(player);
        if (match == nullptr) throw NotFoundException();

        string status = match->matchStatus(player);
        return status;
    }
    return "Error";
}

void GameSystem::report(const string& username, const string& reason) {
    if (loggedInUser == nullptr) throw PermissionDeniedException();

    if (reason.empty()) throw BadRequestException();

    shared_ptr<Player> reportedPlayer = nullptr;
    for (const shared_ptr<Player>& player : players) {
        if (player->getUserName() == username) reportedPlayer = player;
    }

    if (reportedPlayer == nullptr) throw NotFoundException();

    shared_ptr<Player> sender = findLoggedInPlayer();
    if (sender != nullptr) {
        reports.push_back(make_shared<Report>(sender, reportedPlayer, reason, nextReportId));
        nextReportId++;
    }
}

string GameSystem::profile(const string& username) {
    if (loggedInUser == nullptr) throw PermissionDeniedException();

    if (username.empty() && loggedInUser->isObserver()) throw PermissionDeniedException();

    if (username.empty()) {
        shared_ptr<Player> player = findLoggedInPlayer();
        if (player != nullptr) return player->profile();
    }

    auto targetObserver = find_if(observers.begin(), observers.end(),
        [username](const shared_ptr<Observer>& a) { return a->getUserName() == username; });
    if (targetObserver != observers.end()) throw PermissionDeniedException();

    auto targetPlayer = find_if(players.begin(), players.end(),
        [username](const shared_ptr<Player>& a) { return a->getUserName() == username; });
    if (targetPlayer == players.end()) throw NotFoundException();

    return (*targetPlayer)->profile();
}

string GameSystem::receivedInvitation() const {
    if (loggedInUser == nullptr || loggedInUser->isObserver()) throw PermissionDeniedException();

    stringstream descriptions;
    shared_ptr<Player> player = findLoggedInPlayer();
    if (player != nullptr) {
        for (const shared_ptr<Invitation>& invitation : invitations) {
            if (invitation->checkReceiverInvitation(player))
                descriptions << invitation->invitationDetails() << '\n';
        }
    }

    if (descriptions.str().empty()) throw EmptyException();
    return descriptions.str();
}

string GameSystem::getReports() const {
    if (loggedInUser == nullptr || !loggedInUser->isObserver()) throw PermissionDeniedException();

    stringstream descriptions;
    for (const shared_ptr<Report>& reported : reports) {
        descriptions << reported->reportsDetails() << '\n';
    }

    if (descriptions.str().empty()) throw EmptyException();
    return descriptions.str();
}

void GameSystem::block(const string& username, const string& status) {
    if (loggedInUser == nullptr || loggedInUser->isObserver()) throw PermissionDeniedException();

    if (!(status == "blocked" || status == "unblocked")) throw BadRequestException();

    auto blocked1 = find_if(observers.begin(), observers.end(), [username]
        (const shared_ptr<Observer>& observer) { return observer->getUserName() == username; });
    if (blocked1 != observers.end()) throw BadRequestException();

    auto blocked2 = find_if(players.begin(), players.end(), [username]
        (const shared_ptr<Player>& player) { return player->getUserName() == username; });
    if (blocked2 == players.end()) throw NotFoundException();

    if (status == "blocked") (*blocked2)->blockByPlayer(loggedInUser->getUserName());
    else (*blocked2)->unblockByPlayer(loggedInUser->getUserName());
}

void GameSystem::penalty(int id, const string& type, int amount, int numberOfMatches) {
    if (loggedInUser == nullptr || !loggedInUser->isObserver()) throw PermissionDeniedException();

    if (!(type == "health_penalty" || type == "bullet_penalty")) throw BadRequestException();
    
    if ((type == "health_penalty" && (amount < 1 || amount > 2)) ||
        (type == "bullet_penalty" && (amount < 1 || amount > 3))) throw BadRequestException();

    if (numberOfMatches <= 0) throw BadRequestException();

    auto targetReport = find_if(reports.begin(), reports.end(),
        [id](const shared_ptr<Report>& a) { return a->getId() == id; });
    if (targetReport == reports.end()) throw NotFoundException();

    (*targetReport)->setPenalty(numberOfMatches, amount, type);
    reports.erase(targetReport);
}

void GameSystem::dismissReport(int id) {
    if (loggedInUser == nullptr || !loggedInUser->isObserver()) throw PermissionDeniedException();

    auto targetReport = find_if(reports.begin(), reports.end(),
        [id](const shared_ptr<Report>& a) { return a->getId() == id; });
    if (targetReport == reports.end()) throw NotFoundException();
    
    reports.erase(targetReport);
}