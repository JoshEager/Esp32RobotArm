import socket
import time
from pynput import keyboard


board_host = "192.168.1.189"
board_port = 80
board_address = (board_host, board_port)

client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client_socket.connect(board_address)

wIsPressed = False
aIsPressed = False
sIsPressed = False
dIsPressed = False
eIsPressed = False
qIsPressed = False
running = True

elbowAxisDegrees = 0

def on_press(key): 
    global wIsPressed, aIsPressed, sIsPressed, dIsPressed, eIsPressed, qIsPressed, running
    try:
        if key.char == 'w':
            wIsPressed = True
        elif key.char == 'a':
            aIsPressed = True
        elif key.char == 's':
            sIsPressed = True
        elif key.char == 'd':
            dIsPressed = True
        elif key.char == 'e':
            eIsPressed = True
        elif key.char == 'q':
            qIsPressed = True
        elif key.char == 'p':
            running = False
    except:
        pass

def on_release(key):
    global wIsPressed, aIsPressed, sIsPressed, dIsPressed, eIsPressed, qIsPressed
    try:
        if key.char == 'w':
            wIsPressed = False
        elif key.char == 'a':
            aIsPressed = False
        elif key.char == 's':
            sIsPressed = False
        elif key.char == 'd':
            dIsPressed = False
        elif key.char == 'e':
            eIsPressed = False
        elif key.char == 'q':
            qIsPressed = False
    except:
        pass

listen_thread = keyboard.Listener(on_press=on_press, on_release=on_release) 
listen_thread.start()

while (running):
    if wIsPressed:
        print("wIsPressed")
        if elbowAxisDegrees < 180:
            elbowAxisDegrees += 1
        client_socket.send(f"elbowAxis {elbowAxisDegrees} 15\n".encode())
        time.sleep(.015 + .007)
    if sIsPressed:
        print("sIsPressed")
        if elbowAxisDegrees > 0:
            elbowAxisDegrees -= 1
        client_socket.send(f"elbowAxis {elbowAxisDegrees} 15\n".encode())
        time.sleep(.015 + .007)


listen_thread.stop()
client_socket.close()

def readStringUntil(char: str) -> str: 
    message = ""
    message += client_socket.recv(1).decode()
    while message[-1] != char:
        message += client_socket.recv(1).decode()

    return message.replace('\n', '')
