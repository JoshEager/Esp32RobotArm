import socket 

def readStringUntil(char: str) -> str: 
    string = ""
    while True:
        response = client.recv(1).decode()
        if response != char:
            string += response 
        else:
            break

    return string


host = "192.168.1.190"
port = 80
address = (host, port)

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect(address)

client.send("Hello from the client".encode())
response = readStringUntil("\n")
print(response)