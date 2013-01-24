#coding=utf-8
"""
字符串
"""
import xml.dom.minidom;
import struct
import gen

def convert(src, dest):
    xmldoc = xml.dom.minidom.parse(src)
    stringDict = {}
    root = xmldoc.documentElement
    out = open(dest, "wb")
    stringNodes = root.getElementsByTagName("string")
    stringCount = len(stringNodes)
    out.write(struct.pack("H", stringCount))
    index = 0
    for stringNode in stringNodes:
        key = stringNode.getAttribute("name")
        value = stringNode.childNodes[0].nodeValue
        print key,value
        if key in stringDict:
            print "Error, dump key key %s"%key
            return 
        else:
            stringDict[key] = value
        utf8Value = value.encode("utf-8")
        # 网络字节序写入长度
        out.write(struct.pack("!h", len(utf8Value)))
        out.write(utf8Value)   
        gen.add_item("string", (key, index))
        index += 1
    out.close()

if __name__ == "__main__":
    convert("C:/github/lotus-game/resource/client/high/values/strings.xml",
            "C:/github/lotus-game/client/resouce/high/strings")
