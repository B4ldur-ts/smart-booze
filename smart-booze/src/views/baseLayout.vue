<template>
  <ion-page>
    <ion-header>
      <ion-toolbar>
        <ion-title style="text-align: center">{{ tabTitle }}</ion-title>
        <ion-buttons slot="end">
          <ion-button @click="setOpen(true)">
            <ion-icon v-if="connected === true" :icon="wifi"> </ion-icon>
            <ion-icon v-else :icon="closeCircle"> </ion-icon>
          </ion-button>
        </ion-buttons>
        <ion-buttons slot="start" v-if="showBackButton">
          <ion-back-button default-href="/tabs/messung" ref="backButton">
          </ion-back-button>
        </ion-buttons>
      </ion-toolbar>
    </ion-header>
    <ion-popover
      :is-open="isOpenRef"
      css-class="my-custom-class"
      :event="event"
      :translucent="true"
      @onDidDismiss="setOpen(false)"
    >
      <Popover> </Popover>
    </ion-popover>
    <ion-content fullscreen>
      <slot />
    </ion-content>
  </ion-page>
</template>

<script>
import {
  IonPage,
  IonToolbar,
  IonTitle,
  IonContent,
  IonButtons,
  IonIcon,
  IonPopover,
  IonBackButton,
} from "@ionic/vue";
import { wifi, closeCircle } from "ionicons/icons";
import Popover from "./component_helpers/popover.vue";
import { ref, defineComponent } from "vue";  

export default defineComponent({
  props: ["tabTitle", "backLink", "showBackButton"],
  components: {
    IonPage,
    IonToolbar,
    IonTitle,
    IonContent,
    IonButtons,
    IonIcon,
    IonPopover,
    Popover,
    IonBackButton,
  },
  data() {
    return {
      connected: false,
    };
  },
  setup() {
    const isOpenRef = ref(false);
    const event = ref();
    const setOpen = (state) => {
      isOpenRef.value = state;
    };

    return {
      wifi,
      closeCircle,
      isOpenRef,
      setOpen,
      event,
    };
  },
  methods: {
    checkConnectionWrapper() {
      this.connected = navigator.onLine;
    },
  },
  async mounted() {
    this.checkConnectionWrapper();
    setInterval(this.checkConnectionWrapper, 5);
  },
});
</script>
