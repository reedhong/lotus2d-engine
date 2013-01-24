#coding=utf-8

"""
UI资源打包
file.write(pack("idh", 12345, 67.89, 15)) 
"""
import xml.dom.minidom;
import struct
import sys,os
import config
import util
import gen


#control types
eCtrlBase = -1
eCtrlText = 0
eCtrlInput = 1
eCtrlAnimation =2
eCtrlImage = 3
eCtrlPanel = 4
eCtrlPatch = 5
eCtrlPage = 6
eCtrlList = 7
eCtrlTab = 8
eCtrlTabItem = 9

EVENTS = []

def convert_control(node, out):
	x = int(node.getAttribute("x"))
	y = int(node.getAttribute("y"))	
	width = int(node.getAttribute("width"))
	height = int(node.getAttribute("height"))
	mainEventName = node.getAttribute("mainEvent")
	bgSpriteKey = int(node.getAttribute("bgSpriteKey"))
	aframeIndex = int(node.getAttribute("aframeIndex"))
	focusedAframeIndex = int(node.getAttribute("focusedAframeIndex"))
	mainEvent = 0
	if mainEventName:
		for mainEventItem in mainEventName.split(","):
			mainEvent = gen.get_command()
			EVENTS.append((mainEventItem, mainEvent))	
	try:
		# 这里必须拆开来写，否则会出现字节序对齐的问题，python中遵循的规则跟c一样，容易出错，导致不紧凑
		out.write(struct.pack("5h", x, y, width, height, mainEvent))
		out.write(struct.pack("i", bgSpriteKey))	
		out.write(struct.pack("2h", aframeIndex, focusedAframeIndex))
	except Exception:
		print x,y, width, height, mainEvent, bgSpriteKey, aframeIndex,focusedAframeIndex
		sys.exit(0)
	visible = int(node.getAttribute("visible"))
	focusable = int(node.getAttribute("focusable"))
	out.write(struct.pack("2b", visible, focusable))
	
def convert_content_attribute(node, out):
	content = node.getAttribute("content")
	utf8_content = content.encode("utf-8")
	contentLen = len(utf8_content)
	# 网络字节序写入长度
	out.write(struct.pack("!H", contentLen))
	out.write(utf8_content)	
	font_size = int(node.getAttribute("font_size"))
	font_color = int(node.getAttribute("font_color"))
	font_border_color = int(node.getAttribute("font_border_color"))
	font_align = int(node.getAttribute("font_align"))
	font_direction = int(node.getAttribute("font_direction"))
	#此处非常诡异，要是导出为BiiBB，则在读取的时候就会出现错误
	out.write(struct.pack("2i3B",font_color, font_border_color,
						font_size,font_align, font_direction))	

def convert_animation(node, out):
	out.write(struct.pack("B", eCtrlAnimation))
	convert_control(node, out)
	spriteKey =int(node.getAttribute("spriteKey"))
	action = int(node.getAttribute("action"))
	disable = int(node.getAttribute("disable"))
	out.write(struct.pack("ihb", spriteKey, action, disable))

def convert_image(node, out):
	out.write(struct.pack("B", eCtrlImage))
	convert_control(node, out)

	
def convert_input(node, out):
	out.write(struct.pack("B", eCtrlInput))
	convert_control(node, out)
	convert_content_attribute(node,out)
	maxlen = int(node.getAttribute("maxlen"))
	gap = int(node.getAttribute("gap"))
	inputmode = int(node.getAttribute("inputmode"))
	disable = int(node.getAttribute("disable"))
	out.write(struct.pack("2H2B", maxlen, gap, inputmode, disable))

def convert_panel(node, out):
	out.write(struct.pack("B", eCtrlPanel))
	convert_control(node, out)

def convert_patch(node,out):
	out.write(struct.pack("B", eCtrlPatch))
	convert_control(node, out)
	patchSpriteKey =int(node.getAttribute("patchSpriteKey"))
	index = int(node.getAttribute("index"))	
	out.write(struct.pack("i", patchSpriteKey))
	out.write(struct.pack("h", index))

def convert_text(node, out):
	out.write(struct.pack("B", eCtrlText))
	convert_control(node, out)
	convert_content_attribute(node,out)
	
def convert_page(node, out):
	convert_control(node, out)
	# page的类型特殊处理，放在后面
	out.write(struct.pack("b", eCtrlPage))
	
def parser_node(node, out, controlItems, index, parentName):
	convert_func_name = "convert_%s"%node.nodeName
	convert_func = globals()[convert_func_name]
	convert_func(node, out)
	
	name = node.getAttribute("name")
	if parentName:
		# 加上父节点的名字，以防止名字出现重复,虽然这种情况不多
		name = "%s_%s"%(parentName, name) 
	
	# root node 不需要加入
	if convert_func != convert_page:
		controlItems.append((name, index))
	
	hasChild = int(node.getAttribute("hasChild"))
	if hasChild:
		size = 0
		for childNode in node.childNodes:
			if childNode.nodeType  == node.ELEMENT_NODE:	
				size += 1
		out.write(struct.pack("B", size))	
		index = 0	
		for childNode in node.childNodes:
			if childNode.nodeType  == node.ELEMENT_NODE:
				if convert_func != convert_page:
					parser_node(childNode, out, controlItems, index, node.getAttribute("name"))
				else: 
					# 根节点无须增加父亲的名字
					parser_node(childNode, out, controlItems, index, "")
				index += 1	
	
	
def convert(ui_path):
	src = os.path.join(config.RAW_RESOURCE_PATH, config.TARGET, ui_path)
	xmldoc = xml.dom.minidom.parse(src)
	root = xmldoc.documentElement
	key = util.get_path_key(ui_path)
	dest = os.path.join(config.BIN_RESOURCE_PATH, config.TARGET, str(key))
	out = open(dest, "wb")
	controlItems = []
	global EVENTS
	EVENTS = []
	parser_node(root, out, controlItems, 0, "")
	out.close()
	page_name = root.getAttribute("name")
	gen.add_page(page_name, key, controlItems)
	gen.add_ui_command(page_name, EVENTS)

if __name__ == "__main__":
	import pdb;pdb.set_trace()
	if len(sys.argv)== 2:
		convert(sys.argv[1]) 
	
	