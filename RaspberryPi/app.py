from flask import Flask
import RPi.GPIO as GPIO

app = Flask(__name__)

motorPin = 3

GPIO.setmode(GPIO.BCM)
GPIO.setup(motorPin, GPIO.OUT)

@app.route('/<state>')
def hello_world(state):
    if state=1:
        GPIO.output(motorPin, GPIO.HIGH)
    else:
        GPIO.output(motorPin, GPIO.LOW)
    return 'done'

if __name__ == "__main__":
    app.run(host='0.0.0.0', port=8080, debug=True)
