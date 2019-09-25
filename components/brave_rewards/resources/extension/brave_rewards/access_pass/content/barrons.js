import WallbreakerController from './controller/wallbreaker-controller'
import PerformBypassInBackgroundScript from './controller/perform-bypass-in-background'

function detectBypassablePaywall() {
  console.log('Wallbreaker [barrons]: detecting if page has a bypassable paywall')
  let where = document.location.href;
  // skips landing page
  if (where == "https://www.barrons.com/") {
    return Promise.resolve({bypass: false})
  }
  // detects that paywall is up based on the page's DOM
  let paywallDOM = document.getElementsByClassName("barrons-snippet-ad");
  if (paywallDOM.length > 0) {
	return Promise.resolve({bypass: true})
  }
  return Promise.resolve({bypass: false})
}

function bypassPaywall() {
  console.log('Wallbreaker [barrons]: bypassing paywall in background script...')
  return PerformBypassInBackgroundScript({
    cookiesToDelete: [ 'cX_P' ],
    cookiesToExtend: [],
    domain: ".barrons.com",
    url: "https://barrons.com",
  })
}

new WallbreakerController({
  fnDetectBypassablePaywall: detectBypassablePaywall,
  fnBypassPaywall: bypassPaywall
}).runForCurrentPage()