<template>
  <base-layout tabTitle="Post Values">
    <ion-refresher slot="fixed" @ionRefresh="doRefresh($event)">
      <ion-refresher-content> </ion-refresher-content>
    </ion-refresher>
    <ion-list>
      <ion-item>
        <ion-label position="fixed">Temperatur</ion-label>
        <ion-input
          v-model="temperatureValue"
          placeholder="Enter new value"
          type="number"
          value="temperatureValue"
        ></ion-input>
      </ion-item>
      <ion-item>
        <ion-range ref="slider" min="-200" max="200" color="secondary">
          <ion-label slot="start">-200</ion-label>
          <ion-label slot="end">200</ion-label>
        </ion-range>
      </ion-item>
      <ion-item>
        <ion-label>Door lock</ion-label>
        <ion-checkbox
          @update:modelValue="doorLock = $event"
          :modelValue="doorLock"
        >
        </ion-checkbox>
      </ion-item>
      <ion-item>
        <ion-label position=" floating">
          Debug Mode
        </ion-label>
        <ion-toggle v-model="debug" v-on:click="debugModeWrapper"></ion-toggle>
      </ion-item>
    </ion-list>
    <ion-fab horizontal="end" vertical="bottom" slot="fixed">
      <ion-fab-button
        v-if="debug == true"
        v-on:click="showDebug"
        color="success"
        ><ion-icon size="large" :icon="checkmarkDone"> </ion-icon>
      </ion-fab-button>
      <ion-fab-button
        v-on:click="postAllValues"
        v-else-if="posting === false && fetching === false"
        color="success"
        ><ion-icon size="large" :icon="checkmarkDone"> </ion-icon>
      </ion-fab-button>
      <ion-spinner v-else> </ion-spinner>
    </ion-fab>
  </base-layout>
</template>

<script>
import {
  IonFab,
  IonFabButton,
  IonSpinner,
  IonList,
  IonItem,
  IonInput,
  IonLabel,
  IonIcon,
  IonCheckbox,
  toastController,
} from "@ionic/vue";

//import { postData } from "../services/firebase.js";
import { checkmarkDone } from "ionicons/icons";
import { ref } from "vue";

export default {
  components: {
    IonFab,
    IonFabButton,
    IonSpinner,
    IonList,
    IonItem,
    IonInput,
    IonLabel,
    IonIcon,
    IonCheckbox,
  },

  data() {
    return {
      posting: false,
      fetching: false,
      temperatureValue: 0,
      doorLock: false,
      sliderValue: 0,
    };
  },

  setup() {
    const debug = ref(false);
    return {
      debug,
      checkmarkDone,
    };
  },
  async mounted() {
    this.fetchEveryhing();
  },
  async updated() {
    this.fetchEveryhing();
  },
  methods: {
    showDebug() {
      console.log(this.$refs.slider.value);
      console.log(this.debug);
    },
    fetchEveryhing() {
      this.fetching = true;
    },
    postAllValues() {
      return 0;
    },

    doRefresh(event) {
      this.fetchEveryhing();
      event.target.complete();
    },
    async openToastSuccess() {
      const toast = await toastController.create({
        message: "Values have been updated to MCU.",
        duration: 1500,
      });
      return toast.present();
    },
    async openToastFailure() {
      const toast = await toastController.create({
        message: "Values have NOT been updated to MCU.",
        duration: 2000,
      });
      return toast.present();
    },
    debugModeWrapper() {
      this.debug = !this.debug;
    },
    // async postDoorLock() {
    //   this.posting = true;
    //   if (postData("booleanVar", this.doorLock)) {
    //     console.log("success");
    //   } else {
    //     console.log("Du dummer Hund");
    //   }
    //   this.posting = false;
    // },
    // async postDoorLock() {
    //   console.log(this.doorLock);
    //   this.posting = true;
    //   postData("booleanVal", this.doorLock).then((res) => {
    //     if (res) {
    //       this.openToastSuccess();
    //     } else {
    //       this.openToastFailure();
    //     }
    //     this.posting = false;
    //   });
    // },
    debugFunc() {
      console.log("change");
    },
    onChange($event) {
      console.log($event.target.value);
    },
  },
  watch: {
    doorLock: function() {
      this.postDoorLock();
    },
    temperatureValue: function() {
      console.log(this.temperatureValue);
    },
    e: function() {
      console.log(this.$refs.slider.value);
    },
  },
};
</script>
