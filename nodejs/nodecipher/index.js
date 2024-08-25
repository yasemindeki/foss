const config = require('config');
var crypto = require('crypto');

class nodeCipher {
	constructor(nodejsUserPwd) {
		const ivBase64 = config.get('security.iv');
		this.iv = Buffer.from(ivBase64, 'base64');
		this.key = nodejsUserPwd;
		
		console.log("ivBase64 is " + ivBase64);
	}
	
	encrypt(dataToEncrypt) {
		const cipher = crypto.createCipheriv('aes-256-cbc', this.key, this.iv);
		let encrypted = cipher.update(dataToEncrypt, 'utf8', 'hex');
		encrypted += cipher.final('hex');

		return Buffer.from(encrypted).toString('base64');		
	}
	
	decrypt(dataToDecrypt) {
		const encrypted = Buffer.from(dataToDecrypt, 'base64');
		const decipher = crypto.createDecipheriv('aes-256-cbc', this.key, this.iv);
		let decrypted = decipher.update(encrypted.toString('utf8'), 'hex', 'utf8');
		decrypted += decipher.final('utf8');
		
		return decrypted;		
	}
}

module.exports = nodeCipher;