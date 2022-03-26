# -*- coding: utf-8 -*-

# This sample demonstrates handling intents from an Alexa skill using the Alexa Skills Kit SDK for Python.
# Please visit https://alexa.design/cookbook for additional examples on implementing slots, dialog management,
# session persistence, api calls, and more.
# This sample is built using the handler classes approach in skill builder.
import firebase_admin
import pandas as pd
from firebase_admin import credentials
from firebase_admin import db
from firebase_admin import auth
import logging
import ask_sdk_core.utils as ask_utils

from ask_sdk_core.skill_builder import SkillBuilder
from ask_sdk_core.dispatch_components import AbstractRequestHandler
from ask_sdk_core.dispatch_components import AbstractExceptionHandler
from ask_sdk_core.handler_input import HandlerInput

from ask_sdk_model import Response

logger = logging.getLogger(__name__)
logger.setLevel(logging.INFO)

cred = credentials.Certificate('./smart-booze-firebase-adminsdk-8b3go-7581519390.json')

firebase_admin.initialize_app(cred, {
    'databaseURL': 'https://smart-booze-default-rtdb.europe-west1.firebasedatabase.app'
})


class LaunchRequestHandler(AbstractRequestHandler):
    """Handler for Skill Launch."""
    def can_handle(self, handler_input):
        # type: (HandlerInput) -> bool

        return ask_utils.is_request_type("LaunchRequest")(handler_input)

    def handle(self, handler_input):
        # type: (HandlerInput) -> Response
        speak_output = "Willkommen zu Smartbooze. Bist du am sippen?"

        return (
            handler_input.response_builder
                .speak(speak_output)
                .ask(speak_output)
                .response
        )


class HelloWorldIntentHandler(AbstractRequestHandler):
    """Handler for Hello World Intent."""
    def can_handle(self, handler_input):
        # type: (HandlerInput) -> bool
        return ask_utils.is_intent_name("HelloWorldIntent")(handler_input)

    def handle(self, handler_input):
        # type: (HandlerInput) -> Response
        speak_output = "Hallo Welt!"

        return (
            handler_input.response_builder
                .speak(speak_output)
                # .ask("add a reprompt if you want to keep the session open for the user to respond")
                .response
        )


class KaterRezeptIntentHandler(AbstractRequestHandler):
    
    def can_handle(self, handler_input):
        return ask_utils.is_intent_name("KaterRezeptIntent")(handler_input)
        
    def handle(self, handler_input):
        speak_output = "ein Ei, vier Zigaretten, eine Ibuprofen, dazu ein Rosinenbrötchen mit Leberwurst. Und dann kommt Bier ins Spiel! "
        speak_output = speak_output + " Alternativ kannst du viel Wasser trinken und einen Rollmops essen."
        
        return ( 
            handler_input.response_builder
                .speak(speak_output)
                .response
        )


class PromilleIntentHandler(AbstractRequestHandler):
    
    def can_handle(self, handler_input):
        return ask_utils.is_intent_name("PromilleIntent")(handler_input)
        
    def handle(self, handler_input):
        ref = db.reference("devices/29bb3021-e9ce-44d1-9a38-5fe98e89ac83/")
        werte = ref.order_by_key().limit_to_last(1).get()
        
        messwert = 0
        
        for user, val in werte.items():  # iterate until number of people in database is reached
            promill = val.get("value")
            messwert = promill
        
        
        if messwert > 0.3:
            speak_output = "Dein Promillewert ist: " + "{:.1f}".format(messwert).replace('.', ',') + ". Du bist betrunken. Lass das Auto stehen."
        else:
            speak_output = "Dein Promillewert ist: " + "{:.1f}".format(messwert).replace('.', ',') + ". Wenn du das 21. Lebensjahr vollendet hast, darfst du Auto fahren."
        
        return ( 
            handler_input.response_builder
                .speak(speak_output)
                .response
        )

class NuechternIntentHandler(AbstractRequestHandler):
    
    def can_handle(self, handler_input):
        return ask_utils.is_intent_name("NuechternIntent")(handler_input)
        
    def handle(self, handler_input):
        ref = db.reference("devices/29bb3021-e9ce-44d1-9a38-5fe98e89ac83/")
        werte = ref.order_by_key().limit_to_last(1).get()
        
        messwert = 0
        uhrzeit = ""
        
        for user, val in werte.items():
            promill = val.get("value")
            datetime = val.get("date") + " " + val.get("time")
            messwert = promill
            uhrzeit = datetime
        
        values = {'dates':  [uhrzeit]}
        df = pd.DataFrame(values)
        df['dates'] = pd.to_datetime(df['dates'], format='%Y%m%d %H%M%S')
        
        stunden = messwert / 0.15
        uhrzeit_nuechtern = df['dates'].iloc[0] + pd.DateOffset(hours=stunden)
        speak_output = "Du kannst nach " + str(stunden) + " Stunden wieder Auto fahren. Das ist um "
        speak_output = speak_output + str(uhrzeit_nuechtern.hour) + ' Uhr ' + str(uhrzeit_nuechtern.minute)
        
        return ( 
            handler_input.response_builder
                .speak(speak_output)
                .response
        )


class HelpIntentHandler(AbstractRequestHandler):
    """Handler for Help Intent."""
    def can_handle(self, handler_input):
        # type: (HandlerInput) -> bool
        return ask_utils.is_intent_name("AMAZON.HelpIntent")(handler_input)

    def handle(self, handler_input):
        # type: (HandlerInput) -> Response
        speak_output = "Du kannst hallo zu mir sagen! Wie kann ich helfen?"

        return (
            handler_input.response_builder
                .speak(speak_output)
                .ask(speak_output)
                .response
        )


class CancelOrStopIntentHandler(AbstractRequestHandler):
    """Single handler for Cancel and Stop Intent."""
    def can_handle(self, handler_input):
        # type: (HandlerInput) -> bool
        return (ask_utils.is_intent_name("AMAZON.CancelIntent")(handler_input) or
                ask_utils.is_intent_name("AMAZON.StopIntent")(handler_input))

    def handle(self, handler_input):
        # type: (HandlerInput) -> Response
        speak_output = "Tschüss!"

        return (
            handler_input.response_builder
                .speak(speak_output)
                .response
        )


class SessionEndedRequestHandler(AbstractRequestHandler):
    """Handler for Session End."""
    def can_handle(self, handler_input):
        # type: (HandlerInput) -> bool
        return ask_utils.is_request_type("SessionEndedRequest")(handler_input)

    def handle(self, handler_input):
        # type: (HandlerInput) -> Response

        # Any cleanup logic goes here.

        return handler_input.response_builder.response


class IntentReflectorHandler(AbstractRequestHandler):
    """The intent reflector is used for interaction model testing and debugging.
    It will simply repeat the intent the user said. You can create custom handlers
    for your intents by defining them above, then also adding them to the request
    handler chain below.
    """
    def can_handle(self, handler_input):
        # type: (HandlerInput) -> bool
        return ask_utils.is_request_type("IntentRequest")(handler_input)

    def handle(self, handler_input):
        # type: (HandlerInput) -> Response
        intent_name = ask_utils.get_intent_name(handler_input)
        speak_output = "Du hast folgenden Skill getriggert " + intent_name + "."

        return (
            handler_input.response_builder
                .speak(speak_output)
                # .ask("add a reprompt if you want to keep the session open for the user to respond")
                .response
        )


class CatchAllExceptionHandler(AbstractExceptionHandler):
    """Generic error handling to capture any syntax or routing errors. If you receive an error
    stating the request handler chain is not found, you have not implemented a handler for
    the intent being invoked or included it in the skill builder below.
    """
    def can_handle(self, handler_input, exception):
        # type: (HandlerInput, Exception) -> bool
        return True

    def handle(self, handler_input, exception):
        # type: (HandlerInput, Exception) -> Response
        logger.error(exception, exc_info=True)

        speak_output = "Sorry, Ich habe Probleme bei der Ausführung deiner Anfrage. Versuche es bitte erneut."
        return (
            handler_input.response_builder
                .speak(speak_output)
                .ask(speak_output)
                .response
        )

# The SkillBuilder object acts as the entry point for your skill, routing all request and response
# payloads to the handlers above. Make sure any new handlers or interceptors you've
# defined are included below. The order matters - they're processed top to bottom.


sb = SkillBuilder()

sb.add_request_handler(LaunchRequestHandler())
sb.add_request_handler(HelloWorldIntentHandler())
sb.add_request_handler(HelpIntentHandler())
sb.add_request_handler(KaterRezeptIntentHandler())
sb.add_request_handler(PromilleIntentHandler())
sb.add_request_handler(NuechternIntentHandler())
sb.add_request_handler(CancelOrStopIntentHandler())
sb.add_request_handler(SessionEndedRequestHandler())
sb.add_request_handler(IntentReflectorHandler()) # make sure IntentReflectorHandler is last so it doesn't override your custom intent handlers

sb.add_exception_handler(CatchAllExceptionHandler())

lambda_handler = sb.lambda_handler()