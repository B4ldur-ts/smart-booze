<template>
  <ion-page>
    <ion-header>
      <ion-toolbar>
        <ion-title>Verlauf</ion-title>
      </ion-toolbar>
    </ion-header>
    <ion-content :fullscreen="true">
      <ion-header collapse="condense">
        <ion-toolbar>
          <ion-title size="large">Verlauf</ion-title>
        </ion-toolbar>
      </ion-header>

      <ion-grid>
        <ion-row>
          <ion-item lines="none">
            <!-- <ion-label position="fixed">Gender</ion-label> -->
            <ion-select
              placeholder="Zeitraum"
              interface="popover"
              v-model="timeRange"
            >
              <ion-select-option value="today">Heute</ion-select-option>
              <ion-select-option value="days">Letzte 7 Tage</ion-select-option>
              <ion-select-option value="month"
                >Letzte 3 Monate</ion-select-option
              >
              <ion-select-option value="2022">2022</ion-select-option>
            </ion-select>
          </ion-item>
        </ion-row>
        <ion-row>
          <ion-col>
            <div>
              <lineChart
                ref="refLine"
                :chart-data="testData"
                :options="options"
              >
              </lineChart>
            </div>
          </ion-col>
        </ion-row>
      </ion-grid>
    </ion-content>
  </ion-page>
</template>

<script>
import { defineComponent, ref } from "vue";
import {
  IonPage,
  IonHeader,
  IonToolbar,
  IonTitle,
  IonContent,
  IonGrid,
  IonRow,
  IonCol,
  IonSelect,
  IonSelectOption,
  //IonLabel,
} from "@ionic/vue";
// import { getMeasurements } from "@/services/firebase-realtimedb";
import { LineChart } from "vue-chart-3";
import { Chart, registerables } from "chart.js";
Chart.register(...registerables);
require("datejs");

export default defineComponent({
  name: "Verlauf",
  components: {
    IonHeader,
    IonToolbar,
    IonTitle,
    IonContent,
    IonPage,
    IonGrid,
    IonRow,
    IonCol,
    LineChart,
    IonSelect,
    IonSelectOption,
    // IonLabel,
  },
  data() {
    return {
      chart: "",
      storage: 0,
      timeRange: "today",
    };
  },
  async mounted() {
    this.loaded = true;
    //this.getLastSevenDays();
    this.getPointsDay("20220211");
    //await getMeasurements();
  },
  computed: {
    measurements() {
      let allMeasurementsObject = this.$store.getters.allMeasurements;
      return allMeasurementsObject;
      // Object.keys(measurements)[Object.keys(measurements).length - 1]
    },
    testData() {
      let labels = [];
      let values = [];

      let lastThreeMonths = this.getPointsThreeMonth();
      let todayPoints = this.getPointsDay(Date.today().toString("yyyyMMdd"));
      let lastSevenDays = this.getLastSevenDays();
      switch (this.timeRange) {
        case "today":
          labels = todayPoints["labels"];
          values = todayPoints["points"];
          break;
        case "days":
          labels = lastSevenDays["labels"];
          values = lastSevenDays["points"];
          break;
        case "month":
          values = lastThreeMonths["points"];
          labels = lastThreeMonths["labels"];
          break;
        case "2022":
          labels = [
            "Jan",
            "Feb",
            "Mar",
            "Apr",
            "May",
            "Jun",
            "Jul",
            "Aug",
            "Sep",
            "Oct",
            "Nov",
            "Dec",
          ];
          values = this.getYearPoints(2022);

          break;
      }

      return {
        labels: labels,
        datasets: [
          {
            label: "Promille",
            data: values,
            borderColor: "#424247",
            tension: 0.35,
            borderWidth: 3,
          },
        ],
      };
    },
  },

  setup() {
    // const refLine = ref();
    const options = ref({
      responsive: true,
      animation: {
        duration: 10,
      },
      elements: {
        point: {
          radius: 0,
        },
      },
      scales: {
        y: {
          min: 0,
          grid: {
            drawOnChartArea: false,
          },
          max: 4,
        },
        x: {
          grid: {
            drawOnChartArea: false,
          },
          display: true,
          title: {
            display: true,
            text: "Date",
          },
        },
      },
      plugins: {
        legend: {
          position: "bottom",
        },
        title: {
          display: false,
          text: "Solar Data",
        },
      },
    });
    return {
      options,
      // refLine,
    };
  },
  async updated() {},
  methods: {
    getPointsThreeMonth() {
      const dateNow = new Date();
      const year = dateNow.getFullYear();
      let month = dateNow.getMonth() + 1;
      let avg = [];
      let arr1 = [];
      let arr2 = [];
      let arr3 = [];
      let labels = [];
      if (month === 1) {
        arr1 = this.getMonthPoints("11", year.toString());
        arr2 = this.getMonthPoints("12", (year - 1).toString());
        arr3 = this.getMonthPoints("01", (year - 1).toString());
      } else if (month === 2) {
        arr1 = this.getMonthPoints("12", year.toString());
        arr2 = this.getMonthPoints("01", year.toString());
        arr3 = this.getMonthPoints("02", (year - 1).toString());
      } else {
        arr1 = this.getMonthPoints(
          ("0" + (dateNow.getMonth() - 1)).slice(-2),
          year.toString()
        );
        arr2 = this.getMonthPoints(
          ("0" + dateNow.getMonth()).slice(-2),
          year.toString()
        );
        arr3 = this.getMonthPoints(
          ("0" + (dateNow.getMonth() + 1)).slice(-2),
          year.toString()
        );
      }
      avg = avg.concat(arr1[0], arr2[0], arr3[0]);
      labels = labels.concat(arr1[1], arr2[1], arr3[1]);
      return {
        labels: labels,
        points: avg,
      };
    },
    getYearPoints(year) {
      let points = [];
      for (let i = 1; i <= 12; i++) {
        points.push(this.getAverageMonth(("0" + i).slice(-2), year.toString()));
      }
      return points;
    },
    getAverageThreeMonth() {
      const dateNow = new Date();
      const year = dateNow.getFullYear();
      let month = dateNow.getMonth() + 1;
      let avg = 0;
      if (month === 1) {
        avg =
          this.getAverageMonth("01", year.toString()) +
          this.getAverageMonth("12", (year - 1).toString()) +
          this.getAverageMonth("11", (year - 1).toString());
      } else if (month === 2) {
        avg =
          this.getAverageMonth("02", year.toString()) +
          this.getAverageMonth("01", year.toString()) +
          this.getAverageMonth("12", (year - 1).toString());
      } else {
        avg =
          this.getAverageMonth(
            ("0" + dateNow.getMonth()).slice(-2),
            year.toString()
          ) +
          this.getAverageMonth(
            ("0" + (dateNow.getMonth() - 1)).slice(-2),
            year.toString()
          ) +
          this.getAverageMonth(
            ("0" + (dateNow.getMonth() + 1)).slice(-2),
            year.toString()
          );
      }
      return avg / 3;
    },
    getLastSevenDays() {
      let points = [];
      let label = [];

      label.push(Date.today().add(-6).day().toString("dd.MM."));
      label.push(Date.today().add(-5).day().toString("dd.MM."));
      label.push(Date.today().add(-4).day().toString("dd.MM."));
      label.push(Date.today().add(-3).day().toString("dd.MM."));
      label.push(Date.today().add(-2).day().toString("dd.MM."));
      label.push(Date.today().add(-1).day().toString("dd.MM."));
      label.push(Date.today().toString("dd.MM."));

      points.push(
        this.getAverageDay(Date.today().add(-6).day().toString("yyyyMMdd"))
      );
      points.push(
        this.getAverageDay(Date.today().add(-5).day().toString("yyyyMMdd"))
      );

      points.push(
        this.getAverageDay(Date.today().add(-4).day().toString("yyyyMMdd"))
      );

      points.push(
        this.getAverageDay(Date.today().add(-3).day().toString("yyyyMMdd"))
      );

      points.push(
        this.getAverageDay(Date.today().add(-2).day().toString("yyyyMMdd"))
      );
      points.push(
        this.getAverageDay(Date.today().add(-1).day().toString("yyyyMMdd"))
      );
      points.push(this.getAverageDay(Date.today().toString("yyyyMMdd")));

      return {
        labels: label,
        points: points,
      };
    },
    getMonthPoints(month, year) {
      let days = 0;
      const monthArr = [
        "Jan",
        "Feb",
        "Mar",
        "Apr",
        "May",
        "Jun",
        "Jul",
        "Aug",
        "Sep",
        "Oct",
        "Nov",
        "Dec",
      ];
      switch (parseInt(month)) {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
          days = 31;
          break;
        case 2:
          days = 28;
          break;
        case 4:
        case 6:
        case 9:
        case 11:
          days = 30;
          break;
      }
      let label = new Array(days).fill("");
      label[0] = monthArr[parseInt(month) - 1];
      let avg = [];
      for (let i = 1; i <= days; i++) {
        avg.push(
          this.getAverageDay(year + month + String(i).padStart(2, "0") + "")
        );
      }
      return [avg, label];
    },
    getAverageMonth(month, year) {
      let days = 0;
      switch (parseInt(month)) {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
          days = 31;
          break;
        case 2:
          days = 28;
          break;
        case 4:
        case 6:
        case 9:
        case 11:
          days = 30;
          break;
      }
      let avg = 0;
      for (let i = 1; i < days; i++) {
        avg =
          avg +
          this.getAverageDay(year + month + String(i).padStart(2, "0") + "");
      }
      return this.roundTo(avg / days, 2);
    },

    // Day in format "YYYYMMDD"
    getAverageDay(day) {
      let val = [];
      let time = [];
      let allMeasurementsObject = JSON.parse(
        JSON.stringify(this.$store.getters.allMeasurements)
      );
      for (let element in allMeasurementsObject) {
        time.push(
          allMeasurementsObject[element]["date"] +
            allMeasurementsObject[element]["time"]
        );
        val.push(allMeasurementsObject[element]["value"]);
      }
      let sum = 0;
      let i = 0;
      let j = 0;
      for (let x in time) {
        if (time[x].includes(day)) {
          sum += val[j];
          i++;
        }
        j++;
      }
      if (i === 0) {
        return 0;
      }
      return this.roundTo(sum / i, 2);
    },
    getPointsDay(day) {
      let val = [];
      let time = [];
      let allMeasurementsObject = JSON.parse(
        JSON.stringify(this.$store.getters.allMeasurements)
      );
      for (let element in allMeasurementsObject) {
        time.push(
          allMeasurementsObject[element]["date"] +
            allMeasurementsObject[element]["time"]
        );
        val.push(allMeasurementsObject[element]["value"]);
      }
      let retObj = {
        points: [],
        labels: [],
      };
      let j = 0;
      for (let x in time) {
        if (time[x].includes(day)) {
          retObj["points"].push(val[j]);
          retObj["labels"].push(
            time[x].slice(8, 10) + ":" + time[x].slice(10, 12)
          );
        }
        j++;
      }
      return retObj;
    },
    roundTo(n, digits) {
      if (digits === undefined) {
        digits = 0;
      }
      var multiplicator = Math.pow(10, digits);
      n = parseFloat((n * multiplicator).toFixed(11));
      var test = Math.round(n) / multiplicator;
      return +test.toFixed(digits);
    },
  },
});
</script>

<style scoped></style>
