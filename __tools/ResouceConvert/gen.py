#coding=utf-8

"""
生成资源.h头文件
"""

import logging
import config
import os
import time



R_CONTINAER = {
    "music":[],
    "color":[],
    "string":[],
    "sprite":[],
    "patch_sprite":[],
    "texture":[],
    "ui":[],
    "scene":[],
    }


R_HEADER = """
/* AUTO-GENERATED FILE.  DO NOT MODIFY.
 *
 * This class was automatically generated by the
 * convert tool from the resource data it found.  It
 * should not be modified by hand.
 * created time: TIME_STAMP
 */
#ifndef __GENERATE_TYPE_H__
#define __GENERATE_TYPE_H__

class TYPE
{
public:
"""

FOOTER = """
};
#endif
"""


UI_CONTINAER = []

UI_HEADER = """
/* AUTO-GENERATED FILE.  DO NOT MODIFY.
 *
 * This class was automatically generated by the
 * convert tool from the resource data it found.  It
 * should not be modified by hand.
 * created time: TIME_STAMP
 */
#ifndef __GENERATE_UI_H__
#define __GENERATE_UI_H__

class UI
{
public:
"""

UI_COMMAND_INDEX = config.UI_COMMAND_START
UI_COMMAND_CONTAINER = []

UI_COMMAND_HEADER = """
/* AUTO-GENERATED FILE.  DO NOT MODIFY.
 *
 * This class was automatically generated by the
 * convert tool from the resource data it found.  It
 * should not be modified by hand.
 * created time: TIME_STAMP
 */
#ifndef __GENERATE_UI_COMMAND_H__
#define __GENERATE_UI_COMMAND_H__

class UI_COMMAND
{
public:
"""


def add_item(resouce_type, item):
    if resouce_type not in R_CONTINAER:
        logging.error("in generate.add_item resouce_type %s error!", resouce_type)
        return 
    R_CONTINAER[resouce_type].append(item)
 
    
def generate_resource(resouce_type):
    logging.info("generate RES_%s.h", resouce_type.upper())
    generate_path = os.path.join(config.GENERATE_PATH, "RES_%s.h"%resouce_type.upper())
    out = open(generate_path, "wb")
    timestr = time.strftime("%Y-%m-%d %H:%M:%S", time.localtime())
    newHeader = (R_HEADER.replace("TIME_STAMP", timestr))
    out.write(newHeader.replace("TYPE", "RES_%s"%resouce_type.upper()))
    
    for (key, value) in R_CONTINAER[resouce_type]:
        variable_content = """
    const static unsigned int %s = %d;"""%(key.upper(), value)
        out.write(variable_content) 
    out.write(FOOTER)
    
    
    
"""
UI 
"""

def add_page(pageName, key, controlItems):
    UI_CONTINAER.append((pageName,key, controlItems))


def generate_page_class(pageName, key, controlItems, out):
    class_header = """
    class %s
    {
    public:"""%pageName
    out.write(class_header)
    id_content = """
        const static unsigned int PGAE_ID = %d;
    """%key
    out.write(id_content)
    for (key, value) in controlItems:
        variable_content = """
        const static unsigned int %s = %d;"""%(key.upper(), value)
        out.write(variable_content)
    class_footer = """
    };
    """
    out.write(class_footer)

def generate_ui():
    logging.info("generate UI.h")
    generate_path = os.path.join(config.GENERATE_PATH, "UI.h")
    out = open(generate_path, "wb")
    timestr = time.strftime("%Y-%m-%d %H:%M:%S", time.localtime())
    out.write(UI_HEADER.replace("TIME_STAMP", timestr))    
    for (pageName, key, controlItems) in UI_CONTINAER:
        generate_page_class(pageName, key, controlItems, out)
    out.write(FOOTER) 
    
"""
UI_COMMAND 
"""    
def get_command():
    global UI_COMMAND_INDEX
    UI_COMMAND_INDEX+=1
    return UI_COMMAND_INDEX

def add_ui_command(pageName, events):
    UI_COMMAND_CONTAINER.append((pageName, events))    
 
def generate_event_class(pageName,events, out): 
    class_header = """
    class %s
    {
    public:"""%pageName
    out.write(class_header)
    for (name, value) in events:
        variable_content = """
        const static short %s = %d;"""%(name.upper(), value)
        out.write(variable_content)
    class_footer = """
    };
    """
    out.write(class_footer)
 
def generate_ui_command():
    logging.info("generate UI_COMMAND.h")
    generate_path = os.path.join(config.GENERATE_PATH, "UI_COMMAND.h")
    out = open(generate_path, "wb")
    timestr = time.strftime("%Y-%m-%d %H:%M:%S", time.localtime())
    out.write(UI_COMMAND_HEADER.replace("TIME_STAMP", timestr)) 
    for (page, events) in UI_COMMAND_CONTAINER:
        generate_event_class(page,events, out)
    max_command = """
    const static unsigned short MAX_COMMAND = %d;"""%(get_command())
    out.write(max_command)    
    out.write(FOOTER)     
    
def generate():
    generate_resource("music")
    generate_resource("ui")
    generate_resource("color")
    generate_resource("sprite")
    generate_resource("patch_sprite")
    

    
def test_r():
    add_item("music", ("luoyan", 12345))   
    add_item("music", ("caojiec", 12222345)) 
    add_item("ui", ("caojiec", 12222345)) 
    add_item("string", ("caojiec", 12222345)) 
    add_item("string", ("caojiecdd", 12222345)) 
    add_item("color", ("caojiec", 12222345)) 
    add_item("sprite", ("caojiec", 12222345)) 
    add_item("patch_sprite", ("caojiec", 12222345)) 
    generate() 

def test_ui():
    pass

if __name__ == "__main__":
    test_ui()
    
