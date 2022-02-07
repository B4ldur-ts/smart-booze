
import { initializeApp } from 'firebase/app';
import {
    getDatabase,
    ref,
    child,
    get,
    update,
} from "firebase/database";

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

const app = initializeApp(firebaseConfig);

// Get a reference to the database service
const db = getDatabase(app);

function updateDatabase(userID, updateData) {
    const updates = {};
    updates[`/${userID}`] = updateData;
    update(ref(db), updates);
}

module.exports.updateDatabase = updateDatabase;