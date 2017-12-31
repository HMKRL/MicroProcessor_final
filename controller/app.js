var os = require('os');
var fS = require('fs');
var i2c = require('i2c');
var path = require('path');
var express = require('express');
var request = require('request');

var address = 0x04;
var wire = new i2c(address, {device: '/dev/i2c-1'}); // point to your i2c address, debug provides REPL interface 

var express = require('express');
var app = express();

// Get IP Address
ip_wlan0 = os.networkInterfaces()['wlan0'][0].address

// Send IP address to telegram bot
request.get('https://api.telegram.org/bot516534439:AAHgrpViNj3JVeX2ur-tfU9kCchdiBbN-uU/sendMessage?chat_id=229942559&text=' + ip_wlan0);

app.set('port', (5000));

app.get('/', function(req, rsp) {
	rsp.writeHead(200, {'Content-Type':'text/html'});
	content = fS.readFileSync('./web/index.html');
	rsp.end(content);
});

app.get('/:file', function(req, rsp) {
	rsp.writeHead(200, {'Content-Type':'text/html'});
	filename = './web/' + req.params.file;
	if(fS.existsSync(filename)) {
		content = fS.readFileSync(filename);
		rsp.end(content);
	}
	else {
		rsp.end();
	}
});

app.get('/:player/:cmd', function(req, rsp) {
	i2c_msg = req.params.player + ' ' + req.params.cmd;
	wire.write(i2c_msg, function(err) {});
	rsp.end();
});

app.listen(app.get('port'), function() {
	console.log('Node app is running on ' + ip_wlan0 + ':' + app.get('port'));
});
