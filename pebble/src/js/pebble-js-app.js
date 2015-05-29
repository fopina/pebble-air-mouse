/*
 * pebble-js-app.js
 * Sends the sample message once it is initialized.
 */

Pebble.addEventListener('ready', function(e) {
  console.log('PebbleKit JS Ready!');
});

Pebble.addEventListener("appmessage", function(e) {
  console.log("Compass: " + e.payload.compass);
  console.log("Accel: " + e.payload.accel_x + ", " + e.payload.accel_y + ", " + e.payload.accel_z);
});
