import argparse
import jinja2
import ProtoParser
import os
 
# protobuf 컴파일러로 ~.proto 파일을 컴파일한 결과물과
# 템플릿 엔진 jinja2로 PacketHandler.h 파일에 렌더링한 결과물을
# --output 경로에 생성한다
def main():

	arg_parser = argparse.ArgumentParser(description = 'PacketGenerator')
	# 사용 시 경로 변경
	
	# protobuf 컴파일러(protoc.exe)의 default 경로를 이 실행 프로그램이 있는 경로로 지정
	exe_dir = os.path.dirname(os.path.abspath(__file__))
	default_proto_path = os.path.join(exe_dir, 'Protocol.proto')

	arg_parser.add_argument('--path', type=str, default=default_proto_path, help='proto path')
	arg_parser.add_argument('--output', type=str, default='TestPacketHandler', help='output file')
	arg_parser.add_argument('--recv', type=str, default='C_', help='recv convention')
	arg_parser.add_argument('--send', type=str, default='S_', help='send convention')
	arg_parser.add_argument('--namespace', type=str, default='#error', help='namespace')
	args = arg_parser.parse_args()

	parser = ProtoParser.ProtoParser(1000, args.recv, args.send, args.namespace )
	# ProtoParser로 프로토콜 구조체 이름 파싱하기
	parser.parse_proto(args.path)
	
	# 템플릿 파일을 로드할 경로를 Templates로 설정
	file_loader = jinja2.FileSystemLoader('Templates')
	env = jinja2.Environment(loader=file_loader)

	template = env.get_template('PacketHandler.h')
	# 템플릿 렌더링 결과 파일명을 --output 인자로 설정
	output = template.render(parser=parser, output=args.output)

	# 파일 이름은 args.output에 '.h' 확장자를 붙인 것 & 출력 파일을 쓰기 모드(w+)로 열기
	f = open(args.output+'.h', 'w+')
	f.write(output)
	f.close()

	print(output)
	return

if __name__ == '__main__':
	main()