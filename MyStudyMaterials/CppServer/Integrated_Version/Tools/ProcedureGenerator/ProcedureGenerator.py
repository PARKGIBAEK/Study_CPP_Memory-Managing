import argparse
import jinja2
import XmlDBParser

def main():
	arg_parser = argparse.ArgumentParser(description = 'StoredProcedure Generator')
	
	# DB Schema 파일(GameDB.xml)의 default 경로를 이 실행 프로그램이 있는 경로로 지정
	exe_dir = os.path.dirname(os.path.abspath(__file__))
	default_xml_path = os.path.join(exe_dir, 'GameDB.xml')
    
	arg_parser.add_argument('--path', type=str, default=default_xml_path, help='Xml Path')
	arg_parser.add_argument('--output', type=str, default='GenProcedures.h', help='Output File')
	args = arg_parser.parse_args()

	if args.path == None or args.output == None:
		print('[Error] --path --output required')
		return
	
	parser = XmlDBParser.XmlDBParser()
	parser.parse_xml(args.path)
	
	file_loader = jinja2.FileSystemLoader('Templates')
	env = jinja2.Environment(loader=file_loader)
	template = env.get_template('GenProcedures.h')
	
	output = template.render(procs=parser.procedures)
	f = open(args.output, 'w+')
	f.write(output)
	f.close()

	print(output)

if __name__ == '__main__':
    main()