import time
import requests
import math
import random

TOKEN = "YjO0h3qbrIvaPc16"  # Put your TOKEN here
VARIABLE_LABEL_1 = "temp"  # Put your first variable label here
VARIABLE_LABEL_2 = "hum"  # Put your second variable label here

def build_payload(variable_1, variable_2):
    # Creates two random values for sending data
    value_1 = random.randint(-10, 50)
    value_2 = random.randint(0, 85)
    payload = {'values':
                    {'temp': value_1,
                    'hum': value_2}}
    return payload


def post_request(payload):
    # Creates the headers for the HTTP requests
    url = "http://192.168.69.212:8000/api/metrics/" + TOKEN
    headers = {"Content-Type": "application/json"}

    # Makes the HTTP requests
    status = 400
    attempts = 0
    while status >= 400 and attempts <= 5:
        req = requests.post(url=url, headers=headers, json=payload)
        status = req.status_code
        attempts += 1
        time.sleep(1)

    # Processes results
    if status >= 400:
        print("[ERROR] Could not send data after 5 attempts, please check \
            your token credentials and internet connection")
        return False

    print("[INFO] request made properly, your device is updated")
    return True


def main():
    payload = build_payload(VARIABLE_LABEL_1, VARIABLE_LABEL_2)

    print("[INFO] Attemping to send data")
    post_request(payload)
    print("[INFO] finished")


if __name__ == '__main__':
    while (True):
        main()
        time.sleep(1)