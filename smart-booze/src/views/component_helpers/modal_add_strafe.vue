<template>
	<ion-header>
		<ion-toolbar>
			<ion-title style="text-align: center">{{ title }}</ion-title>
			<ion-buttons slot="end">
				<ion-button @click="closeModal" color="primary">schließen</ion-button>
			</ion-buttons>
		</ion-toolbar>
	</ion-header>

	<ion-card>
		<ion-label position="stacked">Fehlverhalten</ion-label>
		<ion-select
			class="select-full-width"
			v-model="selectedValue"
			interface="action-sheet"
			placeholder="  Wähle Fehlverhalten.."
		>
			<ion-select-option value="Keine Rueckmeldung auf eine Frage"
				>Keine Rückmeldung auf eine Frage</ion-select-option
			>
			<ion-select-option value="Aktion gegen den gesunden Menschenverstand"
				>Aktion gegen den gesunden Menschenverstand</ion-select-option
			>
			<ion-select-option value="Alkohol verschuettet"
				>Alkohol verschüttet</ion-select-option
			>
			<ion-select-option value="Kotzen">Kotzen</ion-select-option>
			<ion-select-option value="LilleTT">LilleTT</ion-select-option>
			<ion-select-option value="custom">Eigene Strafe</ion-select-option>
		</ion-select>
	</ion-card>
	<ion-card v-if="selectedValue === 'custom'">
		<ion-card-content>
			<ion-item>
				<ion-label position="floating">Grund</ion-label>
				<ion-input type="string" v-model="customReason"> </ion-input>
			</ion-item>
			<ion-item>
				<ion-label position="fixed">Strafe</ion-label>
				<ion-input type="number" v-model="customValue"> </ion-input>
				<ion-label>€</ion-label>
			</ion-item>
		</ion-card-content>
	</ion-card>
	<ion-card>
		<ion-label position="stacked"> Wer war's ?</ion-label>
		<ion-select
			class="select-full-width"
			v-model="selectedUser"
			interface="action-sheet"
			placeholder="  Finde den Übeltäter..."
			multiple="false"
		>
			<ion-select-option
				v-for="element in allUsers"
				:key="element"
				:value="element"
			>
				{{ element }}
			</ion-select-option>
		</ion-select>
	</ion-card>
	<ion-content class="ion-padding">
		<ion-fab horizontal="end" vertical="bottom" slot="fixed">
			<ion-fab-button color="success" @click="postPunishment"
				><ion-icon size="large" :icon="checkmarkDone"> </ion-icon>
			</ion-fab-button>
		</ion-fab>
	</ion-content>
</template>

<script>
import {
	IonContent,
	IonHeader,
	IonTitle,
	IonToolbar,
	modalController,
	IonIcon,
	toastController,
	IonSelectOption,
	IonSelect,
	IonLabel,
	IonInput,
	//  IonList,
	IonItem,
	IonCard,
	IonCardContent,
} from "@ionic/vue";
import {
	getAllUsers,
	postNewPunishment,
} from "../../services/firebase-realtimedb";

import { defineComponent } from "vue";
import { checkmarkDone } from "ionicons/icons";

export default defineComponent({
	name: "addStrafe",
	props: {
		title: { type: String, default: "Error" },
	},
	components: {
		IonContent,
		IonHeader,
		IonTitle,
		IonToolbar,
		IonIcon,
		IonSelectOption,
		IonSelect,
		IonLabel,
		IonInput,
		//  IonList,
		IonItem,
		IonCard,
		IonCardContent,
	},

	data() {
		return {
			selectedValue: "",
			selectedUser: "Nothing entered",
			customReason: "",
			customValue: "",
		};
	},
	mounted() {
		getAllUsers();
	},
	setup() {
		const closeModal = () => {
			modalController.dismiss();
		};
		return {
			closeModal,
			checkmarkDone,
		};
	},
	computed: {
		console: () => console,
		allUsers() {
			return this.$store.getters.allUsers;
		},
	},
	methods: {
		async postNewDate() {
			try {
				modalController.dismiss();
			} catch {
				this.openToastFailure();
				return false;
			}
		},
		async openToastFailure() {
			const toast = await toastController.create({
				message: "Gib' einen Übeltäter und seine gerechte Strafe ein!",
				duration: 2000,
			});
			return toast.present();
		},
		async openToastFailureReason() {
			const toast = await toastController.create({
				message: "Was ist der Grund für die harte Strafe?",
				duration: 2000,
			});
			return toast.present();
		},
		async openToastFailurePositive() {
			const toast = await toastController.create({
				message: "Bitte gib' eine Strafe > 0€ ein",
				duration: 2000,
			});
			return toast.present();
		},
		postPunishment() {
			let displayStrafe = 0;
			let displayReason = "";
			if (this.selectedUser === "Nothing entered") {
				this.openToastFailure();
				return;
			}
			if (this.selectedValue === "custom") {
				displayStrafe = parseFloat(this.customValue);
				if (isNaN(displayStrafe)) {
					this.openToastFailure();
					return;
				}
				if (this.customReason === "") {
					this.openToastFailureReason();
					return;
				}
				console.log(displayStrafe);
				if (displayStrafe <= 0.0) {
					this.openToastFailurePositive();
					return;
				}
				displayReason = this.customReason;
			} else if (this.selectedValue === "Alkohol verschuettet") {
				displayStrafe = 0.5;
				displayReason = this.selectedValue;
			} else if (this.selectedValue === "LilleTT") {
				displayStrafe = 0.5;
				displayReason = this.selectedValue;
			} else if (this.selectedValue === "Kotzen") {
				displayStrafe = 2.0;
				displayReason = this.selectedValue;
			} else if (
				this.selectedValue === "Aktion gegen den gesunden Menschenverstand"
			) {
				displayStrafe = 2.0;
				displayReason = this.selectedValue;
			} else if (this.selectedValue === "Keine Rueckmeldung auf eine Frage") {
				displayStrafe = 0.5;
				displayReason = this.selectedValue;
			} else {
				this.openToastFailure();
				return;
			}
			postNewPunishment(this.selectedUser, displayReason, displayStrafe);
			modalController.dismiss();
		},
	},
});
</script>

<style>
.select-full-width {
  max-width: 100% !important;
  width: 100% !important;
  padding-left: 0 !important;
}
</style>
