# 由csv文件生成符合Apple格式的vcf通讯录，生成完成之后需要修改编码为UTF-8

#coding=utf8

import sys

def loadFile(filename):
	fp = open(filename, 'r')
	result = []
	for each in fp: result.append(each)
	fp.close()
	return result

def generateAll(array):
	fp = open('icloud.vcf', 'w')

	reload(sys)
	sys.setdefaultencoding('utf-8')

	ORG = '广东创能科技有限公司'
	ORG = ORG.encode('gb2312')

	prefix='广东创能-'
	prefix = prefix.encode('gb2312')

	for item in array:
		fp.write('BEGIN:VCARD\n')
		fp.write('VERSION:3.0\n')
		fp.write('PRODID:-//Apple Inc.//iOS 6.1.2//EN\n')
		fp.write('N:' + prefix + item[0] + ';;;;\n')
		fp.write('FN:' + prefix + item[0] + '\n')
		fp.write('ORG:' + ORG + ';\n')
		fp.write('item1.EMAIL;type=INTERNET;type=pref:' + item[1] + '\n')
		fp.write('TEL;type=CELL;type=VOICE;type=pref:' + item[3] + '\n')
		fp.write('TEL;type=WORK;type=VOICE:' + item[2] + '\n')
		fp.write('END:VCARD\n')
	fp.close()

if __name__ == '__main__':
	try:
		contacts = loadFile('address.csv')
		result = []
		for string in contacts:
			strlist = string.split(',')
			item = [strlist[1], strlist[2], strlist[5], strlist[6]]
			result.append(item)
		generateAll(result)
	except (Exception), e:
		print e
