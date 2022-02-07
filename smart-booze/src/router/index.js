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
        path: 'messung',
        component: () => import('@/views/messung.vue'),
        beforeEnter: guard,
      },
      {
        path: 'verlauf',
        component: () => import('@/views/verlauf.vue'),
        beforeEnter: guard,
      },
      {
        path: 'settings',
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
