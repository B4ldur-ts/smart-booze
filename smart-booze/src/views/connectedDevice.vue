<template>
  <ion-page>
    <ion-header>
      <ion-toolbar>
        <ion-title>Tab 3</ion-title>
      </ion-toolbar>
    </ion-header>
    <ion-content :fullscreen="true">
      <ion-header collapse="condense">
        <ion-toolbar>
          <ion-title size="large" slot="start">Connected Device</ion-title>
          <ion-back-button
            default-href="/tabs/settings"
            slot="primary"
            :text="'Zurück'"
            :icon="'none'"
          >
          </ion-back-button>
        </ion-toolbar>
      </ion-header>
      Your Device ID is: <br /> <br /> {{id}}
    </ion-content>
  </ion-page>
</template>

<script>
import { defineComponent } from "vue";
import {
  IonPage,
  IonHeader,
  IonToolbar,
  IonTitle,
  IonContent,
  IonBackButton,
} from "@ionic/vue";

export default defineComponent({
  name: "Personal",
  components: {
    IonHeader,
    IonToolbar,
    IonTitle,
    IonContent,
    IonPage,
    IonBackButton,
  },
  computed: {
    id() {
      let allMeasurementsObject = this.$store.getters.deviceId;
      return allMeasurementsObject;
    },
  },
});
</script>
