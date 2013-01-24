#coding=utf-8

"""
一些公共的配置属性
"""

import os

# 默认值,可通过setConfig修改
PROJECT_PATH = "C:\\github\\lotus2d-engine"
GAME_NAME = "LandLord"
TARGET="480x800"

RAW_RESOURCE_PATH = os.path.join(PROJECT_PATH, "resource", GAME_NAME)
BIN_RESOURCE_PATH = os.path.join(PROJECT_PATH, "__release", GAME_NAME)
GENERATE_PATH = os.path.join(PROJECT_PATH, "client", GAME_NAME, "Generate")


# 很多情况下不需要copy
CONVERT_FONT = True
CONVERT_MUSIC = True
CONVERT_SPRITE = True
CONVERT_STRINGS = True
CONVERT_COLORS = True
CONVERT_UI = True
CONVERT_SCENE = False

UI_COMMAND_START = 20 


SPRITE_PATHES = (
    "avatar\\ui",
    )

PATCH_SPRITE_PATHES = (
    "avatar\\ui\\magicbg", 
    )

UI_PATHS = (
    "ui",
    )

SCENE_PATHS = (
    "scene",
    )


def setConfig(project, name ,target):
    global PROJECT_PATH, GAME_NAME, TARGET, RAW_RESOURCE_PATH,BIN_RESOURCE_PATH,GENERATE_PATH
    PROJECT_PATH = project
    GAME_NAME = name
    TARGET= target

    RAW_RESOURCE_PATH = os.path.join(PROJECT_PATH, "resource", GAME_NAME)
    BIN_RESOURCE_PATH = os.path.join(PROJECT_PATH, "__release", GAME_NAME)
    GENERATE_PATH = os.path.join(PROJECT_PATH, "client", GAME_NAME, "Generate") 