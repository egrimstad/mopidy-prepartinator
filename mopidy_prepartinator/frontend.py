import pykka
import threading
import logging

logger = logging.getLogger(__name__)

from mopidy import core

from server import ThreadedTCPServer, ClientHandler

class PrepartinatorFrontend(pykka.ThreadingActor, core.CoreListener):
	def __init__(self, config, core):
		super(Server, self).__init__()
		self.core = core
		self.port = config["prepartinator"]["port"]
		self.server = None
		init_server()

	def init_server(self):
		HOST, PORT = get_ip_address(), 9998
	    print('Server running...')
	    print('Host:' + str(HOST))
	    print('Port:' + str(PORT))

	    # Set up and initiate the TCP server
	    server = ThreadedTCPServer((HOST, PORT), ClientHandler)
	    server.serve_forever()

	def command(self, data):
		state = self.core.playback.get_state().get()
		volume = self.core.mixer.get_volume().get()
		logger.info(data)
		if data == "p":
			if state == core.PlaybackState.PLAYING:
				self.core.playback.pause()
			elif state == core.PlaybackState.PAUSED:
				self.core.playback.resume()
			elif state == core.PlaybackState.STOPPED:
				self.core.playback.play()
		elif data == "r":
			self.core.playback.previous()
		elif data == "n":
			self.core.playback.next()
		elif data == "s":
			self.core.playback.stop()
		elif data == "+":
			self.core.mixer.set_volume(volume+2)
		elif data == "-":
			self.core.mixer.set_volume(volume-2)

