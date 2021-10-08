import argparse #프로그램 실행 시 넘겨줄 인자들을 편하게 파싱하기 위한 모듈(라이브러리)
import jinja2 #소스 파일에 jinja2 문법에 해당하는 부분에 정의한 동작에 따라 치환된 내용을 넣어줌
import ProtoParser # 사용자 정의 클래스

def main():
	#파이썬은 Type 선언이 필요가 없다
	arg_parser = argparse.ArgumentParser(description = 'PacketGenerator') # argparse모듈의 ArgumentParser 생성하기(명칭은 PacketGenerator로 지정)

	# ArgumentParser의 add_argument매서드로 인자를 추가하는 방법
	# 인자명은 --path, 인자의 타입은 문자열, 인자의 default 값 설정 가능, 인자 도움말 )
	arg_parser.add_argument('--path', type=str, default='C:/Rookiss/CPP_Server/Server/Common/Protobuf/bin/Protocol.proto', help='proto path')
	arg_parser.add_argument('--output', type=str, default='TestPacketHandler', help='output file')
	arg_parser.add_argument('--recv', type=str, default='C_', help='recv convention')
	arg_parser.add_argument('--send', type=str, default='S_', help='send convention')
	
	# parse_args매서드는 넘겨받은 인자를 파싱하여 사용할 수 있게 해준다
	args = arg_parser.parse_args() # 즉, arg_parser에 설정된 인자로 모듈을 생성한다

	# ProtoParser라는 유저 클래스의 ProtoParse 매서드를 사용 (인자로 1000, args.recv, args.send 를 넘겨주기 )
	parser = ProtoParser.ProtoParser(1000, args.recv, args.send)
	
	parser.parse_proto(args.path) # parse_proto 매서드에 args.path를 인자로 넘겨줌

	# jinja2 모듈 사용하기 위한 초기화 코드
	file_loader = jinja2.FileSystemLoader('Templates') # Templates라는 폴더를 탐색
	env = jinja2.Environment(loader=file_loader) # jinja2의 Environment를 생성하고 동시에 멤버 변수 loader에 file_loader를 전달
	
	# C++ 소스파일을 참고하여 새로운 소스파일 생성하기
	template = env.get_template('PacketHandler.h') # 파싱할 파일인 PacketHandler.h을 넘겨준다

	# render 매서드에서 PacketHandler.h 파일을 분석하여 jinja2문법이 있는 곳을 치환하기 시작
	output = template.render(parser=parser, output=args.output) # args.output은 add_argument매서드에서 '--output'인자로 넘겨준 파일을 의미한다

	f = open(args.output+'.h', 'w+')
	f.write(output)
	f.close()

	print(output)
	return

#파이썬 main문 실행을 위한 기본 문법
if __name__ == '__main__':
	main() 