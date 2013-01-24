#coding=utf-8

"""
动画资源打包
"""
import xml.dom.minidom;
import struct
import os,sys
import util
import gen
import config


def convert_images(node, out):
    imageNodes = node.getElementsByTagName("image")
    image_count = len(imageNodes);
    out.write(struct.pack("B", image_count))
    for imageNode in imageNodes:
        image_file = imageNode.getAttribute("file")
        relative_image_file = image_file[3:]
        key = util.get_path_key(relative_image_file)
        raw_image_path = os.path.join(config.RAW_RESOURCE_PATH, config.TARGET,
                                      relative_image_file)
        
        bin_image_path = os.path.join(config.BIN_RESOURCE_PATH, config.TARGET, str(key))     
        util.copy_file(raw_image_path, bin_image_path)
        out.write(struct.pack("I", key))

def convert_modules(node, out):
    moduleNodes = node.getElementsByTagName("module")
    module_count = len(moduleNodes)
    out.write(struct.pack("H", module_count))
    for moduleNode in moduleNodes:
        x = int(moduleNode.getAttribute("x"))
        y = int(moduleNode.getAttribute("y"))
        width = int(moduleNode.getAttribute("width"))
        height = int(moduleNode.getAttribute("height"))
        imageIndex = int(moduleNode.getAttribute("img"))
        out.write(struct.pack("4hB", x, y, width, height, imageIndex))

def convert_frames(node, out):
    frameNodes = node.getElementsByTagName("frame")
    frame_count = len(frameNodes)
    out.write(struct.pack("H", frame_count))
    for frameNode in frameNodes:
        fmoduleNodes = frameNode.getElementsByTagName("fmodule")
        fmodule_count = len(fmoduleNodes)
        out.write(struct.pack("H", fmodule_count))
        for fmoduleNode in fmoduleNodes:
            x = int(fmoduleNode.getAttribute("x"))
            y = int(fmoduleNode.getAttribute("y"))
            trans = int(fmoduleNode.getAttribute("trans"))
            moduleIndex = int(fmoduleNode.getAttribute("module"))
            #print x, y, moduleIndex, trans
            out.write(struct.pack("3hB", x, y, moduleIndex, trans))

def convert_actions(node, out):
    #write bounds
    bound_x = int(node.getAttribute("bound_x"))
    bound_y = int(node.getAttribute("bound_y")) 
    bound_w = int(node.getAttribute("bound_w")) 
    bound_h = int(node.getAttribute("bound_h")) 
    out.write(struct.pack("4h", bound_x,bound_y,bound_w, bound_h))
    
    actionNodes = node.getElementsByTagName("action")
    action_count = len(actionNodes)
    out.write(struct.pack("H", action_count))
    for actionNode in actionNodes:
        # todo: id
        #out.write(struct("4H", px, py))
        aframeNodes = actionNode.getElementsByTagName("aframe")
        aframe_count = len(aframeNodes)
        out.write(struct.pack("H", aframe_count))
        for aframeNode in aframeNodes:
            frameIndex = int(aframeNode.getAttribute("frame"))
            fx = int(aframeNode.getAttribute("fx"))
            fy = int(aframeNode.getAttribute("fy"))
            flip = int(aframeNode.getAttribute("flip"))
            time = int(aframeNode.getAttribute("time")) 
            out.write(struct.pack("4hB", frameIndex, fx, fy, time, flip))
            

def convert(raw_path):
    """
    raw_path 相对路径，去掉taget之后的路径
    """
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
    convert_images(imagesNode, out)
    
    #modules
    modulesNode = root.getElementsByTagName("modules")[0]
    convert_modules(modulesNode, out)
    
    #frames
    framesNode = root.getElementsByTagName("frames")[0]
    convert_frames(framesNode, out)
    
    #actions
    actionsNodes = root.getElementsByTagName("actions")
    if actionsNodes:
        convert_actions(actionsNodes[0], out)
    
    out.close()
    gen.add_item("sprite", (sprite_name, srcKey))
    
if __name__ == "__main__":
    if len(sys.argv)==2:
        resPath = "C:/github/lotus-game/resource/client/high"
        convPath = "C:/github/lotus-game/client/resouce/high"
        convert(resPath, convPath, sys.argv[1])     





