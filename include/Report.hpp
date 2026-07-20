#ifndef REPORT_HPP
#define REPORT_HPP
#include <memory>
#include "Player.hpp"

class Report {
public:
    Report(const std::shared_ptr<Player>& send, const std::shared_ptr<Player>& receive, const std::string& r, int id_);
    int getId() const { return id; }
    void setPenalty(int numberOfMatches, int numberOfPenalty, const std::string& type);
    std::string reportsDetails();
private:
    int id;
    std::shared_ptr<Player> sender;
    std::shared_ptr<Player> reportedPlayer;
    std::string reason;
};

#endif