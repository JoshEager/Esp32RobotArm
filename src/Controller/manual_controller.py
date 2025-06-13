import socket 
from pynput import keyboard
import threading
from time import sleep

def readStringUntil(char: str) -> str: 
    string = ""
    while True:
        response = client.recv(1).decode()
        if response != char:
            string += response 
        else:
            break

    return string

def on_press(key):
    global wIsPressed, aIsPressed, sIsPressed, dIsPressed, eIsPressed, qIsPressed, pIsPressed
    match key.char:
        case "w":
            wIsPressed = True
        case "s":
            sIsPressed = True
        case "p":
            pIsPressed = True
        case "e":
            eIsPressed = True
        case "q":
            qIsPressed = True
        case "a":
            aIsPressed = True
        case "d":
            dIsPressed = True
        

def on_release(key):
        global wIsPressed, aIsPressed, sIsPressed, dIsPressed, eIsPressed, qIsPressed, pIsPressed
        match key.char:
            case "w":
                wIsPressed = False
            case "s":
                sIsPressed = False
            case "p":
                pIsPressed = False
            case "e":
                eIsPressed = False
            case "q":
                qIsPressed = False
            case "a":
                aIsPressed = False
            case "d":
                dIsPressed = False

def sendFunction():
    global wIsPressed, aIsPressed, sIsPressed, dIsPressed, eIsPressed, qIsPressed, pIsPressed
    while True:
        if wIsPressed:
            print("w is pressed")
            client.send("2 7 10\n".encode())
        if sIsPressed:
            print("s is pressed")
            client.send("2 -7 10\n".encode())
        if pIsPressed:
            break
        if eIsPressed:
            print("e is pressed")
            client.send("3 7 10\n".encode())
        if qIsPressed:
            print("q is pressed")
            client.send("3 -7 10\n".encode())
        if aIsPressed:
            print("a is pressed")
            client.send("1 -40 15\n".encode())
        if dIsPressed:
            print("d is pressed")
            client.send("1 40 15\n".encode())

        sleep(.05)

wIsPressed = False
aIsPressed = False
sIsPressed = False
dIsPressed = False
eIsPressed = False
qIsPressed = False
pIsPressed = False


host = "192.168.1.190"
port = 80
address = (host, port)

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect(address)

keyboardThread = keyboard.Listener(on_press=on_press, on_release=on_release)
keyboardThread.start()

sendThread = threading.Thread(target=sendFunction)
sendThread.start()
sendThread.join()

client.close()