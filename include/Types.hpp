#ifndef TYPES_HPP
#define TYPES_HPP

enum class InvitationStatus {
    PENDING,
    ACCEPTED,
    REJECTED
};

enum class Action {
    SHOOT,
    DEFEND,
    RELOAD,
    PENDING
};

enum  class Rank {
    BRONZE,
    SILVER,
    GOLD,
    PLATINUM
};

struct TurnRecord {
    Action player1Action;
    Action player2Action;
};

#endif