import socket
import time


board_host = "192.168.1.189"
board_port = 80
board_address = (board_host, board_port)

client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
time.sleep(2)
client_socket.connect(board_address)

def readStringUntil(char: str) -> str: 
    message = ""
    message += client_socket.recv(1).decode()
    while message[-1] != char:
        message += client_socket.recv(1).decode()

    return message.replace('\n', '')

client_socket.send("elbowAxis 100 15\n".encode())
client_socket.send("baseAxis 2048 15\n".encode())
client_socket.send("pinchAxis 90 15\n".encode())
time.sleep(3)

client_socket.close()
