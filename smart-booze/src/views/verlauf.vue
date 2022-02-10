<template>
	<ion-page>
		<ion-header>
			<ion-toolbar>
				<ion-title>Ergebnisse</ion-title>
			</ion-toolbar>
		</ion-header>
		<ion-content :fullscreen="true">
			<ion-header collapse="condense">
				<ion-toolbar>
					<ion-title size="large">Ergebnisse</ion-title>
				</ion-toolbar>
			</ion-header>

			<ion-grid>
				<ion-row>
					<ion-col> ion-col </ion-col>
					<ion-col> ion-col </ion-col>
					<ion-col> ion-col </ion-col>
					<ion-col> ion-col </ion-col>
				</ion-row>

				<ion-row>
					<ion-col size="6"> ion-col [size="6"] </ion-col>
					<ion-col> ion-col </ion-col>
					<ion-col> ion-col </ion-col>
				</ion-row>

				<ion-row>
					<ion-col size="3"> ion-col [size="3"] </ion-col>
					<ion-col> ion-col </ion-col>
					<ion-col size="3"> ion-col [size="3"] </ion-col>
				</ion-row>

				<ion-row>
					<ion-col size="3"> ion-col [size="3"] </ion-col>
					<ion-col size="3" offset="3">
						ion-col [size="3"] [offset="3"]
					</ion-col>
				</ion-row>

				<ion-row>
					<ion-col> ion-col </ion-col>
					<ion-col>
						ion-col
						<br />#
					</ion-col>
					<ion-col>
						ion-col
						<br /># <br />#
					</ion-col>
					<ion-col>
						ion-col
						<br /># <br /># <br />#
					</ion-col>
				</ion-row>

				<ion-row>
					<ion-col class="ion-align-self-start"> ion-col [start] </ion-col>
					<ion-col class="ion-align-self-center"> ion-col [center] </ion-col>
					<ion-col class="ion-align-self-end"> ion-col [end] </ion-col>
					<ion-col>
						ion-col
						<br /># <br />#
					</ion-col>
				</ion-row>

				<ion-row class="ion-align-items-start">
					<ion-col> [start] ion-col </ion-col>
					<ion-col> [start] ion-col </ion-col>
					<ion-col class="ion-align-self-end"> [start] ion-col [end] </ion-col>
					<ion-col>
						ion-col
						<br /># <br />#
					</ion-col>
				</ion-row>

				<ion-row class="ion-align-items-center">
					<ion-col> [center] ion-col </ion-col>
					<ion-col> [center] ion-col </ion-col>
					<ion-col> [center] ion-col </ion-col>
					<ion-col>
						ion-col
						<br /># <br />#
					</ion-col>
				</ion-row>

				<ion-row class="ion-align-items-end">
					<ion-col> [end] ion-col </ion-col>
					<ion-col class="ion-align-self-start">
						[end] ion-col [start]
					</ion-col>
					<ion-col> [end] ion-col </ion-col>
					<ion-col>
						ion-col
						<br /># <br />#
					</ion-col>
				</ion-row>

				<ion-row>
					<ion-col size="12" size-sm> ion-col [size="12"] [size-sm] </ion-col>
					<ion-col size="12" size-sm> ion-col [size="12"] [size-sm] </ion-col>
					<ion-col size="12" size-sm> ion-col [size="12"] [size-sm] </ion-col>
					<ion-col size="12" size-sm> ion-col [size="12"] [size-sm] </ion-col>
				</ion-row>

				<ion-row>
					<ion-col size="12" size-md> ion-col [size="12"] [size-md] </ion-col>
					<ion-col size="12" size-md> ion-col [size="12"] [size-md] </ion-col>
					<ion-col size="12" size-md> ion-col [size="12"] [size-md] </ion-col>
					<ion-col size="12" size-md> ion-col [size="12"] [size-md] </ion-col>
				</ion-row>

				<ion-row>
					<ion-col size="6" size-lg offset="3">
						ion-col [size="6"] [size-lg] [offset="3"]
					</ion-col>
					<ion-col size="3" size-lg> ion-col [size="3"] [size-lg] </ion-col>
				</ion-row>
			</ion-grid>
		</ion-content>
	</ion-page>
</template>

<script >
import { defineComponent } from 'vue';
import { IonPage, IonHeader, IonToolbar, IonTitle, IonContent, IonGrid, IonRow, IonCol } from '@ionic/vue';

export default defineComponent({
  name: 'Verlauf',
  components: {  IonHeader, IonToolbar, IonTitle, IonContent, IonPage,IonGrid, IonRow, IonCol }
});
</script>

<style scoped>
ion-col {
	background-color: #f7f7f7;
	border: solid 1px #ddd;
	padding: 10px;
}
</style>
