import { createRouter, createWebHistory } from '@ionic/vue-router';
import TabsPage from '../views/TabsPage.vue'


import { auth } from "../services/firebase-realtimedb";

const guard = (to, from, next) => {
  try {
    if (auth.currentUser?.uid && auth.currentUser.emailVerified) {
      next();
    } else {
      next("/");
    }
  } catch (error) {
    next("/");
  }
};

const routes = [
  {
    path: "/",
    component: () => import("../views/login.vue"),
  },
  {
    path: '/tabs/',
    component: TabsPage,
    children: [
      {
        path: '',
        redirect: '/tabs/tab1',
        beforeEnter: guard,
      },
      {
        path: 'tab1',
        component: () => import('@/views/Tab1Page.vue'),
        beforeEnter: guard,
      },
      {
        path: 'tab2',
        component: () => import('@/views/Tab2Page.vue'),
        beforeEnter: guard,
      },
      {
        path: 'tab3',
        component: () => import('@/views/settingsTab.vue'),
        beforeEnter: guard,
      }
    ]
  }
]

const router = createRouter({
  history: createWebHistory(process.env.BASE_URL),
  routes
})

export default router
