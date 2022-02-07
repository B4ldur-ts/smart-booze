<!--With help from Max -->
<template>
	<base-layout tabTitle="Smart Booze">
		<ion-content :fullscreen="true">
			<ion-card>
				<ion-card-header>
					<ion-card-subtitle>Fass' deinen Alkoholkonsum in Zahlen!</ion-card-subtitle>
					<ion-card-title>
						{{
							mode === AuthMode.SignIn ? "Anmelden" : "Registrieren"
						}}</ion-card-title
					>
				</ion-card-header>

				<ion-card-content>
					<form
						@submit.prevent="
							mode === AuthMode.SignIn
								? signInWithEmailAndPassword(email, password, rememberMe)
								: signUpWithEmailAndPassword(name, email, password)
						"
					>
						<!-- Name (nur registrieren) -->
						<ion-item v-if="mode === AuthMode.SignUp">
							<ion-label position="floating">Name</ion-label>
							<ion-input v-model="name"></ion-input>
						</ion-item>

						<!-- E-Mail -->
						<ion-item lines="full">
							<ion-label position="floating">E-Mail</ion-label>
							<ion-input v-model="email"></ion-input>
						</ion-item>

						<!-- Passwort -->
						<ion-item lines="full">
							<ion-label position="floating">Passwort</ion-label>
							<ion-input v-model="password" type="password"></ion-input>
						</ion-item>

						<!-- Passwort vergessen -->
						<ion-item lines="full" v-if="mode === AuthMode.SignIn">
							<ion-text
								color="primary"
								@click="resetPassword(email)"
								style="font-size: small"
							>
								Passwort vergessen?
							</ion-text>
						</ion-item>

						<!-- Eingeloggt bleiben -->
						<ion-item lines="none" v-if="mode === AuthMode.SignIn">
							<ion-checkbox v-model="rememberMe" />
							<ion-label>Daten speichern</ion-label>
						</ion-item>

						<!-- Submit Button -->
						<ion-button
							expand="block"
							color="primary"
							class="ion-margin-top"
							type="submit"
							ref="myBtn"
						>
							{{ mode === AuthMode.SignIn ? "Einloggen" : "Registrieren" }}
						</ion-button>

						<!-- Wechsel zwischen registrieren und login -->
						<ion-button
							expand="block"
							color="secondary"
							class="ion-margin-top"
							@click="switchMode"
						>
							{{ mode === AuthMode.SignIn ? "Registrieren" : "Abbrechen" }}
						</ion-button>
					</form>
				</ion-card-content>

				<!-- Fehler Meldungen -->
				<ion-card-content v-if="errorMsg" class="error-message">
					{{ errorMsg }}
				</ion-card-content>
			</ion-card>
		</ion-content>
	</base-layout>
</template>

<script>
import {
	IonContent,
	IonCard,
	IonCardSubtitle,
	IonCardTitle,
	IonCardHeader,
	IonCardContent,
	IonInput,
	IonText,
	IonButton,
	IonLabel,
	IonItem,
	toastController,
	IonCheckbox,
} from "@ionic/vue";
import { auth } from "../services/firebase-realtimedb.js";
import { reactive, toRefs } from "vue";
import { useRouter } from "vue-router";
import { getAuth, sendPasswordResetEmail } from "firebase/auth";
import {
	addUserToDb,
} from "../services/firebase-realtimedb.js";
import { setRememberMe } from "../services/settings";
import setDarkMode from "../services/settings";
import { get, set, del } from "idb-keyval";

const AuthMode = {
	SignIn: 0,
	SignUp: 1,
};
export default {
	name: "authentication-page",
	components: {
		IonContent,
		IonCard,
		IonCardSubtitle,
		IonCardTitle,
		IonCardHeader,
		IonCardContent,
		IonInput,
		IonItem,
		IonLabel,
		IonButton,
		IonText,
		IonCheckbox,
	},
	data() {
		return {
			rememberMe: false,
		};
	},
	setup() {
		const router = useRouter();
		const state = reactive({
			name: "",
			email: "",
			password: "",
			mode: AuthMode.SignIn,
			errorMsg: "",
		});
		const signInWithEmailAndPassword = async (email, password, rememberMe) => {
			try {
				if (!email || !password) {
					state.errorMsg = "Email and Password Required";
					return;
				}

				await auth.signInWithEmailAndPassword(email, password);

				setRememberMe(rememberMe);
				if (rememberMe) {
					set("email", email);
					set("password", password);
				} else {
					del("email");
					del("password");
				}
				//initGetWholeObject();
				router.push("/tabs/tab1");
			} catch (error) {
				if (
					error.message ===
					"Firebase: A network AuthError (such as timeout, interrupted connection or unreachable host) has occurred. (auth/network-request-failed)."
				) {
					state.errorMsg =
						"No internet connection. Please enable WiFi or mobile data";
				} else if (
					error.message ===
					"Firebase: The email address is badly formatted. (auth/invalid-email)."
				) {
					state.errorMsg = "Please enter a valid email address";
				} else if (
					error.message ===
					"Firebase: There is no user record corresponding to this identifier. The user may have been deleted. (auth/user-not-found)."
				) {
					state.errorMsg = "User is not registered";
				} else if (
					error.message ===
					"Firebase: The password is invalid or the user does not have a password. (auth/wrong-password)."
				) {
					state.errorMsg = "Invalid password. Check your spelling";
				} else {
					state.errorMsg = error.message;
				}
			}
		};
		const signUpWithEmailAndPassword = async (name, email, password) => {
			try {
				if (!name || !email || !password) {
					state.errorMsg = "Name, Email, and Password Required";
					return;
				}
				const authRes = await auth.createUserWithEmailAndPassword(
					email,
					password
				);
				await authRes.user.sendEmailVerification();
				authRes.user.updateProfile({
					displayName: name,
				});
				console.log(authRes.user.displayName);
				addUserToDb(name);
				router.push("/tabs/tab1");
			} catch (error) {
				state.errorMsg = error.message;
			}
		};
		const resetPassword = async (email) => {
			const auth = getAuth();
			sendPasswordResetEmail(auth, email)
				.then(() => {
					openToastSuccessReset();
				})
				.catch((error) => {
					const errorCode = error.code;
					const errorMessage = error.message;
					openToastFailure(errorCode, errorMessage);
				});
		};
		const openToastSuccessReset = async () => {
			const toast = await toastController.create({
				message: "Reset Mail Sent",
				duration: 1500,
			});
			return toast.present();
		};
		const openToastSuccessVerification = async () => {
			const toast = await toastController.create({
				message: "Verification mail sent",
				duration: 1500,
			});
			return toast.present();
		};
		const openToastFailure = async (errorCode, errorMessage) => {
			const toast = await toastController.create({
				message: `${errorCode} + ${errorMessage}`,
				duration: 2000,
			});
			return toast.present();
		};
		return {
			...toRefs(state),
			signInWithEmailAndPassword,
			signUpWithEmailAndPassword,
			AuthMode,
			resetPassword,
			openToastSuccessVerification,
			openToastFailure,
			openToastSuccessReset,
		};
	},
	async mounted() {
		get("darkMode").then((res) => {
			this.$store.commit("setdarkMode", res);
			setDarkMode(res);
		});
		await this.getStoredData();
	},
	async ionViewDidEnter() {
		await this.getStoredData();
	},
	methods: {
		// sets stored Login Data in input fields
		async getStoredData() {
			this.email = await get("email");
			this.password = await get("password");
			this.rememberMe = await get("rememberMe");
		},
		// switches between register and Login mode
		async switchMode() {
			this.mode =
				this.mode === AuthMode.SignIn ? AuthMode.SignUp : AuthMode.SignIn;

			// remove stored login data from input fields for register mode
			if (this.mode === AuthMode.SignUp) {
				this.email = "";
				this.password = "";
				this.rememberMe = false;
			} else {
				await this.getStoredData();
			}
		},
	},
};
</script>

<style scoped>

.error-message {
  color: #842029;
  background-color: #f8d7da;
  border-color: #f5c2c7;
  text-align: center;
}

</style>
