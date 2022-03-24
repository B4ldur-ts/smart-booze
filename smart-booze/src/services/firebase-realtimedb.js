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
import secret from "./secret.json"

const firebaseConfig = JSON.parse(JSON.stringify(secret))[0];

firebase.initializeApp(firebaseConfig);
const db = getDatabase();
const auth = firebase.auth();
const deviceId = store.getters.deviceId;

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



