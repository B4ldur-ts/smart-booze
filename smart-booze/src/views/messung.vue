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
			<ion-button @click="go"> Go! </ion-button>
			<div class="messung">
				<div id="canvas">
					<span class="point" ref="point_0"></span>
					<span class="point" ref="point_1"></span>
					<span class="point" ref="point_2"></span>
					<span class="point" ref="point_3"></span>
					<span class="point" ref="point_4"></span>
					<span class="point" ref="point_5"></span>
					<span class="point" ref="point_6"></span>
					<span class="point" ref="point_7"></span>
					<span class="point" ref="point_8"></span>
					<span class="point" ref="point_9"></span>
					<span class="point" ref="point_10"></span>
					<span class="point" ref="point_11"></span>
					<span class="point" ref="point_12"></span>
					<span class="point" ref="point_13"></span>
					<span class="point" ref="point_14"></span>
					<span class="point" ref="point_15"></span>
					<span class="point" ref="point_16"></span>
					<span class="point" ref="point_17"></span>
					<span class="point" ref="point_18"></span>
					<span class="point" ref="point_19"></span>
					<span class="point" ref="point_20"></span>
				</div>
				<div class="messung-center"></div>
				<div class="messung-pendel-container">
					<div class="messung-pendel" ref="pendel"></div>
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
  mounted(){
    this.makeCircle();
  },
  methods:{
    go(){
      this.rotationAngle += 90;
      translate(this.$refs.pendel, this.rotationAngle);
    },
    makeCircle(){
      let radius = 200;
      let step = Math.PI / 20;
      let shiftX = 300;
      let shiftY = 20;
      for(let i= 0; i<=20; i++){
        let x = radius * Math.cos(step*i);
        let y = radius * Math.sin(step*i);
        let inputName = 'point_' + i;
        this.$refs[inputName].style.left = x + shiftX;
                this.$refs[inputName].style.bottom = y - shiftY;

      }

    }
  },
});
</script>

<style scoped>
.messung {
	position: relative;
	margin-top: 50vh;
	margin-left: 50vw;
	margin-right: 50vw;
}
.messung-center {
	height: 25px;
	width: 25px;
	background-color: #bbb;
	border-radius: 50%;
	z-index: 303;
}
.messung-pendel {
	position: absolute;
	border-left: solid green;
	height: 20vh;
	top: -15px;
	left: 15px;

	-webkit-transform: rotate(180deg);
	-webkit-transform-origin: 0% 0%;
}
.messung-pendel-container {
	position: relative;
}

.point {
	position: absolute;
	display: block;
	height: 10px;
	width: 10px;
	border-radius: 5px;
	background-color: #000;
}

#canvas {
	position: relative;
	margin-top: 1vh;
	margin-left: 50vw;
	margin-right: 50vw;
}
</style>
