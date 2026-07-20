#include <sstream>
#include "Invitation.hpp"

using namespace std;

Invitation::Invitation(const std::shared_ptr<Player>& senderPlayer, const std::shared_ptr<Player>& receiverPlayer,
    int id_, const string& matchType_) {
    sender = senderPlayer;
    receiver = receiverPlayer;
    id = id_;
    matchType = matchType_;
    status = InvitationStatus::PENDING;
}

void Invitation::acceptInvitation() {
    status = InvitationStatus::ACCEPTED;
}

void Invitation::rejectInvitation() {
    status = InvitationStatus::REJECTED;
}

bool Invitation::isPending() const {
    if (status == InvitationStatus::PENDING) return true;
    return false;
}

bool Invitation::arePlayersInMatch() const {
    return (sender->isInMatch() || receiver->isInMatch());
}

bool Invitation::checkReceiverInvitation(const std::shared_ptr<Player>& player) const {
    return (player->getUserName() == receiver->getUserName());
}

string Invitation::invitationDetails() const {
    stringstream description;
    description << id << ": Invitation from " << '"' << sender->getUserName() << '"' <<
        " for a " << '"' << matchType << '"' << " match";
    return description.str();
}