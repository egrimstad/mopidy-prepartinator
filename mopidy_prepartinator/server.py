class ClientHandler(SocketServer.BaseRequestHandler):
    """
    This is the ClientHandler class. Everytime a new client connects to the
    server, a new ClientHandler object will be created. This class represents
    only connected clients, and not the server itself. If you want to write
    logic for the server, you must write it outside this class
    """

    def finish(self):
        self.active = False
        if active_users.has_key(self.username):
            del active_users[self.username]

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
            received_string = self.connection.recv(4096)
            received_data   = json.loads(received_string, encoding="latin-1")
            request = received_data['request']
            if request == "login":
                value = received_data['content']
                if self.logged_in is False and not active_users.has_key(value) and self.user_pattern.match(value):
                    active_users[value] = self
                    self.username = value
                    self.request.sendall(self.create_response("Server", "info", "Login successful"))
                    self.request.sendall(self.create_response("Server", "history", message_history))
                    self.logged_in = True
                else:
                    self.request.sendall(self.create_response("Server", "error", "Username not valid"))
                continue;
            elif request == "logout":
                if self.logged_in is True:
                    del active_users[self.username]
                    self.request.sendall(self.create_response("Server", "info", "Logout successful"))
                    self.logged_in = False
                else:
                    self.request.sendall(self.create_response("Server", "error", "You are not logged in"))
                continue;
            elif request == "msg":
                if self.logged_in is True:
                    value = received_data['content']
                    response = self.create_response(self.username, "message", value)
                    message_history.append(response)
                    self.request.sendall(response)
                    for username in active_users.keys():
                        if username != self.username:
                            active_users[username].request.sendall(response)
                        
                else:
                    self.request.sendall(self.create_response("Server", "error", "You are not logged in"))
                continue;
            elif request == "names":
                if self.logged_in is True:
                    names = ""
                    self.request.sendall(self.create_response("Server", "info", ", ".join(active_users.keys())))
                else:
                    self.request.sendall(self.create_response("Server", "error", "You are not logged in"))
                continue;
            elif request == "help":
                help_string = """Allowed commands:
                login <username>: Login with your desired username
                logout: Logout from the server
                msg <Message>: Send a message to all users
                names: See all logged in users
                help: See this help text"""
                self.request.sendall(self.create_response("Server", "info", help_string))
            else:
                self.request.sendall(self.create_response("Server", "error", "Command not recognized"))



    def create_response(self, sender, response, content):
        json_dict = {'timestamp': str(time.time()), 'sender': sender, 'response': response, 'content': content}
        string = json.dumps(json_dict, ensure_ascii=False, encoding="latin-1")
        return string.encode('latin-1')




class ThreadedTCPServer(SocketServer.ThreadingMixIn, SocketServer.TCPServer):
    allow_reuse_address = True
