<template>
  <ion-header>
    <ion-toolbar>
      <ion-title style="text-align: center">{{ title }}</ion-title>
      <ion-buttons slot="end">
        <ion-button @click="closeModal" color="primary">close</ion-button>
      </ion-buttons>
    </ion-toolbar>
  </ion-header>
  <ion-card>
    <ion-datetime
      id="time"
      picker-format="HH:mm::DD:MMM"
      display-format="HH:mm @ DD. MMMM"
      placeholder="pick a date"
    >
      ></ion-datetime
    >
  </ion-card>
  <ion-card>
    <ion-label position="floating"> Text Area </ion-label>
    <ion-input placeholder="Enter description" v-model="description">
    </ion-input>
  </ion-card>
  <ion-content class="ion-padding">
    <ion-fab horizontal="end" vertical="bottom" slot="fixed">
      <ion-fab-button color="success" @click="postNewDate"
        ><ion-icon size="large" :icon="checkmarkDone"> </ion-icon>
      </ion-fab-button>
    </ion-fab>
  </ion-content>
</template>

<script>
import {
  IonContent,
  IonHeader,
  IonTitle,
  IonToolbar,
  modalController,
  IonIcon,
  IonInput,
  toastController,
} from "@ionic/vue";
import { postDates } from "../../services/firebase-realtimedb";

import { defineComponent } from "vue";
import { checkmarkDone } from "ionicons/icons";

export default defineComponent({
  name: "Modal",
  props: {
    title: { type: String, default: "Super Modal" },
  },
  components: {
    IonContent,
    IonHeader,
    IonTitle,
    IonToolbar,
    IonIcon,
    IonInput,
  },
  data() {
    return {
      content: "Content",
      description: "",
    };
  },
  setup() {
    const closeModal = () => {
      modalController.dismiss();
    };
    return {
      closeModal,
      checkmarkDone,
    };
  },

  methods: {
    async postNewDate() {
      this.dateTime = document.getElementById("time").value;
      if (this.description === "") {
        this.openToastFailure;
      }
      try {
        let strDate =
          this.dateTime.slice(8, 10) +
          this.dateTime.slice(5, 7) +
          this.dateTime.slice(0, 4) +
          this.dateTime.slice(11, 13) +
          this.dateTime.slice(14, 16);
        await postDates(strDate, this.description);
        modalController.dismiss();
      } catch {
        this.openToastFailure();
        return false;
      }
    },
    async openToastFailure() {
      const toast = await toastController.create({
        message: "Enter a date and a description",
        duration: 2000,
      });
      return toast.present();
    },
  },
});
</script>
