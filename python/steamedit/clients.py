#!/usr/bin/python
import steam
import csgo
class Desktop:
	def __init__(self, username='', password=''):
		self.username = username
		self.password = password
		self.client = steam.SteamClient()
		self.csgo = csgo.CSGOClient(self.client)
		self.client.cli_login(username=username, password=password)
	def startCSGO(self):
		self.csgo.launch()
