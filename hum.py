from sense_emu import SenseHat
import time

sense = SenseHat()
green = (0, 255, 0)
white = (255, 255, 255)

while True:
    print('-----------------')
    humidity = sense.humidity
    humidity_value = 64 * humidity / 100
    print(humidity)
    
    temp = sense.temp
    print(temp)

    time.sleep(2)
