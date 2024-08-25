const net = require('net');
const os = require('os');

class tcpConnector {
	constructor() {
		this.port = 4363;		
	}
	
	connect() {
		/* Method: net.connect
		 * A factory function, which returns a new 'net.Socket' and automatically connects with the supplied options.
		 */
		this.serverSocket = net.connect({
			port: this.port,
			allowHalfOpen: false
		}, this.listener.bind(this));
	}
	
	listener() {
		console.log('YARHELPER: CONNECTED(' + this.serverSocket.localAddress + ')');

		this.serverSocket.setKeepAlive(true);
		this.serverSocket.setNoDelay(true);
		
		// 1 byte header, 2 byte dataLength, 1 byte txcommand, 20 byte username, x byte pwdSHA256Encoded
		// Konsoldan pwd alma
		// Konsol girdisinin echo ozelligini kapama
		// SHA256 ile kodlama
		// Uzunlugunu elde etme appTokenLength
		// Buffer olusturma appTokenLength + 24 uzunlugunda
		// Buffer'i sokete yazma
		var userPassword = "";
		var userName = os.hostname(); // fixed 5
		var dataLength = userName.length + userPassword.length + 1;
		const txByteArray = Buffer.alloc(3 + dataLength);
		
		//const txByteArray = Buffer.concat([Buffer.from(userName), Buffer.from(userPassword)]);
		
		txByteArray.writeUInt8(0xAB, 0);
		txByteArray.writeUInt16LE(dataLength, 1);
		txByteArray.writeUInt8(0x01, 3);
		txByteArray.write(userName, 4);
		txByteArray.write(userPassword, 4 + 5);
		
		this.serverSocket.write(txByteArray);

		this.serverSocket.on('close', function (had_error) {
			console.log("YARHELPER: [close] had_error is " + had_error);
			serverSocket.destroy();
		});

		this.serverSocket.on('end', function () {
			console.log("YARHELPER: [end] ");
		});

		this.serverSocket.on('timeout', function () {
			console.log("YARHELPER: [timeout] ");
		});

		/* Event: net.Socket.error */
		this.serverSocket.on('error', function (data) {
			console.log('YARHELPER: error message is ' + data.message);
			serverSocket.destroy();
		});

		/* Event: net.Socket.data
		 * Emitted when data is received.
		 * 
		 * Callback Parameters:
		 * buffer - {Object} Buffer object
		 *
		 * RESOURCE: https://nodejs.org/api/net.html#net_event_data
		 */
		this.serverSocket.on('data', function (buffer) {
			try {
				rxState.readyRead(buffer, webSocket);
			}
			catch (err) {
				console.log("YARHELPER: [data] error is " + err);
			}

		});

	}
}

module.exports = tcpConnector;