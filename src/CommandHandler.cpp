#include <algorithm>
#include <iostream>
#include <map>
#include "CommandHandler.hpp"
#include "Exceptions.hpp"

using namespace std;

const int QUESTION_MARK_INDEX = 2;
const int MIN_SIZE = 3;

CommandHandler::CommandHandler(GameSystem *system_) {
    system = system_;
}

bool CommandHandler::isEnclosedInQuotes(const string& token) {
    if (token.length() < MIN_SIZE - 1) return false;
    return (token.front() == '"' && token.back() == '"');
}

string CommandHandler::stripQuotes(const string& token) {
    return token.substr(1, token.length() - 2);
}

vector<string> CommandHandler::tokenize(const std::string& command) {
    vector<string> tokens;
    string currentToken = "";
    bool insideQuote = false;
    for (char c : command) {
        if (c == '"') {
            currentToken += c;
            insideQuote = !insideQuote;
            if (!insideQuote) {
                tokens.push_back(currentToken);
                currentToken = "";
            }
            continue;
        }
        if(c == ' ' && !insideQuote)
        {
            if(currentToken != "")
            {
                tokens.push_back(currentToken);
                currentToken = "";
            }
        }
        else currentToken += c;
    }
    if (currentToken != "") tokens.push_back(currentToken);
    return tokens;
}

map<string, string> CommandHandler::makeArguments(const vector<string>& tokens) {
    map<string, string> args;
    for (size_t i = MIN_SIZE; i < tokens.size(); i+=2) {
        if (i + 1 >= tokens.size()) continue;
        args[tokens[i]] = tokens[i + 1];
    }
    return args;
}

bool CommandHandler::areAllValuesQuoted(const map<string, string>& args) {
    for (const auto& arg : args) {
        if (!isEnclosedInQuotes(arg.second)) return false;
    }
    return true;
}

bool CommandHandler::isNumber(const string& token) const {
    if (token.empty()) return false;
    if (!(all_of(token.begin(), token.end(), ::isdigit))) return false;
    return true;
}

void CommandHandler::handle(const std::string& command) {
    vector<string> tokens = tokenize(command);
    map<string, string> args = makeArguments(tokens);
    string method = tokens[0];
    string action = tokens[1];
    string result;
    try {
        if (method != "GET" && method != "POST" && method != "PUT" && method != "DELETE") {
            throw BadRequestException();
        }
        if(method == "POST" && action == "register") {
            result = handleRegister(tokens, args);
        }
        else if(method == "POST" && action == "login") {
            result = handleLogin(tokens, args);
        }
        else if(method == "POST" && action == "logout") {
            result = handleLogout(tokens, args);
        }
        else if(method == "POST" && action == "casual_match_ready") {
            result = handleCasualReady(tokens, args);
        }
        else if(method == "GET" && action == "casual_match_opponents") {
            result = handleCasualOpponents(tokens, args);
        }
        else if(method == "GET" && action == "ranked_match_opponents") {
            result = handleRankedOpponents(tokens, args);
        }
        else if(method == "POST" && action == "invitation") {
            result = handleInvitation(tokens, args);
        }
        else if(method == "POST" && action == "start_match") {
            result = handleStartMatch(tokens, args);
        }
        else if(method == "POST" && action == "reject_invitation") {
            result = handleRejectInvitation(tokens, args);
        }
        else if(method == "POST" && action == "action") {
            result = handleAction(tokens, args);
        }
        else if(method == "GET" && action == "match_status") {
            result = handleMatchStatus(tokens, args);
        }
        else if(method == "POST" && action == "report") {
            result = handleReport(tokens, args);
        }
        else if(method == "GET" && action == "profile") {
            result = handleProfile(tokens, args);
        }
        else if(method == "GET" && action == "received_invitations") {
            result = handleReceivedInvitations(tokens, args);
        }
        else if(method == "GET" && action == "reports") {
            result = handleReports(tokens, args);
        }
        else if (method == "POST" && action == "block") {
            result = handleBlock(tokens, args);
        }
        else if (method == "POST" && action == "penalty") {
            result = handlePenalty(tokens, args);
        }
        else if (method == "POST" && action == "dismiss_report") {
            result = handleDismissReport(tokens, args);
        }
        else {
            throw NotFoundException();
        }
    }
    catch(BadRequestException& ex) {
        result = string(ex.what()) + '\n';
    }
    catch(PermissionDeniedException& ex) {
        result = string(ex.what()) + '\n';
    }
    catch(NotFoundException& ex) {
        result = string(ex.what()) + '\n';
    }
    catch(EmptyException& ex) {
        result = string(ex.what()) + '\n';
    }
    catch(LevelMismatchException& ex) {
        result = string(ex.what()) + '\n';
    }
    resultOutput(result);
}

string CommandHandler::handleRegister(const vector<string>& tokens, const map<string, string>& args) {
    if (tokens.size() < MIN_SIZE || tokens[QUESTION_MARK_INDEX] != "?") throw BadRequestException();
    if (!areAllValuesQuoted(args)) throw BadRequestException();
    if (args.find("username") == args.end() || args.find("password") == args.end()) throw BadRequestException();
    system->registerPlayer(stripQuotes(args.at("username")), stripQuotes(args.at("password")));
    return "OK\n";
}

string CommandHandler::handleLogin(const vector<string>& tokens, const map<string, string>& args) {
    if (tokens.size() < MIN_SIZE || tokens[QUESTION_MARK_INDEX] != "?") throw BadRequestException();
    if (!areAllValuesQuoted(args)) throw BadRequestException();
    if (args.find("username") == args.end() || args.find("password") == args.end()) throw BadRequestException();
    system->logInUser(stripQuotes(args.at("username")), stripQuotes(args.at("password")));
    return "OK\n";
}

string CommandHandler::handleLogout(const vector<string>& tokens, const map<string, string>& args) {
    if (tokens.size() < MIN_SIZE || tokens[QUESTION_MARK_INDEX] != "?") throw BadRequestException();
    if (!areAllValuesQuoted(args)) throw BadRequestException();
    system->logOutUser();
    return "OK\n";
}

string CommandHandler::handleCasualReady(const vector<string>& tokens, const map<string, string>& args) {
    if (tokens.size() < MIN_SIZE || tokens[QUESTION_MARK_INDEX] != "?") throw BadRequestException();
    if (!areAllValuesQuoted(args)) throw BadRequestException();
    if (args.find("status") == args.end()) throw BadRequestException();
    system->getStatus(stripQuotes(args.at("status")));
    return "OK\n";
}

string CommandHandler::handleCasualOpponents(const vector<string>& tokens, const map<string, string>& args) {
    if (tokens.size() < MIN_SIZE || tokens[QUESTION_MARK_INDEX] != "?") throw BadRequestException();
    if (!areAllValuesQuoted(args)) throw BadRequestException();
    string sortType;
    if (args.find("sort_order") == args.end())
        sortType = "desc";
    else {
        sortType = stripQuotes(args.at("sort_order"));
    }
    string details = system->casualMatchOpponents(sortType);
    return details;
}

string CommandHandler::handleRankedOpponents(const vector<string>& tokens, const map<string, string>& args) {
    if (tokens.size() < MIN_SIZE || tokens[QUESTION_MARK_INDEX] != "?") throw BadRequestException();
    if (!areAllValuesQuoted(args)) throw BadRequestException();
    string sortType;
    if (args.find("sort_order") == args.end())
        sortType = "desc";
    else {
        sortType = stripQuotes(args.at("sort_order"));
    }
    string details = system->rankedMatchOpponents(sortType);
    return details;
}

string CommandHandler::handleInvitation(const vector<string>& tokens, const map<string, string>& args) {
    if (tokens.size() < MIN_SIZE || tokens[QUESTION_MARK_INDEX] != "?") throw BadRequestException();
    if (!areAllValuesQuoted(args)) throw BadRequestException();
    if (args.find("username") == args.end() || args.find("match_type") == args.end()) throw BadRequestException();
    system->invitation(stripQuotes(args.at("username")), stripQuotes(args.at("match_type")));
    return "OK\n";
}

string CommandHandler::handleStartMatch(const vector<string>& tokens, const map<string, string>& args) {
    if (tokens.size() < MIN_SIZE || tokens[QUESTION_MARK_INDEX] != "?") throw BadRequestException();
    if (!areAllValuesQuoted(args)) throw BadRequestException();
    if (args.find("invitation_id") == args.end()) throw BadRequestException();
    if (!isNumber(stripQuotes(args.at("invitation_id")))) throw BadRequestException();
    system->startMatch(stoi(stripQuotes(args.at("invitation_id"))));
    return "OK\n";
}

string CommandHandler::handleRejectInvitation(const vector<string>& tokens, const map<string, string>& args) {
    if (tokens.size() < MIN_SIZE || tokens[QUESTION_MARK_INDEX] != "?") throw BadRequestException();
    if (!areAllValuesQuoted(args)) throw BadRequestException();
    if (args.find("invitation_id") == args.end()) throw BadRequestException();
    if (!isNumber(stripQuotes(args.at("invitation_id")))) throw BadRequestException();
    system->rejectInvitation(stoi(stripQuotes(args.at("invitation_id"))));
    return "OK\n";
}

string CommandHandler::handleAction(const vector<string>& tokens, const map<string, string>& args) {
    if (tokens.size() < MIN_SIZE || tokens[QUESTION_MARK_INDEX] != "?") throw BadRequestException();
    if (!areAllValuesQuoted(args)) throw BadRequestException();
    if (args.find("action") == args.end()) throw BadRequestException();
    system->action(stripQuotes(args.at("action")));
    return "OK\n";
}

string CommandHandler::handleMatchStatus(const vector<string>& tokens, const map<string, string>& args) {
    if (tokens.size() < MIN_SIZE || tokens[QUESTION_MARK_INDEX] != "?") throw BadRequestException();
    if (!areAllValuesQuoted(args)) throw BadRequestException();
    string status = system->matchStatus();
    return status + '\n';
}

string CommandHandler::handleReport(const vector<string>& tokens, const map<string, string>& args) {
    if (tokens.size() < MIN_SIZE || tokens[QUESTION_MARK_INDEX] != "?") throw BadRequestException();
    if (!areAllValuesQuoted(args)) throw BadRequestException();
    if (args.find("username") == args.end() || args.find("reason") == args.end()) throw BadRequestException();
    system->report(stripQuotes(args.at("username")), stripQuotes(args.at("reason")));
    return "OK\n";
}

string CommandHandler::handleProfile(const vector<string>& tokens, const map<string, string>& args) {
    if (tokens.size() < MIN_SIZE || tokens[QUESTION_MARK_INDEX] != "?") throw BadRequestException();
    if (!areAllValuesQuoted(args)) throw BadRequestException();
    string username;
    if (args.find("username") == args.end())
        username = "";
    else {
        username = stripQuotes(args.at("username"));
    }
    string profile = system->profile(username);
    return profile + '\n';
}

string CommandHandler::handleReceivedInvitations(const vector<string>& tokens, const map<string, string>& args) {
    if (tokens.size() < MIN_SIZE || tokens[QUESTION_MARK_INDEX] != "?") throw BadRequestException();
    if (!areAllValuesQuoted(args)) throw BadRequestException();
    string desctiptionInvitations = system->receivedInvitation();
    return desctiptionInvitations;
}

string CommandHandler::handleReports(const vector<string>& tokens, const map<string, string>& args) {
    if (tokens.size() < MIN_SIZE || tokens[QUESTION_MARK_INDEX] != "?") throw BadRequestException();
    if (!areAllValuesQuoted(args)) throw BadRequestException();
    string decriptionInvitations = system->getReports();
    return decriptionInvitations;
}

string CommandHandler::handleBlock(const vector<string>& tokens, const map<string, string>& args) {
    if (tokens.size() < MIN_SIZE || tokens[QUESTION_MARK_INDEX] != "?") throw BadRequestException();
    if (!areAllValuesQuoted(args)) throw BadRequestException();
    if (args.find("username") == args.end() || args.find("status") == args.end()) throw BadRequestException();
    system->block(stripQuotes(args.at("username")), stripQuotes((args.at("status"))));
    return "OK\n";
}

string CommandHandler::handlePenalty(const vector<string>& tokens, const map<string, string>& args) {
    if (tokens.size() < MIN_SIZE || tokens[QUESTION_MARK_INDEX] != "?") throw BadRequestException();
    if (!areAllValuesQuoted(args)) throw BadRequestException();
    if (args.find("report_id") == args.end() || args.find("type") == args.end() ||
        args.find("amount") == args.end() || args.find("number_of_matches") == args.end()) throw BadRequestException();
    if (!isNumber(stripQuotes(args.at("report_id"))) || !isNumber(stripQuotes(args.at("amount"))) ||
        !isNumber(stripQuotes(args.at("number_of_matches")))) throw BadRequestException();
    system->penalty(stoi(stripQuotes(args.at("report_id"))), stripQuotes(args.at("type")),
        stoi(stripQuotes(args.at("amount"))), stoi(stripQuotes(args.at("number_of_matches"))));
    return "OK\n";
}

string CommandHandler::handleDismissReport(const vector<string>& tokens, const map<string, string>& args) {
    if (tokens.size() < MIN_SIZE || tokens[QUESTION_MARK_INDEX] != "?") throw BadRequestException();
    if (!areAllValuesQuoted(args)) throw BadRequestException();
    if (args.find("report_id") == args.end()) throw BadRequestException();
    if (!isNumber(stripQuotes(args.at("report_id")))) throw BadRequestException();
    system->dismissReport(stoi(stripQuotes(args.at("report_id"))));
    return "OK\n";
}

void CommandHandler::resultOutput(const string& result) {
    cout << result;
}