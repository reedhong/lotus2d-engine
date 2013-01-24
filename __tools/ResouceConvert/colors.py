#coding=utf-8
"""
颜色表
"""
import xml.dom.minidom;
#import struct
import gen

def convert(src):
    xmldoc = xml.dom.minidom.parse(src)
    keyDict = {}
    root = xmldoc.documentElement
    colorNodes = root.getElementsByTagName("color")
    for colorNode in colorNodes:
        key = colorNode.getAttribute("name")
        value = colorNode.childNodes[0].nodeValue
        print key,value
        if key in keyDict:
            print "Error, dump key key %s"%key
            return 
        else:
            keyDict[key] = value
        gen.add_item("color", (key, int(value, 16)))

if __name__ == "__main__":
    convert("C:/github/lotus-game/resource/client/high/values/colors.xml")