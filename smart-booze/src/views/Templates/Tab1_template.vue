<template>
  <base-layout tabTitle="Tab 1">
    <ion-refresher slot="fixed" @ionRefresh="doRefresh($event)">
      <ion-refresher-content> </ion-refresher-content>
    </ion-refresher>
    <ion-icon
      v-if="updateDoorOpen && loaded"
      :icon="lockOpenOutline"
      size="large"
      @click="closeDoor"
    />
    <ion-icon
      v-else-if="loaded"
      :icon="lockClosedOutline"
      size="large"
      @click="openDoor"
    />
    <ion-button @click="position"> Position </ion-button>
    {{ posLat }}
    {{ posLang }}
  </base-layout>
</template>
<script>
//import { fetchData } from "../services/dataFetch.js";
import { IonRefresher, IonRefresherContent } from "@ionic/vue";
import { getLocation, longitude, latitude } from "../../services/location";
import {
  wifi,
  closeCircle,
  lockClosedOutline,
  lockOpenOutline,
} from "ionicons/icons";
import {
  initGathering,
  postData,
  // messaging,
} from "../../services/firebase-realtimedb";
//import { ref, onValue /*get, child*/ } from "firebase/database";

export default {
  components: { IonRefresher, IonRefresherContent },
  data() {
    return {
      posting: false,
      fetching: false,
      temperatureValue: 0,
      connected: false,
      loaded: false,
      doorOpen: false,
      posLang: 0,
      posLat: 0,
    };
  },
  setup() {
    return {
      wifi,
      closeCircle,
      lockClosedOutline,
      lockOpenOutline,
    };
  },
  computed: {
    updateDoorOpen: function () {
      if (this.loaded) {
        return this.$store.getters.getValue("doorOpen");
      }
      return false;
    },
  },
  async mounted() {
    //this.checkConnectionWrapper();
    initGathering("var");
    initGathering("doorOpen");
    this.loaded = true;
    //this.fetchEveryhing();
  },
  async updated() {
    //initGathering();
    //this.checkConnectionWrapper();
    //this.fetchEveryhing();
  },
  methods: {
    fetchEveryhing() {
      this.fetching = true;
      // fetchData("temperature").then((res) => {
      //   this.temperatureValue = res.data["temperature"];
      //   this.fetching = false;
      // });
    },
    doRefresh(event) {
      this.fetchEveryhing();
      event.target.complete();
    },
    closeDoor() {
      postData("doorOpen", false);
    },
    openDoor() {
      postData("doorOpen", true);
    },
    position() {
      getLocation();
      this.posLang = longitude;
      this.posLat = latitude;
    },
  },
};
</script>
<style></style>
