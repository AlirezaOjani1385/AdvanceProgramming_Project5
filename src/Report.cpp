#include <sstream>
#include "Report.hpp"

using namespace std;

Report::Report(const shared_ptr<Player>& send, const shared_ptr<Player>& receive, const std::string &r, int id_) {
    sender = send;
    reportedPlayer = receive;
    id = id_;
    reason = r;
}

void Report::setPenalty(int numberOfMatches, int numberOfPenalty, const string& type) {
    if (type == "health_penalty") reportedPlayer->setHealthPenalty(numberOfMatches, numberOfPenalty);
    else reportedPlayer->setBulletPenalty(numberOfMatches, numberOfPenalty);
}

string Report::reportsDetails() {
    stringstream description;
    description << id << ": " << '"' << sender->getUserName() << '"' << " reported " << '"' <<
        reportedPlayer->getUserName() << '"' << " for: " << '"' << reason << '"';
    return description.str();
}