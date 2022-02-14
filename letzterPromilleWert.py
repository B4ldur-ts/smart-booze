import firebase_admin
from firebase_admin import credentials
from firebase_admin import db
from collections import OrderedDict

cred = credentials.Certificate('C:/Users/pattymelg/Downloads/smart-booze-firebase-adminsdk-8b3go-7581519390.json')

firebase_admin.initialize_app(cred, {
    'databaseURL': 'https://smart-booze-default-rtdb.europe-west1.firebasedatabase.app'
})

ref = db.reference("devices/29bb3021-e9ce-44d1-9a38-5fe98e89ac83/")

#werte = ref.order_by_key().get()
werte = ref.order_by_child('date').limit_to_last(1).get()
print(werte)

for user, val in werte.items():  # iterate until number of people in database is reached
    print(f'\n{val}')
    who = val.get("value")
    print(who)
