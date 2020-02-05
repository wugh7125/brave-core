/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/purchase_intent/funnel_site_info.h"

namespace ads {

FunnelSiteInfo::FunnelSiteInfo() :
    url_netloc(""),
    weight(0) {}

FunnelSiteInfo::FunnelSiteInfo(
    const std::string& url_netloc,
    const std::uint8_t& weight) :
    url_netloc(url_netloc),
    weight(weight) {}

FunnelSiteInfo::FunnelSiteInfo(const FunnelSiteInfo& info) :
    url_netloc(info.url_netloc),
    weight(info.weight) {}

FunnelSiteInfo::~FunnelSiteInfo() {}

}  // namespace ads
