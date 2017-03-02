import SocketServer
import socket
import time
import re
import json
import sys

class ClientHandler(SocketServer.BaseRequestHandler):
    """
    This is the ClientHandler class. Everytime a new client connects to the
    server, a new ClientHandler object will be created. This class represents
    only connected clients, and not the server itself. If you want to write
    logic for the server, you must write it outside this class
    """

    def finish(self):
        self.active = False

    def handle(self):
        """
        This method handles the connection between a client and the server.
        """
        self.ip = self.client_address[0]
        self.port = self.client_address[1]
        self.connection = self.request
        self.logged_in = False
        self.active = True

        # Loop that listens for messages from the client
        while self.active:
            received_data = ""
            try:
                received_string = self.connection.recv(4096)
                received_data   = json.loads(received_string)
            except ValueError:
                continue

            self.server.callback(received_data)

class ThreadedTCPServer(SocketServer.ThreadingMixIn, SocketServer.TCPServer):
    allow_reuse_address = True