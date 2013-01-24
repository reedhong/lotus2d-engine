#coding=utf-8

"""
patch资源打包
"""
import xml.dom.minidom;
import struct
import os
#import sys
import util
import gen
import config
import sprite
import logging


def convert_patches(node, out):
    patchNodes = node.getElementsByTagName("magicbg")
    patch_count = len(patchNodes)
    out.write(struct.pack("H", patch_count))
    for patchNode in patchNodes:
        fmoduleNodes = patchNode.getElementsByTagName("fmodule")
        fmodule_count = len(fmoduleNodes)
        patch_type = int(patchNode.getAttribute("type"))
        # B放在H前面打包会出现错误，怀疑这里面也涉及字节对齐的问题,暂时分开
        out.write(struct.pack("B", patch_type));
        out.write(struct.pack("H", fmodule_count))
        for fmoduleNode in fmoduleNodes:
            x = int(fmoduleNode.getAttribute("x"))
            y = int(fmoduleNode.getAttribute("y"))
            trans = int(fmoduleNode.getAttribute("trans"))
            moduleIndex = int(fmoduleNode.getAttribute("moduleindex"))
            #print x, y, moduleIndex, trans
            out.write(struct.pack("3hB", x, y, moduleIndex, trans))



def convert(raw_path):
    src = os.path.join(config.RAW_RESOURCE_PATH, config.TARGET, raw_path)
    print "parse xml %s"%src
    xmldoc = xml.dom.minidom.parse(src)
    srcKey = util.get_path_key(raw_path)
    root = xmldoc.documentElement
    dest = os.path.join(config.BIN_RESOURCE_PATH, config.TARGET, str(srcKey))
    sprite_name = raw_path[7:-4].replace("\\", "_")
    out = open(dest, "wb")
    
    #for test
    #out.write(struct.pack("h", -10));
    #image
    imagesNode = root.getElementsByTagName("images")[0]
    sprite.convert_images(imagesNode, out)
    
    #modules
    modulesNode = root.getElementsByTagName("modules")[0]
    sprite.convert_modules(modulesNode, out)
    
    #patch
    patchesNode = root.getElementsByTagName("magicbgs")[0]
    convert_patches(patchesNode, out)
    logging.info("convert patch_sprite %s"%dest)
    out.close()
    gen.add_item("patch_sprite", (sprite_name, srcKey))
    
if __name__ == "__main__":
    resPath = "C:/github/lotus-game/resource/client/high"
    convPath = "C:/github/lotus-game/client/resouce/high"
    convert(resPath, convPath, "avatar/ui/magicbg/ui_magicbg.xml")     

