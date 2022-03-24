import { createStore } from "vuex";

export default createStore({
  state: {
    allMeasurements: {},
    deviceId: "29bb3021-e9ce-44d1-9a38-5fe98e89ac83",

  },
  mutations: {
    setdarkMode(state, val) {
      state.darkMode = val;
    },
    setRememberMe(state, val) {
      state.rememberMe = val;
    },
    setAllMeasurements(state, object) {
      state.allMeasurements = object;
    },
    setDeviceId(state, val) {
      state.deviceId = val;
    },

  },
  actions: {},
  getters: {
    darkMode: (state) => {
      return state.darkMode;
    },
    rememberMe: (state) => {
      return state.rememberMe;
    },
    anwesenheit: (state) => {
      return state.anwesenheit;
    },
    allMeasurements: (state) => {
      return state.allMeasurements;
    },
    deviceId: (state) => {
      return state.deviceId;
    },

  },
});
