
class ProtoParser():
	# 파이썬에서는 생성자는 무조건 __init__ 이다.
	# 파이썬 함수의 첫번째 인자가 self이면 this를 의미하므로 함수 호출 시에는 두번째 인자부터 넘겨준다
	def __init__(self, start_id, recv_prefix, send_prefix):
		# 파이썬은 생성자에서 클래스의 멤버 변수를 선언한다.
		# self는 this 포인터의 역할이며 self.멤버 변수명으로 클래스의 멤버 변수를 선언한다.
		# =[] 는 동적 배열을 받겠다는 의미
		self.recv_pkt = []	# 수신 패킷 목록
		self.send_pkt = [] # 송신 패킷 목록
		self.total_pkt = [] # 모든 패킷 목록 (enum 항목 작성을 위해 전채 패킷 목록 필요)
		self.start_id = start_id
		self.id = start_id
		self.recv_prefix = recv_prefix
		self.send_prefix = send_prefix

	def parse_proto(self, path):
		f = open(path, 'r') # open은 파일 입출력 매서드이며 'r' 은 읽기 옵션 적용
		lines = f.readlines() # readlines는 열려진 파일을 한줄씩 읽는다

		# ~.proto 파일 파싱 시작
		for line in lines:
			if line.startswith('message') == False: # 해당 라인이 message로 시작하지 않으면 다음 줄로 넘어간다
				continue

			# message로 시작하는 라인에 대한 파싱 시작
			# split은 해당 라인의 각 단어들을 배열로 추출한다. upper매서드는 대문자 변환 기능
			pkt_name = line.split()[1].upper() # 프로토 객체 정의 시 message 다음에 오는 단어인 패킷 명을 저장
			if pkt_name.startswith(self.recv_prefix): # 패킷 명이 recv_prefix로 시작한다면
				self.recv_pkt.append(Packet(pkt_name, self.id)) # 동적 배열 recv_pkt에 Packet(pkt_name, self.id) 추가
			elif pkt_name.startswith(self.send_prefix):# 패킷 명이 send_prefix로 시작한다면
				self.send_pkt.append(Packet(pkt_name, self.id)) # 동적 배열 send_pkt에 Packet(pkt_name, self.id) 추가
			else:
				continue

			self.total_pkt.append(Packet(pkt_name, self.id)) # enum 항목 작성을 위해 전채 패킷 목록 필요
			self.id += 1

		f.close()

class Packet:
	def __init__(self, name, id):
		self.name = name
		self.id = id