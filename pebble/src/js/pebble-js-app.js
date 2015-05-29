/*
 * pebble-js-app.js
 * Sends the sample message once it is initialized.
 */

var ws;

Pebble.addEventListener('ready', function(e) {
  console.log('PebbleKit JS Ready!');
  // to add to Settings
  ws = new WebSocket('ws://192.168.1.88:9000/ws');
});

Pebble.addEventListener("appmessage", function(e) {
  console.log("Compass: " + e.payload.compass);
  console.log("Accel: " + e.payload.accel_x + ", " + e.payload.accel_y + ", " + e.payload.accel_z);
  ws.send(e.payload.compass + ':' + e.payload.accel_x + ':' + e.payload.accel_y + ':' + e.payload.accel_z);
});
