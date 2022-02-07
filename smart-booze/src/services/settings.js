import { set, get, del } from "idb-keyval";
import store from "../store/index";

export default function setDarkMode(bool) {
  var darkMode = false;
  if (bool) {
    if (!document.body.classList.contains("dark")) {
      document.body.classList.add("dark");
      darkMode = true;
    }
  } else {
    if (document.body.classList.contains("dark")) {
      document.body.classList.remove("dark");
      darkMode = false;
    }
  }
  store.commit("setdarkMode", darkMode);
  set("darkMode", darkMode);
}

export function setRememberMe(bool) {
  store.commit("setRememberMe", bool);
  set("rememberMe", bool);
}

export function setExtern(key, value) {
  set(key, value).then((res) => {
    return res;
  });
}

export function getExtern(key) {
  get(key).then((res) => {
    return res;
  });
}

export function delExtern(key) {
  del(key).then((res) => {
    return res;
  });
}
