#coding=utf-8

"""
资源打包
"""
import os,sys
import colors
import patch_sprite
import scene
import sprite
import strings
import util
import time
import logging
import config
import gen
import ui

def convert_font():
    logging.info("convert font....")
    raw_font_path = os.path.join(config.RAW_RESOURCE_PATH, "common", "font", "font")
    bin_font_path = os.path.join(config.BIN_RESOURCE_PATH,  "font")
    util.copy_file(raw_font_path, bin_font_path)

def convert_music():
    logging.info("convert music....")
    raw_music_dir = os.path.join(config.RAW_RESOURCE_PATH, "common", "music")
    for item in os.listdir(raw_music_dir):
        if item == "." or item == ".." or  (not item.endswith(".mp3")):
            continue
        raw_music_path = os.path.join(raw_music_dir, item)
        bin_music_path = os.path.join(config.BIN_RESOURCE_PATH,"music", item)
        util.copy_file(raw_music_path, bin_music_path)
        
                                        
 
def convert_strings():
    logging.info("convert strings....")
    raw_string_path = os.path.join(config.RAW_RESOURCE_PATH, "common/values/", "strings.xml")
    bin_string_path = os.path.join(config.BIN_RESOURCE_PATH, "strings")
    strings.convert(raw_string_path, bin_string_path)

def convert_colors():
    logging.info("convert colors....")
    raw_string_path = os.path.join(config.RAW_RESOURCE_PATH, "common/values/", "colors.xml")
    colors.convert(raw_string_path) 
    
def convert_sprite():
    logging.info("convert sprite....")
    for path in config.SPRITE_PATHES:
        raw_sprite_dir = os.path.join(config.RAW_RESOURCE_PATH, config.TARGET, path)
        for item in os.listdir(raw_sprite_dir):
            if item.endswith(".xml"):
                sprite_item = os.path.join(path, item)
                sprite.convert(sprite_item)
    

def convert_patch_sprite():
    logging.info("convert patch sprite....")
    for path in config.PATCH_SPRITE_PATHES:
        raw_patch_sprite_dir = os.path.join(config.RAW_RESOURCE_PATH, config.TARGET, path)
        for item in os.listdir(raw_patch_sprite_dir):
            if item.endswith(".xml"):
                patch_sprite_item = os.path.join(path, item)
                patch_sprite.convert(patch_sprite_item)
 
 
def visit_ui_dir(arg,dirname,names):
    for name in names:
        path = os.path.join(dirname,name)
        if path.endswith(".xml"):
            tmplen = len(os.path.join(config.RAW_RESOURCE_PATH, config.TARGET))
            ui_path = path[tmplen+1:]
            ui.convert(ui_path)  
    
def convert_ui():
    logging.info("convert ui....")    
    for path in config.UI_PATHS:
        raw_ui_path = os.path.join(config.RAW_RESOURCE_PATH, config.TARGET, path)
        os.path.walk(raw_ui_path,visit_ui_dir,())
    
def convert_scene():
    logging.info("convert scene....")
    for path in config.SCENE_PATHS:
        raw_scene_dir = os.path.join(config.RAW_RESOURCE_PATH, config.TARGET, path)
        for item in os.listdir(raw_scene_dir):
            if item.endswith(".sce"):
                scene_path = os.path.join(path, item)
                scene.convert(scene_path)
    

def convert():
    timestr = time.strftime("%Y-%m-%d-%H-%M-%S", time.localtime())
    log_path = os.path.join(config.RAW_RESOURCE_PATH, "log", "%s.log"%timestr)
    util.init_logger(log_path, "debug")
    startTime = time.time()
    logging.info("start convert, rawResoucePath=%s, binResoucePath=%s, target=%s",
                 config.RAW_RESOURCE_PATH, config.BIN_RESOURCE_PATH, config.TARGET)
    
    if config.CONVERT_FONT:
        convert_font()
        #gen.generate_resource("font")
        

    if config.CONVERT_MUSIC:
        convert_music()
        #gen.generate_resource("music")
        
    if config.CONVERT_STRINGS:
        convert_strings()
        gen.generate_resource("string")
        
    if config.CONVERT_COLORS:
        convert_colors()
        gen.generate_resource("color")
    
    if config.CONVERT_SPRITE:
        convert_sprite()
        convert_patch_sprite()
        gen.generate_resource("sprite")
        gen.generate_resource("patch_sprite")
    
    if config.CONVERT_UI:
        convert_ui()
        gen.generate_ui()
        gen.generate_ui_command() 
        
    if config.CONVERT_SCENE:
        convert_scene()
        gen.generate_resource("scene")
    
     
    endTime = time.time()
    logging.info("convert end, used:%f seconds",endTime-startTime)


def test():
    convert()
    
    
if __name__ == "__main__":
    if len(sys.argv)==2:
        if sys.argv[1] == "ui":
            config.CONVERT_FONT = False
            config.CONVERT_MUSIC = False
            config.CONVERT_SPRITE = False
            config.CONVERT_STRINGS = False
            config.CONVERT_COLORS = False
            config.CONVERT_UI = True
            config.CONVERT_SCENE = False
            convert()
        elif sys.argv[1] == "string":
            config.CONVERT_FONT = False
            config.CONVERT_MUSIC = False
            config.CONVERT_SPRITE = False
            config.CONVERT_STRINGS = True
            config.CONVERT_COLORS = False
            config.CONVERT_UI = False
            config.CONVERT_SCENE = False
            convert()      
        elif sys.argv[1] == "color":
            config.CONVERT_FONT = False
            config.CONVERT_MUSIC = False
            config.CONVERT_SPRITE = False
            config.CONVERT_STRINGS = False
            config.CONVERT_COLORS = True
            config.CONVERT_UI = False
            config.CONVERT_SCENE = False
            convert()             
    else:
        convert()
    


