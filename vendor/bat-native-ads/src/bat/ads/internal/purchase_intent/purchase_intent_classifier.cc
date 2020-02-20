/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <iostream>  // TODO(Moritz Haller): delete
#include <functional>
#include <utility>
#include <algorithm>

#include "bat/ads/internal/purchase_intent/purchase_intent_classifier.h"
#include "bat/ads/internal/purchase_intent/funnel_sites.h"
#include "bat/ads/internal/purchase_intent/keywords.h"
#include "bat/ads/internal/time.h"

namespace ads {

PurchaseIntentClassifier::PurchaseIntentClassifier() {
}

PurchaseIntentClassifier::~PurchaseIntentClassifier() {
}

PurchaseIntentSignalInfo PurchaseIntentClassifier::ExtractIntentSignal(
    const std::string& url) {
  auto search_query = SearchProviders::ExtractSearchQueryKeywords(url);

  if (!search_query.empty()) {
    auto kw_segments = Keywords::MatchSegmentsList(search_query);

    if (kw_segments.empty()) {
      return PurchaseIntentSignalInfo();
    }

    auto kw_weight = Keywords::MatchFunnelList(search_query);

    return PurchaseIntentSignalInfo(Time::NowInSeconds(),
        kw_segments, kw_weight);
  } else {
    FunnelSiteInfo funnel_site = FunnelSites::MatchFunnelSite(url);
    return PurchaseIntentSignalInfo(Time::NowInSeconds(),
        funnel_site.segments, funnel_site.weight);
  }

  return PurchaseIntentSignalInfo();
}

std::vector<std::string> PurchaseIntentClassifier::GetWinningCategories(
    std::map<std::string, std::deque<PurchaseIntentSignalHistory>> history,
    uint8_t count) {  // TODO(Moritz Haller): Rename to max_count
  signal_threshold_ = 10;  // TODO(Moritz Haller): Move to initialiser list

  if (history.empty()) {
    return {};
  }

  // TODO(Moritz Haller): Verify that `winning_categories` won't containt
  // duplicate entries
  if (count < history.size()) {
    count = history.size();
  }

  std::vector<std::pair<std::string, u_int8_t>> scores;
  for (const auto& segment_history : history) {
    auto score = GetIntentScoreForSegment(segment_history.second);
    scores.push_back(std::make_pair(segment_history.first, score));
  }

  std::sort(scores.begin(), scores.end(), [](auto &lhs, auto &rhs) {
    return lhs.second > rhs.second;
  });

  std::vector<std::string> winning_categories;
  std::vector<std::pair<std::string, u_int8_t>> top_scores(
      scores.begin(), scores.begin() + scores.size());
  for (const auto& scores_el : top_scores) {
    if (scores_el.second > signal_threshold_) {
      winning_categories.push_back(scores_el.first);
    }
  }
  return winning_categories;
}

// TODO(Moritz Haller): Make protected
u_int8_t PurchaseIntentClassifier::GetIntentScoreForSegment(
    const std::deque<PurchaseIntentSignalHistory> segment_history) {
  // TODO(Moritz Haller): Add parameters to initializer list and
  // pass during instantiation in `AdsImpl`
  signal_level_ = 1;
  // TODO(Moritz Haller): Use time constants
  signal_time_window_in_seconds_ = 60 * 60 * 24 * 7;

  u_int8_t intent_score = 0;

  for (const auto& signal : segment_history) {
    // Did signal decay already?
    const auto signal_decayed_at =
        Time::FromDoubleT(signal.timestamp_in_seconds) +
        base::TimeDelta::FromSeconds(signal_time_window_in_seconds_);
    const auto now = base::Time::Now();

    if (now > signal_decayed_at) {
      // Signal did decay, continue
      continue;
    }

    // If still present, add to total
    intent_score += signal_level_ * signal.weight;
  }

  return intent_score;
}

}  // namespace ads
