### Ads service public facing API changelog

---
### New API (4th November 2019)
##### Related PR

[https://github.com/brave/brave-core/pull/3805](https://github.com/brave/brave-core/pull/3805)

##### Description

Added a new function to the ads service to check if the users locale is newly
supported. Androd on-boarding should use this API call instead of the Android
implementation.

##### Added

bool AdsService::IsNewlySupportedLocale()
---

---
### New API (21st February 2020)
##### Related PR

[https://github.com/brave/brave-core/pull/4685](https://github.com/brave/brave-core/pull/4685)

##### Description

Refactored public facing API for ad notifications and added new public API calls
for Publisher Ads

##### Renamed

GetAdNotificationForId to GetAdNotification
GetAds to GetCreativeAdNotifications
ConfirmAd to ConfirmAdNotification
NotificationInfo to AdNotificationInfo
OnNotificationEvent to OnAdNotificationEvent
NotificationEventType to AdNotificationEventType

##### Added

bool ShouldShowPublisherAdsOnPariticipatingSites() const

void GetCreativePublisherAds(
    const std::string& url,
    const std::vector<std::string>& categories,
    const std::vector<std::string>& sizes,
    const OnGetCreativePublisherAdsCallback callback)

void GetPublisherAds(
    const std::string& url,
    const std::vector<std::string>& sizes,
    GetPublisherAdsCallback callback)

void OnPublisherAdEvent(
    const PublisherAdInfo& info,
    const PublisherAdEventType event_type)

void ConfirmPublisherAd(
    const PublisherAdInfo& info)

---
