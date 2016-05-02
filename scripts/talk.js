var async = require("async");
var dgram = require("dgram");
var msg = process.argv.pop();

var pos = 0;

var socket = dgram.createSocket("udp4");

function pushMessage(e) {
  var b = new Buffer(9);
  b.write(msg.slice(pos, pos + 8));
  b.write("\0", 8);

  console.log(b);
  socket.send(b, 0, b.length, 1234, "100.64.64.206", function(e) {
    pos++;

    if(msg.length - pos >= 8)
      setTimeout(pushMessage, 1000);
    else
      socket.close();
  });
}

pushMessage();
