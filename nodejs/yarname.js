const iconv = require('iconv-lite');

const forge = require('node-forge');
const fs = require('fs');

const nodemailer = require('nodemailer');
const config = require('config');

const prompt = require('prompt-sync')();
const nodejsUserPwd = prompt.hide('Enter nodejs password: ');

const cipherClass = require('./nodecipher');
var nodecipher =  new cipherClass(nodejsUserPwd);

const poolClass = require('pg').Pool;
var pool = new poolClass({
	user: config.get('pg.user'),
	host: config.get('pg.host'),
	database: config.get('pg.database'),
	password: nodecipher.decrypt(config.get('pg.password')),
	port: config.get('pg.port')
});
	
//for (var key in myobject) {
//	console.log(key + ": " + typeof (myobject[key]));
//}

const tcpConnectorClass = require("./nodeconnector");
var ynes = new tcpConnectorClass();

testEncoding = function() {
	var passin;
	var tmp = '';
	//tmp = '';

	var isEncodingAvailable = iconv.encodingExists("ISO-8859-9");
	console.log("ISO-8859-9 is " + isEncodingAvailable);
	
	var buffer = Buffer.from(tmp, "utf-8");
	console.log("BUFFER:" + buffer.toString('hex'));
	
	var buffer2 = iconv.encode(buffer, "ISO-8859-9");
	console.log("BUFFE2:" + buffer2.toString('hex'));
	
	var buffer3 = iconv.encode(buffer, "UTF-16");
	console.log("BUFFE3:" + buffer3.toString('hex'));
	
	var buffer4 = iconv.encode(tmp, "ISO-8859-9");
	console.log("BUFFE4:" + buffer4.toString('hex'));

	
	var str1 = forge.util.encodeUtf8(tmp);
	console.log("str1:" + str1);
	
	var str2 = forge.util.encodeUtf8(buffer2);
	console.log("str2:" + str2);

	var str3 = forge.util.encodeUtf8(buffer);
	console.log("str3:" + str3);

	var str4 = forge.util.encodeUtf8(tmp);
	console.log("str4:" + str4);
	
	
	passin = buffer;
	return passin;
	
}

getCertificate = function () {
	try
	{
		var p12File = fs.readFileSync("yarname2.p12", 'binary');
		var p12Asn1 = forge.asn1.fromDer(p12File);
		
		//var p12 = forge.pkcs12.pkcs12FromAsn1(p12Asn1, '');
		var p12 = forge.pkcs12.pkcs12FromAsn1(p12Asn1, '');
		
		var bags = p12.getBags({
			bagType: forge.pki.oids.certBag
		});
		
		
		for (var key in bags) {
			console.log(key + ": " + typeof (bags[key]));
		}
		
		//console.log('[getPrivateKey][INF] ' + );
		var bag = bags[forge.pki.oids.certBag][0];
		
		console.log('CERTIFICATE');
		console.log(bag);
		/*
		var msg = {
			type: 'CERTIFICATE',
			body: forge.asn1.toDer(bag.asn1).getBytes()
		};
		var pem = forge.pem.encode(msg);
		
		console.log('PEM');
		console.log(pem);
		*/
	} catch (err) {
		console.log('[getPrivateKey][ERR] ' + err);
	}
}

getPrivateKey = function () {
	try
	{	
		var passin = testEncoding();

		var p12File = fs.readFileSync("yarname.p12", 'binary');
		var p12Asn1 = forge.asn1.fromDer(p12File);
		
		var p12 = forge.pkcs12.pkcs12FromAsn1(p12Asn1, passin);		
		var bags = p12.getBags({
			//friendlyName: '... Client Encryption Certificate',
			//bagType: forge.pki.oids.keyBag
			bagType: forge.pki.oids.pkcs8ShroudedKeyBag
		});
		
		console.log(bags);
		
		
		for (var key in bags) {
			console.log(key + ": " + typeof (bags[key]));
		}
		/*for (var key in bags.friendlyName) {
			console.log(key + ": " + typeof (bags.friendlyName[key]));
		}*/

		
		console.log('[getPrivateKey][INF] ' + bags[forge.pki.oids.pkcs8ShroudedKeyBag].length);
		var bag = bags[forge.pki.oids.pkcs8ShroudedKeyBag][0];
		var key = bag.key;
		
		/*
		var asn1 = forge.pki.privateKeyToAsn1(key);
		var der = forge.asn1.toDer(asn1);
		var b64key = forge.util.encode64(der.getBytes());
		*/
		
		/*
		var pem = forge.pki.privateKeyToPem(key);
		var pem64 = forge.util.decode64(pem);
		*/
		
		var asn1 = forge.pki.privateKeyToAsn1(key);
		var keyInfo = forge.pki.wrapRsaPrivateKey(asn1);
		var der = forge.asn1.toDer(keyInfo);
		var b64key = forge.util.encode64(der.getBytes());
		
		console.log('PRIVATE KEY');
		console.log(b64key);
	} catch (err) {
		console.log('[getPrivateKey][ERR] ' + err);
	}
}

/*
let transporter = nodemailer.createTransport({
	 service: "Gmail",
	 auth: {
		 user: "",
		 pass: ""
	 }
});
*/
let transporter = nodemailer.createTransport({
	 host: 'smtpout.secureserver.net',
	 port: 465,
	 secureConnection: true,
	 service: "Godaddy",
	 debug: true,
	 auth: {
		 user: "",
		 pass: ""					
		 
	 }
});
		 
var crypto = require('crypto');

const express = require("express");
const app = express();
const port = config.get('express.port');
var userMap = {};

var dataToEncrypt = "";
var ivEncrypted = nodecipher.encrypt(dataToEncrypt);
var ivDecrypted = nodecipher.decrypt(ivEncrypted);

console.log('ivBase64-encrypted: ' + ivEncrypted);
console.log('ivBase64-decrypted: ' + ivDecrypted);

ynes.connect();

/*sendOneTimePassword = function (id, otp) {		
	message = {
		from: "from-example@email.com",
		to: id,
		subject: "... Giriş Kodunuz",
		text: otp
	};

	transporter.sendMail(message, function(err, info) {
		 if (err) {
		   console.log(err)
		 } else {
		   console.log(info);
		 }
	});
}*/

sendOneTimePassword = async function (id, otp) {		
	var rc = false;
	try {
		var message = {
			 from: "",
			 to: id,
			 subject: "... hesabı güvenlik kodu",
			 text: otp
		};

		var nmInfoObject = await transporter.sendMail(message);		// [object Object] geridonuyor
		console.log("[nmInfoObject] info.messageId: " + nmInfoObject.messageId);	
		console.log("[nmInfoObject] info.accepted: " + nmInfoObject.accepted);	
		console.log("[nmInfoObject] info.rejected: " + nmInfoObject.rejected);	
		console.log("[nmInfoObject] info.pending: " + nmInfoObject.pending);	
		console.log("[nmInfoObject] info.response: " + nmInfoObject.response);	
		
		rc = true;
		
	} catch (error) {
		console.log("[sendOneTimePassword][ERR] " + error);
		rc = false;
	}

	return rc;
}

app.get("/api/status", (req, res) => {
	res.send("active");
});

app.get('/', (req, res) => {
  res.send('Hello World!');
});

/*app.get('/login/:id', (req, res) => {

	const buffer = crypto.randomBytes(3);
	const otp =	parseInt(buffer.toString("hex"), 16)
				.toString()
				.substr(0, 6);
				
	userMap[req.params.id] = otp;
	
	console.log("/login/id " + req.params.id);
	console.log("/login/id " + userMap[req.params.id]);
	
	sendOneTimePassword(req.params.id, otp);
	res.send(otp);	
});*/

getCustomer = async function (email) {
	var customerID = 0;
/*	pool.query("SELECT * FROM app.get_customer($1)", [email], (err, res) => {
		if (err) {
			console.log('query function failed');
		} else {
			console.log('query function has been executed.');
			if (res.rows.length) {
				console.log('query function id: ' + res.rows[0].id);
			}
		}		
	});	
*/

	try
	{
		var res = await pool.query("SELECT * FROM app.get_customer($1)", [email]);
		console.log("[getCustomer][INF] query has returned " + res.rows.length);
		
		if (res.rows.length)
			customerID = res.rows[0].id;
			
	} catch(err) {
		console.log('[getCustomer][ERR] ' + err);
	}
	
	return customerID;

}

acceptCustomerLogin = function (email, response) {
	/* GENERATE OTP */
	const buffer = crypto.randomBytes(3);
	const otp =	parseInt(buffer.toString("hex"), 16)
				.toString()
				.substr(0, 6);
				
	userMap[email] = otp;
	
	console.log("/login/id " + email);
	console.log("/login/id " + userMap[email]);
	
	/* TRANSMIT OTP */
	var promiseObject = sendOneTimePassword(email, otp);	// [object Promise] geridonuyor	
	promiseObject.then((rc) => {
		console.log("[sendOneTimePassword][INF] sent " + rc);
		if (rc) {
			response.sendStatus(200);
		} else {
			response.status(500).send("Mailer hatası: Lütfen destek birimimize haber veriniz");
		}
	}).catch((err) => {
		console.log("[/login/:id][sendOneTimePassword][PO][ERR] " + err);
	});	

}

app.get('/login/:id', (req, res) => {
	
	var po = getCustomer(req.params.id);
	po.then((customerID) => {		
		if (customerID > 0) {
			console.log('[/login/:id][INF] login request from customer ' + customerID);
			acceptCustomerLogin(req.params.id, res);
		} else {
			console.log('[/login/:id][WRN] login request from unregistered email ' + req.params.id);
			res.status(500).send("Kaydınız mevcut değil: ... hesabınızı oluşturunuz");
		}
	}).catch((err) => {
		console.log("[/login/:id][getCustomer][PO][ERR]" + err);
	});

});

app.get('/login/:id/:otp', (req, res) => {

	const otp =	req.params.otp;
	
	console.log("/login/id/otp " + otp);
	console.log("/login/id/otp " + userMap[req.params.id]);

	if (userMap[req.params.id] === otp) {
		res.send("1");
	} else {
		res.send("0");
	}
});

app.get('/kms/:id', (req, res) => {
	// Oturum acmis musteriler haritasinda - userMap icinde - var mi kontrolu
	// Gelen paketi private key ile cozme > onetimeaeskey
	// keystore tablosundan eDEK'i cekip sifresini cozup, onetimeaeskey ile sifreleme
	// Yeni eDEK'i cevap olarak gonderme
	// Bellekte sensitive data kalmasin
	
});


app.listen(port, () => {
  console.log(`Yarname.app listening on port ${port}`);
  
  //getCertificate();
  getPrivateKey();
});

// 80