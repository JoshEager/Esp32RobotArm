import socket
import time
from pynput import keyboard

# Dictionary to track key states
key_states = {}

def readStringUntil(char: str) -> str: 
    message = ""
    message += client_socket.recv(1).decode()
    while message[-1] != char:
        message += client_socket.recv(1).decode()
    return message.replace('\n', '')

# Socket setup
board_host = "192.168.1.190"
board_port = 80
board_address = (board_host, board_port)

client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client_socket.connect(board_address)

def on_press(key):
    try:
        key_char = key.char  # Get the character of the key (e.g., 'a', 'd')
        # Only process if the key is newly pressed
        if key_char in ('a', 'd', 's', 'w', 'e', 'q') and (key_char not in key_states or not key_states[key_char]):
            key_states[key_char] = True  # Mark key as pressed
            if key_char == 'a':
                print("sending 'a'")
                client_socket.send("1 cw 15\n".encode())
                print(readStringUntil("\n"))
            elif key_char == 'd':
                print("sending 'd'")
                client_socket.send("1 ccw 15\n".encode())
                print(readStringUntil("\n"))
            elif key_char == 'w':
                print("sending w")
                client_socket.send("2 ccw 10\n".encode())
                print(readStringUntil("\n"))
            elif key_char == 's':
                print("sending s")
                client_socket.send("2 cw 10\n".encode())
                print(readStringUntil('\n'))
            elif key_char == 'e':
                print("sending e")
                client_socket.send("3 ccw 10\n".encode())
                print(readStringUntil("\n"))
            elif key_char == 'q':
                print("sending q")
                client_socket.send("3 cw 10\n".encode())
                print(readStringUntil("\n"))
    except AttributeError:
        pass  # Ignore non-character keys (e.g., shift, ctrl)

def on_release(key):
    try:
        key_char = key.char
        if key_char in ('a', 'd', 's', 'w', 'e', 'q'):
            key_states[key_char] = False  # Mark key as released
            if key_char in ('a', 'd'):
                print("sending 'stop' on axis 1")
                client_socket.send("1 stop 15\n".encode())
                print(readStringUntil("\n"))
            elif key_char in ('w', 's'):
                print("sending 'stop' on axis 2")
                client_socket.send("2 stop 15\n".encode())
                print(readStringUntil("\n"))
            elif key_char in ('e', 'q'):
                print("sending stop on axis 3")
                client_socket.send("3 stop 15\n".encode())
                print(readStringUntil("\n"))
    except AttributeError:
        pass  # Ignore non-character keys

# Set up and start the keyboard listener
with keyboard.Listener(on_press=on_press, on_release=on_release) as listen_thread:
    listen_thread.join()

# Close the socket
client_socket.close()
