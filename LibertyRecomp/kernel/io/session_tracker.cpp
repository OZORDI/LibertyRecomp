#include "session_tracker.h"
#include <user/config.h>
#include <os/logger.h>

namespace Net {

// ============================================================================
// Utility Functions
// ============================================================================

const char* GameModeToString(GameMode mode) {
    switch (mode) {
        case GameMode::FreeMode:        return "Free Mode";
        case GameMode::Deathmatch:      return "Deathmatch";
        case GameMode::TeamDeathmatch:  return "Team Deathmatch";
        case GameMode::MafiyaWork:      return "Mafiya Work";
        case GameMode::TeamMafiyaWork:  return "Team Mafiya Work";
        case GameMode::CarJackCity:     return "Car Jack City";
        case GameMode::TeamCarJackCity: return "Team Car Jack City";
        case GameMode::Race:            return "Race";
        case GameMode::GTARace:         return "GTA Race";
        case GameMode::CopsNCrooks:     return "Cops 'n' Crooks";
        case GameMode::TurfWar:         return "Turf War";
        case GameMode::DealBreaker:     return "Deal Breaker";
        case GameMode::HangmansNOOSE:   return "Hangman's NOOSE";
        case GameMode::BombDaBaseII:    return "Bomb Da Base II";
        case GameMode::PartyMode:       return "Party Mode";
        default:                        return "Unknown";
    }
}

const char* MapAreaToString(MapArea area) {
    switch (area) {
        case MapArea::AllOfLibertyCity: return "All of Liberty City";
        case MapArea::Broker:           return "Broker";
        case MapArea::Dukes:            return "Dukes";
        case MapArea::Bohan:            return "Bohan";
        case MapArea::Algonquin:        return "Algonquin";
        case MapArea::Alderney:         return "Alderney";
        default:                        return "Unknown";
    }
}

GameMode StringToGameMode(const std::string& str) {
    if (str == "freemode" || str == "free_mode" || str == "Free Mode")
        return GameMode::FreeMode;
    if (str == "deathmatch" || str == "Deathmatch")
        return GameMode::Deathmatch;
    if (str == "team_deathmatch" || str == "Team Deathmatch")
        return GameMode::TeamDeathmatch;
    if (str == "mafiya_work" || str == "Mafiya Work")
        return GameMode::MafiyaWork;
    if (str == "team_mafiya_work" || str == "Team Mafiya Work")
        return GameMode::TeamMafiyaWork;
    if (str == "car_jack_city" || str == "Car Jack City")
        return GameMode::CarJackCity;
    if (str == "team_car_jack_city" || str == "Team Car Jack City")
        return GameMode::TeamCarJackCity;
    if (str == "race" || str == "Race")
        return GameMode::Race;
    if (str == "gta_race" || str == "GTA Race")
        return GameMode::GTARace;
    if (str == "cops_n_crooks" || str == "Cops 'n' Crooks")
        return GameMode::CopsNCrooks;
    if (str == "turf_war" || str == "Turf War")
        return GameMode::TurfWar;
    if (str == "deal_breaker" || str == "Deal Breaker")
        return GameMode::DealBreaker;
    if (str == "hangmans_noose" || str == "Hangman's NOOSE")
        return GameMode::HangmansNOOSE;
    if (str == "bomb_da_base_ii" || str == "Bomb Da Base II")
        return GameMode::BombDaBaseII;
    if (str == "party_mode" || str == "Party Mode")
        return GameMode::PartyMode;
    return GameMode::FreeMode;
}

MapArea StringToMapArea(const std::string& str) {
    if (str == "all" || str == "All of Liberty City")
        return MapArea::AllOfLibertyCity;
    if (str == "broker" || str == "Broker")
        return MapArea::Broker;
    if (str == "dukes" || str == "Dukes")
        return MapArea::Dukes;
    if (str == "bohan" || str == "Bohan")
        return MapArea::Bohan;
    if (str == "algonquin" || str == "Algonquin")
        return MapArea::Algonquin;
    if (str == "alderney" || str == "Alderney")
        return MapArea::Alderney;
    return MapArea::AllOfLibertyCity;
}

// ============================================================================
// Forward declarations for backend implementations
// ============================================================================

// Defined in session_tracker_community.cpp
std::unique_ptr<ISessionTracker> CreateCommunitySessionTracker();

// Defined in session_tracker_firebase.cpp  
std::unique_ptr<ISessionTracker> CreateFirebaseSessionTracker();

// Defined in session_tracker_lan.cpp
std::unique_ptr<ISessionTracker> CreateLANSessionTracker();

// ============================================================================
// Factory Implementation
// ============================================================================

std::unique_ptr<ISessionTracker> CreateSessionTracker() {
    EMultiplayerBackend backend = Config::MultiplayerBackend;
    
    switch (backend) {
        case EMultiplayerBackend::Community:
            LOGF_INFO("[SessionTracker] Creating Community backend");
            return CreateCommunitySessionTracker();
            
        case EMultiplayerBackend::Firebase:
            LOGF_INFO("[SessionTracker] Creating Firebase backend");
            return CreateFirebaseSessionTracker();
            
        case EMultiplayerBackend::LAN:
            LOGF_INFO("[SessionTracker] Creating LAN backend");
            return CreateLANSessionTracker();
            
        default:
            LOGF_WARNING("[SessionTracker] Unknown backend, falling back to Community");
            return CreateCommunitySessionTracker();
    }
}

} // namespace Net
