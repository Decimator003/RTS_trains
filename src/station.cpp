#include "../include/station.h"
#include <iostream>

TrainStation::TrainStation() {
    // Initialize tracks
    for(int i = 0; i < 8; i++) {
        tracks[i] = {i, false, true}; // id, occupied, light_status
    }
   
    // Initialize switches
    for(int i = 0; i < 4; i++) {
        switches[i] = {i, false}; // id, position
    }
   
    // Initialize entry points
    left_entries = {0, 1, 2, 3};
    right_entries = {4, 5, 6, 7};
}
/*
bool TrainStation::allocateTrack(int train_id, const std::string& entry_point) {
    const std::vector<int>& available_tracks =
        (entry_point == "WEST") ? left_entries : right_entries;
   
    for(int track_id : available_tracks) {
        if(!tracks[track_id].occupied) {
            tracks[track_id].occupied = true;
            tracks[track_id].light_status = false;
            train_track_map[train_id] = track_id;  // Track which train is on which track
            return true;
        }
    }
    return false;
}
*/


void TrainStation::updateSwitches() {
    for(auto& sw : switches) {
        // Simple toggle for demonstration
        sw.position = !sw.position;
    }
}

int TrainStation::getTrainTrack(int train_id) const {
    auto it = train_track_map.find(train_id);
    if(it != train_track_map.end()) {
        return it->second;
    }
    return -1;
}

void TrainStation::printStatus() const {
    std::cout << "Track Status:\n";
    for(const auto& track : tracks) {
        std::cout << "Track " << track.id << ": "
                 << (track.occupied ? "Occupied" : "Free") << " - Light: "
                 << (track.light_status ? "Green" : "Red") << "\n";
    }
}

void TrainStation::releaseTrack(int track_number) {
    if(track_number >= 0 && track_number < 8) {
        tracks[track_number].occupied = false;
        tracks[track_number].light_status = true;
    }

	// Remove from tracking map
    for(auto it = train_track_map.begin(); it != train_track_map.end(); ++it) {
        if(it->second == track_number) {
            train_track_map.erase(it);
            break;
        }
    }
}

void TrainStation::reset() {
    // Reset tracks
    for(auto& track : tracks) {
        track.occupied = false;
        track.light_status = true;
    }
   
    // Reset switches
    for(auto& sw : switches) {
        sw.position = false;
    }
   
    // Clear any other state if necessary
    left_entries = {0, 1, 2, 3};
    right_entries = {4, 5, 6, 7};
}


bool TrainStation::allocateTrack(int train_id, const std::string& entry_point) {
    // Determine if train is entering from east or west
    const std::vector<int>& available_tracks =
        (entry_point == "WEST") ? left_entries : right_entries;

    // Find first available track in the appropriate direction
    for(int track_id : available_tracks) {
        if(!tracks[track_id].occupied) {
            tracks[track_id].occupied = true;
            tracks[track_id].light_status = false;
			train_track_map[train_id] = track_id;  // Store the train-track mapping
            return true;
        }
    }
    return false;
}
