import socket


board_host = "192.168.1.189"
board_port = 80
board_address = (board_host, board_port)

client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client_socket.connect(board_address)

def readStringUntil(char: str) -> str: 
    message = ""
    message += client_socket.recv(1).decode()
    while message[-1] != char:
        message += client_socket.recv(1).decode()

    return message.replace('\n', '')

data = readStringUntil('\n')
print(data)

client_socket.send("Hello from the client".encode())

client_socket.close()
print("Disconnected from server")


