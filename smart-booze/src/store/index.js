import { createStore } from "vuex";

export default createStore({
  state: {
    data: {},
    darkMode: false,
    rememberMe: false,
    anwesenheit: {},
    wichteln: false,
    update: {},
    allUsers: [],
    punishments: {},
  },
  mutations: {
    changeData(state, obj) {
      switch (obj.name) {
        case "solarVal":
          state.data.solarVal = obj.object;
          break;
        case "doorOpen":
          state.data.doorOpen = obj.object;
          break;
        case "all":
          state.data = obj.object;
          break;
        default:
          //console.log("Error");
          break;
      }
    },
    setdarkMode(state, val) {
      state.darkMode = val;
    },
    setRememberMe(state, val) {
      state.rememberMe = val;
    },
    setWichteln(state, val) {
      state.wichteln = val;
    },
    setUpdate(state, val) {
      state.update = val;
    },
    setUsers(state, val) {
      state.allUsers = val;
    },
    setPunishments(state, val) {
      state.punishments = val;
    },
    setAnwesenheit(state, object) {
      state.anwesenheit[object.index] = {};
      state.anwesenheit[object.index]["time"] = object.val["time"];
      state.anwesenheit[object.index]["name"] = object.val["name"];
      state.anwesenheit[object.index]["description"] =
        object.val["description"];
      state.anwesenheit[object.index]["coming"] = object.val["coming"];
      state.anwesenheit[object.index]["notComing"] = object.val["notComing"];
    },
  },
  actions: {},
  getters: {
    getValue: (state) => (variableName) => {
      try {
        return state.data[variableName]["value"];
      } catch (error) {
        console.log("Vuex Error" + error);
      }
    },
    getWholeObject: (state) => {
      return state.data;
    },
    darkMode: (state) => {
      return state.darkMode;
    },
    rememberMe: (state) => {
      return state.rememberMe;
    },
    anwesenheit: (state) => {
      return state.anwesenheit;
    },
    wichteln: (state) => {
      return state.wichteln;
    },
    update: (state) => {
      return state.update;
    },
    allUsers: (state) => {
      return state.allUsers;
    },
    punishments: (state) => {
      return state.punishments;
    },
  },
});
