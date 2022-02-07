<template>
  <ion-content color="success" class="ion-padding" v-if="connected === true">
    <ion-text color="dark"> You are connected to the database </ion-text>
  </ion-content>
  <ion-content color="danger" class="ion-padding" v-else>
    <ion-text color="dark"> Disconnected from the Database </ion-text>
  </ion-content>
</template>
<script>
import { IonContent, IonText } from "@ionic/vue";
//import { getIpAdress } from "../../services/dataFetch.js";
import { defineComponent } from "vue";
import { ref, onValue } from "firebase/database";
import { db } from "../../services/firebase-realtimedb";

export default defineComponent({
  name: "Popover",
  components: { IonContent, IonText },
  data() {
    return {
      connected: true,
    };
  },
  async mounted() {
    const connectedRef = ref(db, ".info/connected");
    onValue(connectedRef, (snap) => {
      if (snap.val() === true) {
        this.connected = true;
      } else {
        this.connected = false;
      }
    });
  },
});
</script>
