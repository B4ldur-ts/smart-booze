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
			<ion-button @click="makeCircle"> Make Circle </ion-button>
			<div class="messung">
				<div id="sthElse">
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
				<!-- <div class="messung-center"></div> -->
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
  },
  methods:{
    go(){
      this.rotationAngle += 30;
      translate(this.$refs.pendel, this.rotationAngle);
    },
    makeCircle(){
      let radius = (screen.width/2) * 0.8;
      let step = Math.PI / 20;
      let shiftX = 0 ;
      let shiftY = 0;
		
      for(let i= 0; i<=20; i++){
        let x = Math.round(radius * Math.cos(step*i));
        let y = Math.round(radius * Math.sin(step*i));
        let inputName = 'point_' + i;
        this.$refs[inputName].style.left = x + shiftX +'px';
                this.$refs[inputName].style.bottom = y - shiftY + 'px';
				this.$refs[inputName].style.transform = `rotate(${-(i*9)+90}deg)`;
		
      }

    }
  },
});
</script>

<style scoped>
.messung {
	position: relative;
	margin-top: 30vh;
	margin: auto;
	width: 50%;
	height: 20%;
}
.messung-center {
	height: 25px;
	width: 25px;
	background-color: #bbb;
	border-radius: 50%;
	z-index: 303;
}
.messung-pendel {
	position: relative;
	border-left: solid green;
	height: 20vh;
	margin: auto;
	top: 29.25vh;
	-webkit-transform: rotate(180deg);
	-webkit-transform-origin: 0% 0%;
}
.messung-pendel-container {
	position: relative;
	margin-left: 50%;
}

.point {
	position: absolute;
	display: block;
	height: 10px;
	width: 1px;
	/* border-radius: 5px; */
	background-color: #000;
	z-index: 500;
}

#sthElse {
	position: absolute;
	margin-left: 50%;

	width: 200px;
	height: 200px;

	z-index: 400;
}
</style>
