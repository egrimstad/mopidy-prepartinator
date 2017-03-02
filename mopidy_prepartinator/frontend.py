import pykka
import threading
import logging

logger = logging.getLogger(__name__)

from mopidy import core

from server import ThreadedTCPServer, ClientHandler

class PrepartinatorFrontend(pykka.ThreadingActor, core.CoreListener):
	def __init__(self, config, core):
		super(PrepartinatorFrontend, self).__init__()
		self.core = core
		self.port = config["prepartinator"]["port"]
		self.server = None
		self.init_server()

	def init_server(self):
		HOST, PORT = '0.0.0.0', 1337
		print('Server running...')
		print('Host:' + str(HOST))
		print('Port:' + str(PORT))

		# Set up and initiate the TCP server
		self.server = ThreadedTCPServer((HOST, PORT), ClientHandler)
		self.server.callback = self.receive

		self.server_thread = threading.Thread(target=self.server.serve_forever)
		self.server_thread.start()

	def on_stop(self):
		self.server.shutdown()
    	#self.server.server_close()
    	print("goodbye, cruel world")

	def receive(self, data):
		self.command(data)

	def command(self, data):
		state = self.core.playback.get_state().get()
		volume = self.core.mixer.get_volume().get()

		request = data["request"]
		value = data["value"]

		if request == "cmd":
			if value == "playpause":
				if state == core.PlaybackState.PLAYING:
					self.core.playback.pause()
				elif state == core.PlaybackState.PAUSED:
					self.core.playback.resume()
				elif state == core.PlaybackState.STOPPED:
					self.core.playback.play()
			elif value == "prev":
				self.core.playback.previous()
			elif value == "next":
				self.core.playback.next()

		elif request == "vol":
			if value == "+":
				self.core.mixer.set_volume(volume+2)
			elif value == "-":
				self.core.mixer.set_volume(volume-2)

