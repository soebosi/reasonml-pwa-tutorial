importScripts('https://storage.googleapis.com/workbox-cdn/releases/3.2.0/workbox-sw.js');

workbox.core.setLogLevel(workbox.core.LOG_LEVELS.debug);

workbox.routing.registerRoute(
  new RegExp('.*'),
  workbox.strategies.networkFirst()
);
