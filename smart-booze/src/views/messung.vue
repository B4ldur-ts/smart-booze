<template>
  <ion-page>
    <ion-header>
      <ion-toolbar>
        <ion-title>Messung</ion-title>
      </ion-toolbar>
    </ion-header>
    <ion-content :fullscreen="true">
      <ion-header collapse="condense">
        <ion-toolbar>
          <ion-title size="large">Messung</ion-title>
        </ion-toolbar>
      </ion-header>
    <ion-button @click="go">
      Go!
    </ion-button>
    <div class="messung">
      <div class="messung-center">
      </div>
      <div class="messung-pendel-container">
        <div class="messung-pendel" ref="pendel">
        </div>
      </div>
    </div>
    </ion-content>
  </ion-page>
</template>

<script >
import { defineComponent } from 'vue';
import { IonPage, IonHeader, IonToolbar, IonTitle, IonContent } from '@ionic/vue';
import {translate} from "@/services/animate"

export default  defineComponent({
  name: 'Messung',
  components: {  IonHeader, IonToolbar, IonTitle, IonContent, IonPage },
    data() {
    return {
      rotationAngle :  0,
    };
  },
  methods:{
    go(){
      this.rotationAngle += 90;
      translate(this.$refs.pendel, this.rotationAngle);
    }
  },
});
</script>

<style scoped>
.messung{
  position: relative;
  margin-top: 50vh;
  margin-left: 50vw;
  margin-right: 50vw;
}
  .messung-center{
   
    height: 25px;
    width: 25px;
    background-color: #bbb;
    border-radius: 50%;
    z-index: 303;
  }
  .messung-pendel{
    position: absolute;
    border-left: solid green;
    height: 20vh;
    top: -15px;
    left: 15px;
  
    -webkit-transform: rotate(180deg);
    -webkit-transform-origin: 0% 0%;
  }
  .messung-pendel-container{
    position: relative;
  }
</style>
