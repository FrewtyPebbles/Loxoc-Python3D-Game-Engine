from os import getenv, listdir, path
import os
from setuptools import setup, Extension
from Cython.Build import cythonize
from dotenv import load_dotenv

load_dotenv()

MODULE_NAME = "Runespoor"

C_PATH = path.join(path.dirname(__file__), "src/")

INCLUDE_DIRS = [
    getenv("INCLUDE_DIR"),
    path.join(path.dirname(__file__), "glad/include/"),
]
LIBRARY_DIRS = [
    getenv("STATIC_LIB_DIR")
]

LIBRARIES = [f.replace(".lib", "") for f in listdir(getenv("STATIC_LIB_DIR")) if f.endswith(".lib")]

c_deps = [
    *[path.join(C_PATH, cppf) for cppf in listdir(C_PATH) if cppf.endswith(".cpp")],
    path.join(path.dirname(__file__), "glad/src/gl.c")
]

EXTENSIONS = [
    Extension(f"{MODULE_NAME}.core",
              sources=[path.join(path.dirname(__file__), MODULE_NAME, "core" + ".pyx"), *c_deps],
              language="c++",
              include_dirs=INCLUDE_DIRS,
              libraries=LIBRARIES + [
                  "kernel32", "user32", "gdi32", "winmm", "imm32", "ole32",
                  "oleaut32", "version", "uuid", "advapi32", "setupapi",
                  "shell32", "dinput8", "opengl32"
              ],
              library_dirs=LIBRARY_DIRS,
              extra_compile_args=["/MT", "/std:c++20", "/MP", "/Ox"],
              extra_link_args=[]
              )
]

readme_src = (fp:=open(path.join(path.dirname(__file__), "readme.md"), "r")).read()
fp.close()

setup(
    name=MODULE_NAME,
    version="1.0.0-dev0",
    author="William Lim",
    description = """
    Runespoor is a flexible, straight forward, multi-paradigm game engine that is built from the ground up with developer experience in mind.
    Utilizing a game loop system similar to pygame, you are able to abstract elements of your game away from the game loop as much as you'd like.
    """,
    long_description=readme_src,
    license="MIT",
    keywords="game engine sdl opengl 3d quaternion glm vec3 vector3",
    url="https://github.com/FrewtyPebbles/Runespoor-Python3D-Game-Engine",
    ext_modules = cythonize(EXTENSIONS,
        compiler_directives={
            "language_level": 3,
            "profile": False
        }
    ),
    classifiers=[
        "Development Status :: 2 - Pre-Alpha",
        "License :: OSI Approved :: MIT License",
        "Intended Audience :: Developers",
        "Topic :: Games/Entertainment"
    ],
    setup_requires=['wheel'],
    package_data={
        '': [
            'LICENSE',
            path.join(path.dirname(__file__), MODULE_NAME, "core" + ".pyi")
        ]
    },
    include_package_data=True,
)