/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <iostream>

#include "bat/ads/internal/purchase_intent/purchase_intent_classifier.h"
#include "bat/ads/internal/purchase_intent/funnel_sites.h"
#include "bat/ads/internal/purchase_intent/keywords.h"
#include "bat/ads/internal/time.h"

namespace ads {

PurchaseIntentClassifier::PurchaseIntentClassifier() {
}

PurchaseIntentClassifier::~PurchaseIntentClassifier() {
}

IntentSignalInfo PurchaseIntentClassifier::ExtractIntentSignal(
    const std::string& url) {
  auto search_query = SearchProviders::ExtractSearchQueryKeywords(url);
  // std::cout << "[DEBUG] 1: " << search_query << "\n";

  if(!search_query.empty()) {
    auto kw_segments = Keywords::MatchSegmentsList(search_query);
    // std::cout << "[DEBUG] 2: " << kw_segments.size() << "\n";

    if (kw_segments.empty()) {
      return IntentSignalInfo();
    }

    auto kw_weight = Keywords::MatchFunnelList(search_query);

    return IntentSignalInfo(Time::NowInSeconds(), kw_segments, kw_weight);
  } else {
    // std::cout << "[DEBUG] 3: no search\n";
    FunnelSiteInfo funnel_site = FunnelSites::MatchFunnelSite(url);
    return IntentSignalInfo(Time::NowInSeconds(), funnel_site.segments,
        funnel_site.weight);
  }

  return IntentSignalInfo();
}

}  // namespace ads
