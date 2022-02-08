<!--
<template>
  <base-layout tabTitle="Settings Tab">
    <ion-list>
      <ion-item>
        <ion-label> Dark Mode </ion-label>
        <ion-checkbox v-model="darkMode" checked="darkMode"> </ion-checkbox>
      </ion-item>
    </ion-list>
    <div id="logOutButton">
      <ion-button
        v-on:click="logOut"
        expand="block"
        size="normal"
        color="danger"
      >
        Log Out
      </ion-button>
    </div>
  </base-layout>
</template>
-->
<template>
  <ion-page>
    <ion-header>
      <ion-toolbar>
        <ion-title>Settings</ion-title>
      </ion-toolbar>
    </ion-header>
    <ion-content :fullscreen="true">
      <ion-header collapse="condense">
        <ion-toolbar>
          <ion-title size="large">Settings</ion-title>
        </ion-toolbar>
      </ion-header>
    <div class="list-bottom">
    <ion-list lines="none">
      <ion-item :detail="true">
        Persönliches
      </ion-item>
      <ion-item :detail="true">
        Verbundenes Gerät
      </ion-item>
      <ion-item>
        <ion-label> Dark Mode </ion-label>
        <ion-checkbox v-model="darkMode" checked="darkMode"> </ion-checkbox>
      </ion-item>
      <ion-item :detail="true">
        Impressum
      </ion-item>
      <ion-item :detail="true">
        Lizenzen & Copyright
      </ion-item>
    </ion-list>
    </div>
    <div id="logOutButton">
      <ion-button
        v-on:click="logOut"
        expand="block"
        size="normal"
        
      >
      Log Out
      </ion-button>
    </div>
      
    </ion-content>
  </ion-page>
</template>



<script>
//import { postData } from "../services/dataFetch.js";
import { getAuth, signOut } from "firebase/auth";
import { IonPage, IonHeader, IonToolbar, IonTitle, IonContent,// IonList,
  // IonLabel,
  // IonItem,
  // IonButton,
  toastController,
  // IonCheckbox
  } from '@ionic/vue';


import setDarkMode from "../services/settings";
import { ref } from "vue";
import { useRouter } from "vue-router";
import { del } from "idb-keyval";
import { setRememberMe } from "../services/settings";

export default {
  components: {
    // IonList,
    // IonLabel,
    // IonItem,
    // IonButton,
    // IonCheckbox,
    IonHeader, IonToolbar, IonTitle, IonContent, IonPage
  },
    data() {
    return {
      connected: false,
      loaded: false,
      darkMode: this.$store.getters.darkMode,
    };
  },
  setup() {
    const debugMode = ref(false);
    const router = useRouter();
    const logOut = async () => {
      const auth = getAuth();
      const ret = await signOut(auth)
        .then(() => {
          router.push("/");
        })
        .catch((error) => {
          console.log(error);
        });
      await del("password");
      await del("email");
      await del("loggedInBefore");
      setRememberMe(false);
      return ret;
    };
    return {
      debugMode,
      router,
      logOut,
    };
  },
  async mounted() {
    this.loaded = true;
  },
  async updated() {},
  computed: {},
  watch: {
    darkMode() {
      this.toggledarkModeWrapper();
    },
  },
  methods: {
    restartESP() {
      // postData("restart", 1).then((res) => {
      //   if (res) {
      //     this.openToastSuccess();
      //   } else {
      //     this.openToastFailure();
      //   }
      // });
    },
    async openToastSuccess() {
      const toast = await toastController.create({
        message: "Restarting ESP.",
        duration: 1500,
      });
      return toast.present();
    },

    async openToastFailure() {
      const toast = await toastController.create({
        message: "Try again.",
        duration: 2000,
      });
      return toast.present();
    },
    toggledarkModeWrapper() {
      setDarkMode(this.darkMode);
    },
    checkConnection() {
      this.connected = navigator.onLine;
      setInterval(this.checkConnectionWrapper, 5);
    },
  },
};
</script>


<style scoped>
  #logOutButton{
    position: relative;
    margin-top: 2vh;
    width: 70vw;
    margin-left: auto;
    margin-right: auto ;
  }
  
  ion-button{
  --background: transparent;
  --color: red;
  --border-radius: 20px;
  --border-width: 1.5px;
  --border-color: red;
  --border-style: solid;
  --color-activated: white;
  --background-activated: red;
}

.list-bottom{
  position: relative;
  margin-top: 35vh;
}
</style>

