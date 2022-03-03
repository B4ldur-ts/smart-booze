/* firebase integration */
import firebase from "firebase/compat/app";
import "firebase/compat/auth";
import "firebase/compat/firestore";
import "firebase/messaging";
import "firebase/compat/messaging";
import {
  getDatabase,
  ref,
  onValue,
  child,
  get,
  update,
} from "firebase/database";
import store from "../store/index";
// import { v4 as uuidv4 } from "uuid";
// import {
//   set as setStorage,
//   get as getStorage,
//   del as deleteStorage,
// } from "idb-keyval";

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

firebase.initializeApp(firebaseConfig);
const db = getDatabase();
const auth = firebase.auth();
// const deviceId = store.getters.deviceId;
const deviceId = "29bb3021-e9ce-44d1-9a38-5fe98e89ac83";

export { db, auth };

export async function addUserToDb(name) {
  const updates = {};
  const database = ref(getDatabase());
  get(child(database, "/users/"))
    .then((snapshot) => {
      if (snapshot.exists()) {
        let updateData = snapshot.val();
        updateData["allUsers"].push(name);
        updates["/users/"] = updateData;
        update(ref(db), updates);
      } else {
        console.log("No data available");
      }
    })
    .catch((error) => {
      console.error(error);
    });
}


export async function getMeasurements() {
  const reference = ref(db, `/devices/${deviceId}/`);
  onValue(reference, (snapshot) => {
    let object = snapshot.val();
    store.commit("setAllMeasurements", object);
  });
}



