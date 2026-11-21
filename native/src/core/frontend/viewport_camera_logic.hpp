#pragma once

#include "core/frontend/view_state.hpp"

namespace frontend::view {

constexpr bool is_reverse_pod(std::string_view definition)
{
    return definition == "pod_reverse" || definition == "podoff_reverse";
}

class ViewportCamera {
public:
    void update_phase(bool in_session, bool replay)
    {
        if (in_session_ != in_session) {
            before_replay_ = UNKNOWN;
            replay_exit_candidate_ = UNKNOWN;
            kind_ = UNKNOWN;
        } else if (replay_ != replay) {
            if (replay) {
                before_replay_ = kind_;
                replay_exit_candidate_ = UNKNOWN;
            } else {
                replay_exit_candidate_ = kind_ == before_replay_ ? kind_ : UNKNOWN;
                before_replay_ = UNKNOWN;
            }
            kind_ = UNKNOWN;
        }
        in_session_ = in_session;
        replay_ = replay;
    }

    Kind kind() const { return replay_ && kind_ == UNKNOWN ? OTHER : kind_; }

    bool needs_replay_recovery() const
    {
        return in_session_ && !replay_ && kind_ == UNKNOWN && replay_exit_candidate_ != UNKNOWN &&
               replay_exit_candidate_ != OTHER;
    }

    bool recover_live(uint8_t current_type)
    {
        if (!needs_replay_recovery() || classify(current_type) != replay_exit_candidate_)
            return false;
        kind_ = replay_exit_candidate_;
        replay_exit_candidate_ = UNKNOWN;
        return true;
    }

    bool observe(uint32_t viewport, uint8_t type, Kind fallback = OTHER)
    {
        if (!in_session_ || viewport != 0)
            return false;

        Kind next = type == 8 ? fallback : classify(type);
        if (kind_ == next)
            return false;
        kind_ = next;
        return true;
    }

private:
    static constexpr Kind classify(uint8_t type)
    {
        switch (type) {
        case 3:
            return COCKPIT;
        case 4:
            return TCAM;
        case 5:
            return TCAM_OFFSET;
        default:
            return OTHER;
        }
    }

    bool in_session_ = false;
    bool replay_ = false;
    Kind kind_ = UNKNOWN;
    Kind before_replay_ = UNKNOWN;
    Kind replay_exit_candidate_ = UNKNOWN;
};

}
