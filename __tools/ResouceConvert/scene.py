#coding=utf-8
"""
场景转化二进制
"""
import xml.dom.minidom;
#import struct
import sys,os
import config
import util
import gen


def convert(scene_path):
    src = os.path.join(config.RAW_RESOURCE_PATH, config.TARGET, scene_path)
    xmldoc = xml.dom.minidom.parse(src)
    root = xmldoc.documentElement
    key = util.get_path_key(scene_path)
    dest = os.path.join(config.BIN_RESOURCE_PATH, config.TARGET, str(key))
    out = open(dest, "wb")
    #parser_node(root, out, controlItems, 0, "")
    out.close()
    scenename = os.path.split(scene_path)[1][0:-4]
    gen.add_item("scene", (scenename, key))

if __name__ == "__main__":

    if len(sys.argv)!=3:
        convert("../../../resource/client/high/ui/login.xml", "login")
    else:
        os.chdir("C:/github/lotus-game/resource/client/high/ui")
        convert(sys.argv[1], sys.argv[2]) 

