/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <memory>
#include <tuple>

#include "bat/ads/internal/ads_client_mock.h"
#include "bat/ads/internal/ads_impl.h"

#include "testing/gtest/include/gtest/gtest.h"

#include "bat/ads/internal/purchase_intent/funnel_sites.h"

using ::testing::_;

// npm run test -- brave_unit_tests --filter=AdsPurchaseIntent*

namespace {

const std::vector<std::tuple<std::string, bool>> kTestUrls = {
  std::tuple<std::string, bool>("http://www.carmax.com", true),
  std::tuple<std::string, bool>("http://www.carmax.com/foobar", true),
  std::tuple<std::string, bool>("http://carmax.com", true),
  std::tuple<std::string, bool>("https://brave.com", false),
  std::tuple<std::string, bool>("https://paragonmotorclub.com/roadside/roadside_fleet.html", true),
  std::tuple<std::string, bool>("http://jagxk.com/news.html", true),
  std::tuple<std::string, bool>("https://www.autotrader.com/research/?rdpage=SUBNAV", true),
  std::tuple<std::string, bool>("https://www.kbb.com/audi/a6/2019/styles/?intent=buy-new", true),
  std::tuple<std::string, bool>("https://www.cargurus.com/Cars/inventorylisting/viewDetailsFilterViewInventoryListing.action?sourceContext=carGurusHomePageModel&entitySelectingHelper.selectedEntity=m3&zip=12345", true),
  std::tuple<std::string, bool>("carmax.com", false)
};

}  // namespace

namespace ads {

class AdsPurchaseIntentFunnelSitesTest : public ::testing::Test {
 protected:
  std::unique_ptr<MockAdsClient> mock_ads_client_;
  std::unique_ptr<AdsImpl> ads_;

  AdsPurchaseIntentFunnelSitesTest() :
      mock_ads_client_(std::make_unique<MockAdsClient>()),
      ads_(std::make_unique<AdsImpl>(mock_ads_client_.get())) {
    // You can do set-up work for each test here
  }

  ~AdsPurchaseIntentFunnelSitesTest() override {
    // You can do clean-up work that doesn't throw exceptions here
  }

  // If the constructor and destructor are not enough for setting up and
  // cleaning up each test, you can use the following methods

  void SetUp() override {
    // Code here will be called immediately after the constructor (right before
    // each test)
    funnel_sites_ = std::make_unique<FunnelSites>();
  }

  void TearDown() override {
    // Code here will be called immediately after each test (right before the
    // destructor)
  }

  // Objects declared here can be used by all tests in the test case
  std::unique_ptr<FunnelSites> funnel_sites_;
};

TEST_F(AdsPurchaseIntentFunnelSitesTest, MatchesFunnelSites) {  
  for (const auto& test_url: kTestUrls) {
    // Arrange
    auto url = std::get<0>(test_url);
    auto is_funnel_site = std::get<1>(test_url);

    // Act
    const bool did_match = funnel_sites_->IsFunnelSite(url);

    // TODO(MH): Delete
    std::cout << "DEBUG: " << url << " - " << is_funnel_site << " - "
        << did_match << "\n";

    // Assert
    EXPECT_TRUE(is_funnel_site == did_match);
  }
}

}  // namespace ads
