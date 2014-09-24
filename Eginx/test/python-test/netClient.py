import socket
def tcpClient():
	clisock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	clisock.connect(('192.168.137.135', 10000))

    
if __name__ == "__main__":
    tcpClient()
