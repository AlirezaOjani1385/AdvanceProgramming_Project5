#ifndef INVITATION_HPP
#define INVITATION_HPP
#include <string>
#include <memory>
#include "Player.hpp"
#include "Types.hpp"

class Invitation {
public:
    Invitation(const std::shared_ptr<Player>& senderPlayer, const std::shared_ptr<Player>& receiverPlayer,
        int id_, const std::string& matchType_);
    void acceptInvitation();
    void rejectInvitation();
    int getInvitationId() const { return id; };
    std::string getMatchType() const { return matchType; };
    bool isPending() const;
    bool arePlayersInMatch() const;
    bool checkReceiverInvitation(const std::shared_ptr<Player>& player) const;
    std::shared_ptr<Player> getSender() const { return sender; };
    std::string invitationDetails() const;
private:
    int id;
    std::shared_ptr<Player> sender;
    std::shared_ptr<Player> receiver;
    InvitationStatus status;
    std::string matchType;
};

#endif