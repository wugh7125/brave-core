/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/purchase_intent/funnel_sites.h"

#include "url/gurl.h"

namespace ads {

FunnelSites::FunnelSites() = default;
FunnelSites::~FunnelSites() = default;

FunnelSiteInfo FunnelSites::MatchFunnelSite(const std::string& url) {
  auto visited_url = GURL(url);

  if (!visited_url.has_host()) {
    return FunnelSiteInfo();
  }

  for (const auto& funnel_site : _automotive_funnel_sites) {
    auto funnel_site_hostname = GURL(funnel_site.url_netloc);

    if (!funnel_site_hostname.is_valid()) {
      continue;
    }

    if (visited_url.DomainIs(funnel_site_hostname.host_piece())) {
      return funnel_site;
    }
  }

  return FunnelSiteInfo();
}

}  // namespace ads
