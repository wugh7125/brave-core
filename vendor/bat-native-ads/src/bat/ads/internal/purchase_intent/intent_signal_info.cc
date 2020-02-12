/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <string>
#include <vector>

#include "bat/ads/internal/purchase_intent/intent_signal_info.h"

namespace ads {

IntentSignalInfo::IntentSignalInfo() :
    timestamp_in_seconds(0),
    segments({}),
    strength(0) {}

IntentSignalInfo::IntentSignalInfo(
    const uint64_t timestamp_in_seconds,
    const std::vector<std::string> segments,
    const std::uint16_t& strength) :
    timestamp_in_seconds(timestamp_in_seconds),
    segments(segments),
    strength(strength) {}

IntentSignalInfo::IntentSignalInfo(const IntentSignalInfo& info) :
    timestamp_in_seconds(info.timestamp_in_seconds),
    segments(info.segments),
    strength(info.strength) {}

IntentSignalInfo::~IntentSignalInfo() {}

}  // namespace ads
