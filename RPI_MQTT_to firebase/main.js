/*
 * Copyright 2021 HiveMQ GmbH
 */

const mqtt = require('mqtt');
let firebase = require('firebase/app');
let firebasedb = require('firebase/database');
// import { initializeApp } from 'firebase/app';
// import {
//     getDatabase,
//     ref,
//     child,
//     get,
//     update,
// } from "firebase/database";


let received = 0;



const firebaseConfig = {
    apiKey: "AIzaSyCK7xzDTTzflkvvpLtaEuUSZdBR3Qq-2R4",
    authDomain: "smart-booze.firebaseapp.com",
    databaseURL: "https://smart-booze-default-rtdb.europe-west1.firebasedatabase.app",
    projectId: "smart-booze",
    storageBucket: "smart-booze.appspot.com",
    messagingSenderId: "778984110570",
    appId: "1:778984110570:web:17e6901ea09bc0ed6f1a93",
    measurementId: "G-F7YTZVB9RV"
};

const app = firebase.initializeApp(firebaseConfig);

// Get a reference to the database service
const db = firebasedb.getDatabase(app);

function updateDatabase(userID, updateData, number) {
    const updates = {};
    updates[`/devices/${userID}/Measurement${number}`] = updateData;
    console.log(updates[`/devices/${userID}/Measurement${number}`]);
    firebasedb.update(firebasedb.ref(db), updates);
}

// your credentials for HiveMQ
const options = {
    username: 'B4ldur',
    password: 'fg%2oR3!Sh6Ntu$Q#571HH$XBp',
};

let uploadObject = {

    value: "",
    time: "",
    date: "",
    //id: "",

}

let messageNumber;

// connect to your cluster, insert your host name and port
const client = mqtt.connect('tls://e6c0f2b4d98b45a58474f291fbfdcec4.s1.eu.hivemq.cloud', options);


client.on('message', function (topic, message) {
    let date_ob = new Date();

    let id = message.toString().slice(0, 36);
    uploadObject.value = parseInt(message.toString().slice(42, 47)) / 1000;
    messageNumber = message.toString().slice(36, 42);
    // current date
    // adjust 0 before single digit date
    let date = ("0" + date_ob.getDate()).slice(-2);
    // current month
    let month = ("0" + (date_ob.getMonth() + 1)).slice(-2);
    // current year
    let year = date_ob.getFullYear();
    // current hours
    let hours = ("0" + (date_ob.getHours() + 1)).slice(-2);
    // current minutes
    let minutes = ("0" + (date_ob.getMinutes() + 1)).slice(-2);
    // current seconds
    let seconds = ("0" + (date_ob.getSeconds() + 1)).slice(-2);

    // prints date in YYYYMMDD format
    uploadObject.date = (year + month + date);

    // prints time in HH:MM:SS format
    uploadObject.time = hours.toString() + minutes.toString() + seconds.toString();


    console.log(uploadObject.value);
    console.log(uploadObject.id);
    console.log(messageNumber);
    console.log(uploadObject.date);
    console.log(uploadObject.time);
    console.log("________________________________");


    updateDatabase(id, uploadObject, messageNumber);

});
// reassurance that the connection worked
client.on('connect', () => {
    console.log('Connected!');
});

// prints an error message
client.on('error', (error) => {
    console.log('Error:', error);
});



// subscribe and publish to the same topic
client.subscribe('devices/#');
//client.publish('messages', 'Hello, this message was received!');



