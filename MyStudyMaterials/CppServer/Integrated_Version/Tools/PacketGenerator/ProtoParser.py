
class ProtoParser():
	
	def __init__(self, start_id, recv_prefix, send_prefix, namespace):
		self.recv_pkt = []	# 수신 패킷 목록
		self.send_pkt = []	# 송신 패킷 목록
		self.total_pkt = []	# 모든 패킷 목록
		self.start_id = start_id
		self.id = start_id
		self.recv_prefix = recv_prefix
		self.send_prefix = send_prefix
		self.namespace = namespace
		

	def parse_proto(self, path):
		f = open(path, 'r')
		lines = f.readlines()

		# ~.proto파일을 전달
		for line in lines:
			if line.startswith('message') == False:
				continue
			# 프로토콜 구조체 이름 파싱하기
			pkt_name = line.split()[1].upper()
			# recv_prefix인 C_인 경우와 send_prefix인 S_인 경우에 따라 처리
			if pkt_name.startswith(self.recv_prefix):
				self.recv_pkt.append(Packet(pkt_name, self.id))
			elif pkt_name.startswith(self.send_prefix):
				self.send_pkt.append(Packet(pkt_name, self.id))
			else:
				continue

			self.total_pkt.append(Packet(pkt_name, self.id))
			self.id += 1

		f.close()


class Packet:
	def __init__(self, name, id):
		self.name = name
		self.id = id