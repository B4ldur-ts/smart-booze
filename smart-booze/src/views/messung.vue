<template>
  <ion-page>
    <ion-header>
      <ion-toolbar>
        <ion-title>Messung</ion-title>
      </ion-toolbar>
    </ion-header>
    <ion-content :fullscreen="true">
      <!-- <ion-grid> -->
      <!-- <ion-row id="row-0">
          <ion-col> -->
      <ion-header collapse="condense">
        <ion-toolbar>
          <ion-title size="large">Messung</ion-title>
        </ion-toolbar>
      </ion-header>
      <!-- </ion-col>
        </ion-row> -->
      <ion-grid>
        <ion-row id="row-1">
          <ion-col> Placeholder </ion-col>
        </ion-row>
        <ion-row id="row-2">
          <ion-col>
            <div class="messung">
              <div id="sthElse">
                <span class="text-messung" ref="point_0"> 2,5</span>
                <span class="point" ref="point_1"></span>
                <span class="point" ref="point_2"></span>
                <span class="point" ref="point_3"></span>
                <span class="point" ref="point_4"></span>
                <span class="point" ref="point_5"></span>
                <span class="point" ref="point_6"></span>
                <span class="point" ref="point_7"></span>
                <span class="point" ref="point_8"></span>
                <span class="point" ref="point_9"></span>
                <span class="text-messung" ref="point_10"> 1.25 </span>
                <span class="point" ref="point_11"></span>
                <span class="point" ref="point_12"></span>
                <span class="point" ref="point_13"></span>
                <span class="point" ref="point_14"></span>
                <span class="point" ref="point_15"></span>
                <span class="point" ref="point_16"></span>
                <span class="point" ref="point_17"></span>
                <span class="point" ref="point_18"></span>
                <span class="point" ref="point_19"></span>
                <span class="text-messung" ref="point_20"> 0 </span>
              </div>
              <!-- <div class="messung-center"></div> -->
              <div class="messung-pendel-container">
                <div class="messung-pendel" ref="pendel"></div>
              </div>
            </div>
          </ion-col>
        </ion-row>
        <ion-row id="row-3">
          <ion-col>
            <ion-text id="text-promille" v-if="valuesLoaded">
              {{
                measurements[
                  Object.keys(measurements)[
                    Object.keys(measurements).length - 1
                  ]
                ].value
              }}
              &permil;
            </ion-text>
          </ion-col>
        </ion-row>
        <ion-row id="row-4">
          <ion-col id="col-4-1">
            <ion-text>
              Tage trocken: <br />
              Längste Trocken Streak: <br />
              Tage mit kritischen Werten: <br />
            </ion-text>
          </ion-col>
          <ion-col id="col-4-2" size="3">
            <ion-text>
              {{ daysSober }} <br />
              {{ longestSoberStreak }} <br />
              {{ criticalDays }} <br />
            </ion-text>
          </ion-col>
        </ion-row>
      </ion-grid>
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
  IonGrid,
  IonCol,
  IonRow,
} from "@ionic/vue";
import { translate } from "@/services/animate";
import { getMeasurements } from "../services/firebase-realtimedb";

export default defineComponent({
  name: "Messung",
  components: {
    IonHeader,
    IonToolbar,
    IonTitle,
    IonContent,
    IonPage,
    IonGrid,
    IonCol,
    IonRow,
  },
  data() {
    return {
      rotationAngle: 0,
      valuesLoaded: false,
      criticalDays: 0,
      longestSoberStreak: 0,
      daysSober: 0,
    };
  },
  async mounted() {
    await getMeasurements();
    this.makeCircle();
  },
  computed: {
    measurements() {
      let allMeasurementsObject = this.$store.getters.allMeasurements;
      return allMeasurementsObject;
      // Object.keys(measurements)[Object.keys(measurements).length - 1]
    },
  },
  watch: {
    measurements(newVal) {
      this.valuesLoaded = true;
      this.go(newVal);
      this.getCriticalDays(newVal);
    },
  },
  methods: {
    async go(x) {
      // let x = await this.$store.getters.allMeasurements;
      try {
        if ("value" in x[Object.keys(x)[0]]) {
          let val = x[Object.keys(x)[Object.keys(x).length - 1]].value;
          this.rotationAngle = val * 72 + 90;
          translate(this.$refs.pendel, this.rotationAngle);
        } else {
          console.log("Fehler");
        }
      } catch (error) {
        console.log("test");
      }
    },

    async getCriticalDays(x) {
      let daysArr = [];
      let i = 0;
      for (let element in x) {
        if (x[element]["value"] > 1 && !daysArr.includes(x[element]["date"])) {
          i++;
          daysArr.push(x[element]["date"]);
        }
      }
      this.criticalDays = i;
      i = 0;
    },

    getSoberStreak(x) {
      return x;
    },

    getdaysSober(x) {
      return x;
    },

    getStats(allMeasurements) {
      const x = JSON.parse(JSON.stringify(allMeasurements));
      this.getCriticalDays(x);
      this.getdaysSober(x);
      this.getSoberStreak(x);
    },

    makeCircle() {
      let radius = (screen.width / 2) * 0.8;
      let step = Math.PI / 20;
      let shiftX = 0;
      let shiftY = 0;

      for (let i = 0; i <= 20; i++) {
        let x = Math.round(radius * Math.cos(step * i));
        let y = Math.round(radius * Math.sin(step * i));
        let inputName = "point_" + i;
        this.$refs[inputName].style.left = x + shiftX + "px";
        this.$refs[inputName].style.bottom = y - shiftY + "px";
        this.$refs[inputName].style.transform = `rotate(${-(i * 9) + 90}deg)`;
      }
    },
  },
});
</script>

<style scoped>
.messung {
  position: relative;
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

.text-messung {
  position: absolute;
  display: block;
  z-index: 500;
}

#sthElse {
  position: absolute;
  margin-left: 50%;

  width: 200px;
  height: 200px;

  z-index: 400;
}

ion-row {
  text-align: center;
}

ion-grid {
  height: 100%;
}

#row-2 {
  height: 40%;
}

#row-1 {
  height: 15%;
}

#row-0 {
  height: 15%;
}
#row-3 {
  height: 10%;
}
#row-4 {
  height: 30%;
}

#col-4-1 {
  text-align: left;
}
#col-4-2 {
  text-align: right;
}
ion-col {
  background-color: #f7f7f7;
  border: solid 1px #ddd;
  padding: 10px;
}
</style>
